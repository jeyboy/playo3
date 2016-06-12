#include "download_view.h"
#include "modules/core/web/web_apis.h"

using namespace Views;
using namespace Core;
using namespace Web;

DownloadView::DownloadView(QJsonObject * hash, QWidget * parent) : QListView(parent), toggleNeeded(false),
    mdl(new DownloadModel(hash, this)), paused(false) {

    setModel(mdl);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::CopyAction);

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
        if (it.value()) {
            disconnect(it.value(), SIGNAL(finished()), this, SLOT(savingCompleted()));
            it.value() -> cancel();
            it.value() -> waitForFinished();
        }
        it.key() -> setData(DOWNLOAD_PROGRESS, -1);
        it.key() -> setData(REMOTE_PROGRESS, -1);
    }

    bussyWatchers.clear();
    return mdl -> toJson();
}

bool DownloadView::proceedDownload(DownloadModelItem * item) {
    if (bussyWatchers.contains(item)) return false;
    if (bussyWatchers.count() > 3) return false;
    bussyWatchers.insert(item, 0);
    return initiateDownloading(item);
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
                FilenameConversions::downloadTitle(data.attrs[JSON_TYPE_TITLE].toString(), data.attrs.value(JSON_TYPE_EXTENSION, QStringLiteral("mp3")).toString()),
                data.attrs[JSON_TYPE_ITEM_TYPE].toInt(),
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

    if (parentWidget() -> isHidden()) {
        if (Settings::obj().isShowDownloadingOnStart()) {
            toggleNeeded = true;
            parentWidget() -> show();
        } else toggleNeeded = false;
    }

    Manager * networkManager = Manager::prepare();
    QUrl from = item -> data(DOWNLOAD_FROM).toUrl();
    bool isRemote = item -> data(DOWNLOAD_IS_REMOTE).toBool();

    if (isRemote) {
        if (from.isEmpty()) {
            from = restoreUrl(item);
            if (from.isEmpty()) {
                setItemError(item);
                bussyWatchers.remove(item);
                return false;
            }
        }

        source = networkManager -> getFollowedAsync(from, Func(this, SLOT(asyncRequestFinished(QIODevice*,void*)), item));
        emit updateAttr(item, REMOTE_PROGRESS, 0);
        downIndexes.insert(source, item);
        connect(source, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadRemoteProgress(qint64,qint64)));
        return true;
    } else {
        source = new QFile(from.toLocalFile());

        if (!source -> open(QIODevice::ReadOnly)) {
            setItemError(item, Core::FileErrors::ioError((QFile *)source));
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
void DownloadView::initiateSaving(DownloadModelItem * item, QIODevice * source) {
    QFutureWatcher<DownloadModelItem *> * newItem = new QFutureWatcher<DownloadModelItem *>();
    connect(newItem, SIGNAL(finished()), this, SLOT(savingCompleted()));
    item -> setData(REMOTE_PROGRESS, -1);
    item -> setData(DOWNLOAD_PROGRESS, 0);
    bussyWatchers.insert(item, newItem);
    newItem -> setFuture(QtConcurrent::run(this, &DownloadView::saving, item, source, newItem));
}

void DownloadView::savingCompleted() {
    QFutureWatcher<DownloadModelItem *> * obj = (QFutureWatcher<DownloadModelItem *> *)sender();
    DownloadModelItem * item = bussyWatchers.key(obj);//obj -> result();

    if (item) {
        bussyWatchers.remove(item);
        QModelIndex ind = mdl -> index(item);
        obj -> deleteLater();

        if (!ind.data(DOWNLOAD_ERROR).isValid()) {
            removeRow(item);

            if (toggleNeeded && mdl -> root() -> childList().isEmpty()) {
                toggleNeeded = false;
                parentWidget() -> hide();
            }
        } else {
    //        mdl -> moveRow(QModelIndex(), ind.row(), QModelIndex(), mdl -> root() -> childCount()); //TODO: is broken
        }
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

    DownloadModelItem * itm = mdl -> appendRow(data);
    initiateItem(itm);
    proceedDownload(itm);
}

bool DownloadView::removeRow(DownloadModelItem * item) {
    QIODevice * device = 0;
    if (bussyWatchers.contains(item)) {
        device = downIndexes.key(item);
        if (device) {
            device -> disconnect();
            device -> close();
        }

        QFutureWatcher<DownloadModelItem *> * watcher = bussyWatchers.take(item);
        if (watcher) {
            watcher -> cancel();
            watcher -> waitForFinished();
            watcher -> deleteLater();
        }
    }

    if (device) {
        downIndexes.remove(device);
        device -> deleteLater();
    }
    QModelIndex node = mdl -> index(item);
    return mdl -> removeRow(node.row(), node.parent());
}

void DownloadView::asyncRequestFinished(QIODevice * source, void * userData) {
    DownloadModelItem * item = (DownloadModelItem *)userData;
    QUrl from = item -> data(DOWNLOAD_FROM).toUrl();
    downIndexes.remove(source);

    int status = ((QNetworkReply *)source) -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status == 404 || status == 500) { // od return 500 status on too old links
        qDebug() << "REMOTE CALL RESTORATION" << from;
        qDebug() << ((QNetworkReply *)source) -> readAll();
        source -> close();
        source -> deleteLater();
        bool invalid = true;

        if (item -> data(DOWNLOAD_REFRESH_ATTEMPTS).toInt() == 0) {
            qDebug() << "REMOTE CALL RESTORATION IN" << from;
            QUrl newFrom = restoreUrl(item);
            QApplication::processEvents();

            if (newFrom != from) {
                item -> setData(DOWNLOAD_FROM, newFrom);
                initiateDownloading(item);
                return;
            }
        }

        qDebug() << "REMOTE CALL RESTORATION" << from << !invalid;
        if (invalid) {
            setItemError(item);
            bussyWatchers.remove(item);
            return;
        }
    }

    initiateSaving(item, source);
}

void DownloadView::downloadRemoteProgress(qint64 bytesReceived, qint64 bytesTotal) {
    QIODevice * source = (QIODevice *)sender();
    emit updateAttr(downIndexes.value(source), REMOTE_PROGRESS, bytesReceived / (double)bytesTotal * 100);
}

void DownloadView::reproceedDownload() {
    QList<DownloadModelItem *> items =  mdl -> root() -> childList();

    for(QList<DownloadModelItem *>::Iterator it = items.begin(); it != items.end(); it++)
        if ((*it) -> data(DOWNLOAD_PROGRESS).toInt() == -2)
            initiateItem((*it));

    proceedDownload();
}

void DownloadView::proceedDownload() {
    QList<DownloadModelItem *> items =  mdl -> root() -> childList();

    for(QList<DownloadModelItem *>::Iterator it = items.begin(); it != items.end(); it++) {
        if (paused || (*it) -> data(DOWNLOAD_ERROR).isValid() || (*it) -> data(DOWNLOAD_FINISHED).toBool())
            continue;
        proceedDownload(*it);
    }
}

void DownloadView::initiateItem(DownloadModelItem * itm) {
    itm -> setData(DOWNLOAD_PROGRESS, -1);
    itm -> setData(REMOTE_PROGRESS, -1);
    itm -> setData(DOWNLOAD_REFRESH_ATTEMPTS, 0);
    itm -> setData(DOWNLOAD_FINISHED, false);
    itm -> setData(DOWNLOAD_ERROR, QVariant());
}

void DownloadView::setItemError(DownloadModelItem * itm, const QString & err) {
    emit updateAttr(itm, DOWNLOAD_ERROR, err);
}

QUrl DownloadView::restoreUrl(DownloadModelItem * item) {
    if (item -> data(DOWNLOAD_REFRESH_ATTEMPTS).toInt() == 0) {
        item -> setData(DOWNLOAD_REFRESH_ATTEMPTS, item -> data(DOWNLOAD_REFRESH_ATTEMPTS).toInt() + 1);

        QApplication::processEvents();

        return QUrl(
            Core::Web::Apis::restoreUrl(
                item -> data(DOWNLOAD_REFRESH_ATTRS).toString(),
                (DataSubType)item -> data(DOWNLOAD_TYPE).toInt()
            )
        );
    }

    return QUrl();
}

DownloadModelItem * DownloadView::saving(DownloadModelItem * itm, QIODevice * source, QFutureWatcher<DownloadModelItem *> * watcher) {
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
    else setItemError(itm, Core::FileErrors::ioError(&toFile));

    source -> close();
    source -> deleteLater();

    if (!isReadyForWriting || watcher -> isCanceled())
        toFile.remove();
    else {
        itm -> setData(DOWNLOAD_FINISHED, true);
        emit updateAttr(itm, DOWNLOAD_PROGRESS, 100);
        toFile.close();
    }

    return itm;
}

void DownloadView::contextMenuEvent(QContextMenuEvent * event) {
    QMenu menu(this);

    menu.addAction(QStringLiteral("Restart all unsuccessfull"), this, SLOT(reproceedDownload()));

    menu.exec(event -> globalPos());
    event -> accept();
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

void DownloadView::dragEnterEvent(QDragEnterEvent * event) {
    if (!event -> mimeData() -> formats().toSet().intersect(mdl -> mimeTypes().toSet()).isEmpty())
//    if (event -> mimeData() -> formats().contains(DROP_INNER_FORMAT) || event -> mimeData() -> formats().contains(DROP_OUTER_FORMAT))
        event -> acceptProposedAction();
}

void DownloadView::dragMoveEvent(QDragMoveEvent * event) {
    if (!event -> mimeData() -> formats().toSet().intersect(mdl -> mimeTypes().toSet()).isEmpty())
        event -> acceptProposedAction();
}

void DownloadView::dropEvent(QDropEvent * event) {
    proceedDrop(event, Settings::obj().defaultDownloadPath());
}

void DownloadView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        selectionModel() -> clearSelection();

        if (list.size() > 1) {
            qSort(list.begin(), list.end(), qGreater<QModelIndex>());

            for (QModelIndexList::Iterator it = list.begin(); it != list.end(); it++)
                removeRow(mdl -> item(*it));
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
