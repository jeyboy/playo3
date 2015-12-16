#include "download_view.h"
#include "modules/core/web/web_apis.h"

using namespace Views;
using namespace Core;
using namespace Web;

DownloadView::DownloadView(QJsonObject * hash, QWidget * parent) : QListView(parent),
    mdl(new DownloadModel(hash, this)), paused(false) {

    setModel(mdl);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setDragDropMode(QAbstractItemView::DragDrop);
//    setDefaultDropAction(Qt::CopyAction);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection); // ContiguousSelection

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setItemDelegate(new DownloadDelegate(this));

    setContextMenuPolicy(Qt::DefaultContextMenu);

    connect(this, SIGNAL(updateAttr(DownloadModelItem *,int,QVariant)), this, SLOT(onUpdateAttr(DownloadModelItem *,int,QVariant)));
    proceedDownload();
}

DownloadView::~DownloadView() {
    qDeleteAll(bussyWatchers.values());
    bussyWatchers.clear();

//    delete mdl;
}

QJsonObject DownloadView::toJson() {
    paused = true;

    for(QHash<DownloadModelItem *, QFutureWatcher<DownloadModelItem *> *>::Iterator it = bussyWatchers.begin(); it != bussyWatchers.end(); it++) {
        disconnect(it.value(), SIGNAL(finished()), this, SLOT(savingCompleted()));
        it.value() -> cancel();
        it.value() -> waitForFinished();
        it.key() -> setData(DOWNLOAD_PROGRESS, -1);
    }

    bussyWatchers.clear();
    return mdl -> toJson();
}

bool DownloadView::proceedDownload(DownloadModelItem * item) {
    if (bussyWatchers.count() > 3) return false;
    return initiateDownloading(item);

//    QFutureWatcher<QModelIndex> * newItem = 0;

//    if (watchers.isEmpty()) {
//        if (watchers.size() + bussyWatchers.size() < qMax(1,  2/*QThread::idealThreadCount()*/)) {
//            newItem = new QFutureWatcher<QModelIndex>();
//            connect(newItem, SIGNAL(finished()), this, SLOT(savingCompleted()));
//        }
//        else return false;
//    }
//    else newItem = watchers.takeLast();

//    mdl -> setData(ind, 0, DOWNLOAD_PROGRESS);
//    bussyWatchers.insert(ind, newItem);
//    newItem -> setFuture(QtConcurrent::run(this, &DownloadView::downloading, ind, newItem));
//    return true;
}

void DownloadView::proceedDrop(QDropEvent * event, const QString & path) {
    if (event -> mimeData() -> hasFormat(DROP_INNER_FORMAT)) {
        QByteArray encoded = event -> mimeData() -> data(DROP_INNER_FORMAT);
        QDataStream stream(&encoded, QIODevice::ReadOnly);
        bool isRemote;

        while (!stream.atEnd()) {
            InnerData data;
            stream >> data.url >> isRemote >> data.attrs;

            addRow(
                data.url,
                path,
                FilenameConversions::downloadTitle(data.attrs[JSON_TYPE_TITLE].toString(), data.attrs[JSON_TYPE_EXTENSION].toString()),
                data.attrs[JSON_TYPE_SUB_TYPE].toInt(),
                data.attrs[JSON_TYPE_REFRESH_PATH].toString()
            );
        }
    } else if (event -> mimeData() -> hasUrls()) {
        QList<QUrl> urls = event -> mimeData() -> urls();

        for(QList<QUrl>::Iterator it = urls.begin(); it != urls.end(); it++) {
            QFileInfo file = QFileInfo((*it).toLocalFile());
            addRow((*it), path, FilenameConversions::downloadTitle(file.baseName(), file.completeSuffix()));
        }
    }
    else {
        event -> ignore();
        return;
    }

    event -> accept();
}

bool DownloadView::initiateDownloading(DownloadModelItem * item) {
    QIODevice * source;

    Manager * networkManager = Manager::prepare();
    QUrl from = item -> data(DOWNLOAD_FROM).toUrl();
    bool isRemote = item -> data(DOWNLOAD_IS_REMOTE).toBool();

    if (isRemote) {
        QApplication::processEvents();
        source = networkManager -> followedGet(from);

        int status = ((QNetworkReply *)source) -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status == 404 || status == 0) { // status 0 has meaning what url is empty and should be refreshed
            source -> close();
            source -> deleteLater();

            QApplication::processEvents();
            bool invalid = true;
            QUrl newFrom = QUrl(
                Core::Web::Apis::restoreUrl( // maybe need to do this async
                    item -> data(DOWNLOAD_REFRESH_ATTRS).toString(),
                    (Web::SubType)item -> data(DOWNLOAD_TYPE).toInt()
                )
            );
            QApplication::processEvents();

            if (newFrom != from) {
                source = networkManager -> followedGet(newFrom);
                invalid = ((QNetworkReply *)source) -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404;
            }

            if (invalid) {
                emit updateAttr(item, DOWNLOAD_ERROR, QStringLiteral("unprocessable"));
                return false;
            }
        }

        emit updateAttr(item, REMOTE_PROGRESS, 0);
        downIndexes.insert(source, item);
        connect(source, SIGNAL(readyRead()), this, SLOT(downloadFinished()));
        connect(source, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadRemoteProgress(qint64,qint64)));
    } else {
        source = new QFile(from.toLocalFile());

        if (!source -> open(QIODevice::ReadOnly)) {
            emit updateAttr(item, DOWNLOAD_ERROR, Core::FileErrors::ioError((QFile *)source));
            source -> close();
            source -> deleteLater();
            return false;
        }

        initiateSaving(item, source);
    }

    return true;
}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////
void DownloadView::downloadFinished() {
    QIODevice * device = (QIODevice *)sender();
    DownloadModelItem * item = downIndexes.value(device);
    downIndexes.remove(device);

    initiateSaving(item, device);
}

void DownloadView::initiateSaving(DownloadModelItem * item, QIODevice * source) {
    QFutureWatcher<DownloadModelItem *> * newItem = new QFutureWatcher<DownloadModelItem *>();
    connect(newItem, SIGNAL(finished()), this, SLOT(savingCompleted()));
    item -> setData(DOWNLOAD_PROGRESS, 0);
    bussyWatchers.insert(item, newItem);
    newItem -> setFuture(QtConcurrent::run(this, &DownloadView::downloading, item, source, newItem));
}

void DownloadView::savingCompleted() {
    QFutureWatcher<DownloadModelItem *> * obj = (QFutureWatcher<DownloadModelItem *> *)sender();
    DownloadModelItem * item = bussyWatchers.key(obj);//obj -> result();
    bussyWatchers.remove(item);
    QModelIndex ind = mdl -> index(item);

    if (!ind.data(DOWNLOAD_ERROR).isValid()) {
        mdl -> setData(ind, -100, DOWNLOAD_PROGRESS);
        removeRow(item);
    } else {
        mdl -> setData(ind, -2, DOWNLOAD_PROGRESS);
//        mdl -> moveRow(QModelIndex(), ind.row(), QModelIndex(), mdl -> root() -> childCount()); //TODO: is broken
    }
    proceedDownload();
}

void DownloadView::addRow(const QUrl & from, const QString & to, const QString & name, int dtype, const QString & refresh_attrs) {
    QVariantMap data;
    data.insert(QString::number(DOWNLOAD_FROM), from);
    data.insert(QString::number(DOWNLOAD_TO), to.endsWith('/') ? to.mid(0, to.size() - 1) : to);
    data.insert(QString::number(DOWNLOAD_TITLE), name);
    data.insert(QString::number(DOWNLOAD_TYPE), dtype);

    if (!refresh_attrs.isEmpty())
        data.insert(QString::number(DOWNLOAD_REFRESH_ATTRS), refresh_attrs);
    data.insert(QString::number(DOWNLOAD_IS_REMOTE), !from.isLocalFile());
    data.insert(QString::number(DOWNLOAD_PROGRESS), -1);
    data.insert(QString::number(REMOTE_PROGRESS), -1);

    proceedDownload(mdl -> appendRow(data));
}

bool DownloadView::removeRow(DownloadModelItem * item) {
//    setRowHidden(node.row(), true);

//    if (bussyWatchers.contains(node))
//        bussyWatchers.value(node) -> cancel();

//    return true;

    QModelIndex node = mdl -> index(item);
    return mdl -> removeRow(node.row(), node.parent());
}

void DownloadView::downloadRemoteProgress(qint64 bytesReceived, qint64 bytesTotal) {
    QIODevice * source = (QIODevice *)sender();
    qDebug() << "DOWN" << downIndexes.value(source) << (bytesReceived / (double)bytesTotal);
    emit updateAttr(downIndexes.value(source), REMOTE_PROGRESS, bytesReceived / (double)bytesTotal);
}

void DownloadView::reproceedDownload() {
    QList<DownloadModelItem *> items =  mdl -> root() -> childList();

    for(QList<DownloadModelItem *>::Iterator it = items.begin(); it != items.end(); it++)
        if ((*it) -> data(DOWNLOAD_PROGRESS).toInt() == -2) {
            (*it) -> setData(DOWNLOAD_PROGRESS, -1);
            (*it) -> setData(REMOTE_PROGRESS, -1);
            (*it) -> setData(DOWNLOAD_ERROR, QVariant());
        }

    proceedDownload();
}

void DownloadView::proceedDownload() {
    QList<DownloadModelItem *> items =  mdl -> root() -> childList();

    for(QList<DownloadModelItem *>::Iterator it = items.begin(); it != items.end(); it++) {
        if (!paused && (*it) -> data(DOWNLOAD_PROGRESS).toInt() == -1) {
            if (!proceedDownload(*it))
                return;
        }
    }
}

DownloadModelItem * DownloadView::downloading(DownloadModelItem * itm, QIODevice * source, QFutureWatcher<DownloadModelItem *> * watcher) {
    QVariant toVar = itm -> data(DOWNLOAD_TO);
    QString to = toVar.toString() % '/' % itm -> data(DOWNLOAD_TITLE).toString();

    if (QFile::exists(to)) {
        QFile existed(to);
        if (existed.open(QIODevice::ReadOnly)) {
            bool already_existed = source -> bytesAvailable() == existed.size();
            existed.close();
            if (already_existed) return itm;
        }
        QFile::remove(to);
    }

    QFile toFile(to);
    bool isRemote = itm -> data(DOWNLOAD_IS_REMOTE).toBool();
    // QIODevice::Append | QIODevice::Unbuffered
    bool isReadyForWriting = toFile.open(QIODevice::WriteOnly) && toFile.resize(source -> bytesAvailable());

    if (isReadyForWriting) {
        qint64 pos = 0;
        double readTime = 15.0;
        int minBufferLen = 1024 * 128 * 4;  //512 kb
        int bufferLength = isRemote ?
                qMin(source -> bytesAvailable(), qint64(minBufferLen)) :
                qMin(source -> bytesAvailable(), qint64(1024 * 1024 * 2)); //2 mb

        double limit = source -> bytesAvailable() / 100;

        while(!source -> atEnd()) {
            {
                qint64 v = QDateTime::currentMSecsSinceEpoch();
                QByteArray buffer = source -> read(bufferLength);
                v = QDateTime::currentMSecsSinceEpoch() - v;
                pos += toFile.write(buffer);

                if (v < readTime)
                    bufferLength *= 1.1;/*(1 + (1.0 - v / readTime));*/
                else if (bufferLength > minBufferLen)
                    bufferLength /= 2;
            }

            emit updateAttr(itm, DOWNLOAD_PROGRESS, pos / limit);

            if (watcher -> isCanceled())
                break;

            if (isRemote) QThread::msleep(10);
        }

        emit downloadProceeded(to);
    }
    else emit updateAttr(itm, DOWNLOAD_ERROR, Core::FileErrors::ioError(&toFile));

    source -> close();
    source -> deleteLater();

    if (!isReadyForWriting || watcher -> isCanceled())
        toFile.remove();
    else {
        emit updateAttr(itm, DOWNLOAD_PROGRESS, 100);
        toFile.close();
    }

    return itm;
}

void DownloadView::contextMenuEvent(QContextMenuEvent * event) {
    QMenu menu(this);

    menu.addAction(QStringLiteral("Restart all blocked"), this, SLOT(reproceedDownload()));

    menu.exec(event -> globalPos());
    event -> accept();
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

void DownloadView::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> formats().contains(DROP_INNER_FORMAT) || event -> mimeData() -> formats().contains(DROP_OUTER_FORMAT))
        event -> accept();
    else
        event -> ignore();

    QListView::dragEnterEvent(event);
}

void DownloadView::dragMoveEvent(QDragMoveEvent * event) {
    if (event -> mimeData() -> formats().contains(DROP_INNER_FORMAT) || event -> mimeData() -> formats().contains(DROP_OUTER_FORMAT))
        event -> accept();
    else
        event -> ignore();

    QListView::dragMoveEvent(event);
}

void DownloadView::dropEvent(QDropEvent * event) {
    proceedDrop(event, Settings::obj().defaultDownloadPath());
}

void DownloadView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        selectionModel() -> clearSelection();

        if (list.size() > 1) {
            qSort(list.begin(), list.end());

            QModelIndexList::Iterator eit = --list.end();
            for (; eit != list.begin(); --eit) {
                DownloadModelItem * itm = mdl -> item(*eit);
                QFutureWatcher<DownloadModelItem *> * watcher = bussyWatchers.value(itm, 0);
                if (!watcher)/* {
                    watcher -> cancel();
                    watcher -> waitForFinished();
                }*/
                    removeRow(itm);
            }

            DownloadModelItem * itm = mdl -> item(*eit);
            if (!bussyWatchers.value(itm, 0))
                removeRow(itm);
        } else {
            QModelIndex ind = currentIndex();
            if (ind.isValid())
                removeRow(mdl -> item(ind));
        }
    }
    else QListView::keyPressEvent(event);
}

void DownloadView::mousePressEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton)
        dragPoint = event -> pos();

    QListView::mousePressEvent(event);
}

void DownloadView::mouseMoveEvent(QMouseEvent * event) {
    if ((event -> buttons() == Qt::LeftButton) && (dragPoint - event -> pos()).manhattanLength() >= 10){
        if (selectedIndexes().length() > 0) {
            QDrag * drag = new QDrag(this);
            QMimeData * mimeData = model() -> mimeData(selectedIndexes());
            drag -> setPixmap(QPixmap(QStringLiteral(":drag")));
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }
    else
        QListView::mouseMoveEvent(event); // if we add this to common loop - after dnd we lost focus on widget
}
