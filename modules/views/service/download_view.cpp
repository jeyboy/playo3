#include "download_view.h"

using namespace View;
using namespace Core;
using namespace Web;

DownloadView::DownloadView(QJsonObject * hash, QWidget * parent) : QListView(parent),
    paused(false), mdl(new DownloadModel(hash, this)) {

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

    setItemDelegate((item_delegate = new DownloadDelegate(this)));

    setContextMenuPolicy(Qt::DefaultContextMenu);

    connect(this, SIGNAL(updateAttr(QModelIndex,int,QVariant)), this, SLOT(onUpdateAttr(QModelIndex,int,QVariant)));
    proceedDownload();
}

DownloadView::~DownloadView() {
    qDeleteAll(watchers);   
    watchers.clear();
    qDeleteAll(bussyWatchers.values());
    bussyWatchers.clear();

    delete mdl;
}

QJsonObject DownloadView::toJson() {
    paused = true;

    for(QHash<QModelIndex, QFutureWatcher<QModelIndex> *>::Iterator it = bussyWatchers.begin(); it != bussyWatchers.end(); it++) {
        disconnect(it.value(), SIGNAL(finished()), this, SLOT(downloadCompleted()));
        it.value() -> cancel();
        it.value() -> waitForFinished();

        watchers.append(it.value());
        mdl -> setData(it.key(), -1, DOWNLOAD_PROGRESS);
    }

    bussyWatchers.clear();
    proceedDownload();

    return mdl -> toJson();
}

bool DownloadView::proceedDownload(QModelIndex & ind) {
    QFutureWatcher<QModelIndex> * newItem = 0;

    if (watchers.isEmpty()) {
        if (watchers.size() + bussyWatchers.size() < qMax(1,  2/*QThread::idealThreadCount()*/)) {
            newItem = new QFutureWatcher<QModelIndex>();
            connect(newItem, SIGNAL(finished()), this, SLOT(downloadCompleted()));
        }
        else return false;
    }
    else newItem = watchers.takeLast();

    mdl -> setData(ind, 0, DOWNLOAD_PROGRESS);
    bussyWatchers.insert(ind, newItem);
    newItem -> setFuture(QtConcurrent::run(this, &DownloadView::downloading, ind, newItem));
    return true;
}

void DownloadView::proceedDrop(QDropEvent * event, QString path) {
    if (event -> mimeData() -> hasFormat(DROP_INNER_FORMAT)) {
        event -> accept();

        QByteArray encoded = event -> mimeData() -> data(DROP_INNER_FORMAT);
        QDataStream stream(&encoded, QIODevice::ReadOnly);
        bool isRemote;

        while (!stream.atEnd()) {
            InnerData data;
            stream >> data.url >> isRemote >> data.attrs;

            bool is_vk = data.attrs.take(JSON_TYPE_ITEM_TYPE).toInt() == VK_FILE; // vk monkey patch
            // need to add patch for 4shared // need to prepare download link before row creation

            addRow(
                data.url,
                path,
                FilenameConversions::downloadTitle(data.attrs[JSON_TYPE_TITLE].toString(), data.attrs[JSON_TYPE_EXTENSION].toString()),
                is_vk ? QStringLiteral("vk") : QString(),
                is_vk ? (data.attrs[JSON_TYPE_OWNER_ID].toString() % QStringLiteral("_") % data.attrs[JSON_TYPE_UID].toString()) : QString() //WebItem::toUid
            );
        }
    } else if (event -> mimeData() -> hasUrls()) {
        event -> accept();
        QList<QUrl>::Iterator it = event -> mimeData() -> urls().begin();

        for(; it != event -> mimeData() -> urls().end(); it++) {
            QFileInfo file = QFileInfo((*it).toLocalFile());
            addRow((*it), path, FilenameConversions::downloadTitle(file.baseName(), file.completeSuffix()));
        }
    }
    else event -> ignore();
}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////
void DownloadView::onUpdateAttr(const QModelIndex ind, int attr, QVariant val) {
    mdl -> setData(ind, val, attr);
}

void DownloadView::downloadCompleted() {
    QFutureWatcher<QModelIndex> * obj = (QFutureWatcher<QModelIndex> *)sender();
    QModelIndex ind = bussyWatchers.key(obj);//obj -> result();

    if (!ind.data(DOWNLOAD_ERROR).isValid()) {
        mdl -> setData(ind, -100, DOWNLOAD_PROGRESS);
        setRowHidden(ind.row(), true);
    } else {
        mdl -> setData(ind, -2, DOWNLOAD_PROGRESS);
//        mdl -> moveRow(QModelIndex(), ind.row(), QModelIndex(), mdl -> root() -> childCount()); //TODO: is broken
    }

    bussyWatchers.remove(ind);
    watchers.append(obj);
    proceedDownload();
}

void DownloadView::addRow(QUrl from, QString to, QString name, QString dtype, QString uid) {
    QVariantMap data;
    data.insert(QString::number(DOWNLOAD_FROM), from);
    data.insert(QString::number(DOWNLOAD_TO), to.endsWith('/') ? to.mid(0, to.size() - 1) : to);
    data.insert(QString::number(DOWNLOAD_TITLE), name);

    if (!dtype.isEmpty())
        data.insert(QString::number(DOWNLOAD_TYPE), dtype);

    if (!uid.isEmpty())
        data.insert(QString::number(DOWNLOAD_ID), uid);
    data.insert(QString::number(DOWNLOAD_IS_REMOTE), !from.isLocalFile());
    data.insert(QString::number(DOWNLOAD_PROGRESS), -1);

    QModelIndex ind = mdl -> appendRow(data);
    proceedDownload(ind);
}

bool DownloadView::removeRow(const QModelIndex & node) {
    setRowHidden(node.row(), true);

    if (bussyWatchers.contains(node))
        bussyWatchers.value(node) -> cancel();

    return true;
//    return mdl -> removeRow(node.row(), node.parent());
}

void DownloadView::reproceedDownload() {
    QList<DownloadModelItem *> items =  mdl -> root() -> childList();
    QList<DownloadModelItem *>::Iterator it = items.begin();

    for(int i = 0; it != items.end(); it++, i++)
        if ((*it) -> data(DOWNLOAD_PROGRESS).toInt() == -2) {
            (*it) -> setData(DOWNLOAD_PROGRESS, -1);
            (*it) -> setData(DOWNLOAD_ERROR, QVariant());
        }

    proceedDownload();
}

void DownloadView::proceedDownload() {
    QList<DownloadModelItem *> items =  mdl -> root() -> childList();
    QModelIndex ind;

    QList<DownloadModelItem *>::Iterator it = items.begin();

    for(int i = 0; it != items.end(); it++, i++) {
        if (isRowHidden(i)) {
            if (bussyWatchers.isEmpty())
                mdl -> removeRow(i--, QModelIndex());
        } else {
            if (!paused && (*it) -> data(DOWNLOAD_PROGRESS).toInt() == -1) {
                ind = mdl -> index((*it));
                if (!proceedDownload(ind))
                    return;
            }
        }
    }
}

QModelIndex DownloadView::downloading(QModelIndex & ind, QFutureWatcher<QModelIndex> * watcher) {
    DownloadModelItem * itm = mdl -> item(ind);
    Manager * networkManager = Manager::prepare();

    QString to;
    QVariant toVar = itm -> data(DOWNLOAD_TO);

//    if (toVar.type() == typeof(QUrl))
//        to = toVar.toUrl().toLocalFile();
//    else
        to = toVar.toString() + '/' + itm -> data(DOWNLOAD_TITLE).toString();

    if (QFile::exists(to))
        QFile::remove(to);

    QFile toFile(to);

    ////    QIODevice::Append | QIODevice::Unbuffered
    if (toFile.open(QIODevice::WriteOnly)) {
        int bufferLength, minBufferLen = 1024 * 128 * 4;  //512 kb
        double limit;
        qint64 pos = 0;
        double readTime = 15.0;
        QIODevice * source;

        QUrl from = itm -> data(DOWNLOAD_FROM).toUrl();
        bool isRemote = itm -> data(DOWNLOAD_IS_REMOTE).toBool();

        if (isRemote) {
            source = networkManager -> followedGet(from);
            int status = ((QNetworkReply *)source) -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); // vk monkey patch
            if (status == 404) {
                source -> close();
                delete source;

                bool invalid = true;

                if (itm -> data(DOWNLOAD_TYPE).toString() == QStringLiteral("vk")) {
                    QUrl newFrom = QUrl(Vk::Api::instance() -> refresh(itm -> data(DOWNLOAD_ID).toString()));
                    if (newFrom != from) {
                        source = networkManager -> followedGet(newFrom);
                        invalid = ((QNetworkReply *)source) -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404;
                    }
                }

                if (invalid) {
                    emit updateAttr(ind, DOWNLOAD_ERROR, QStringLiteral("unprocessable"));
                    toFile.remove();
                    delete networkManager;
                    return ind;
                }
            }

            bufferLength = qMin(source -> bytesAvailable(), qint64(minBufferLen));
        } else {
            source = new QFile(from.toLocalFile());

            if (!source -> open(QIODevice::ReadOnly)) {
                emit updateAttr(ind, DOWNLOAD_ERROR, Core::FileErrors::ioError((QFile *)source));
                source -> close();
                delete source;
                toFile.close();
                delete networkManager;
                return ind;
            }

            bufferLength = qMin(source -> bytesAvailable(), qint64(1024 * 1024 * 2)); //2 mb
        }

        limit = source -> bytesAvailable() / 100;

        if (!toFile.resize(source -> bytesAvailable())) {
            emit updateAttr(ind, DOWNLOAD_ERROR, Core::FileErrors::ioError(&toFile));
            source -> close();
            delete source;
            toFile.close();
            delete networkManager;
            return ind;
        }

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

            emit updateAttr(ind, DOWNLOAD_PROGRESS, pos / limit);

            if (watcher -> isCanceled())
                break;

            if (isRemote) QThread::msleep(10);
        }

        if (!watcher -> isCanceled())
            emit updateAttr(ind, DOWNLOAD_PROGRESS, 100);

        source -> close();
        delete source;
        delete networkManager;
        if (watcher -> isCanceled())
            toFile.remove();
        else
            toFile.close();

        emit downloadProceeded(to);
    }
    else emit updateAttr(ind, DOWNLOAD_ERROR, Core::FileErrors::ioError(&toFile));

    return ind;
}

void DownloadView::contextMenuEvent(QContextMenuEvent * event) {
    event -> accept();

    QList<QAction *> actions;
    QAction * act;

    actions.append((act = new QAction(QStringLiteral("Restart all blocked"), this)));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(reproceedDownload()));

//    if (isEditable()) {
//        actions.append((act = new QAction(QIcon(":/settings"), "View settings", this)));
//        connect(act, SIGNAL(triggered(bool)), Dockbars::instance(), SLOT(editActiveBar()));

//        actions.append((act = new QAction(this)));
//        act -> setSeparator(true);
//    }

//    if (Player::instance() -> playedIndex().isValid()) {
//        actions.append((act = new QAction(QIcon(":/active_tab"), "Show active elem", this)));
//        connect(act, SIGNAL(triggered(bool)), Dockbars::instance(), SLOT(scrollToActive()));

//        actions.append((act = new QAction(this)));
//        act -> setSeparator(true);
//    }

//    QModelIndex ind = indexAt(event -> pos());

//    if (ind.isValid()) {
//        if (!ind.data(IFULLPATH).toString().isEmpty()) {
//            actions.append((act = new QAction(QIcon(":/open"), "Open location", this)));
//            connect(act, SIGNAL(triggered(bool)), this, SLOT(openLocation()));
//        }

//        if (ind.data(IREMOTE).toBool()) {
//            //    openAct = new QAction(QIcon(":/refresh"), "Refresh", this);
//            //    connect(openAct, SIGNAL(triggered(bool)), model, SLOT(refresh()));
//            //    actions.append(openAct);

//            //    if (model -> getApi() != 0) {
//            //        openAct = new QAction(QIcon(":/refresh"), "Friends (groups) audio", this);
//            //        connect(openAct, SIGNAL(triggered(bool)), QApplication::activeWindow(), SLOT(showVKRelTabDialog()));
//            //        actions.append(openAct);
//            //    }

//            //    if (QString(metaObject() -> className()) == QString("VkView")) {
//            //        openAct = new QAction(QIcon(":/refresh"), "Parse/Refresh Wall", this);
//            //        connect(openAct, SIGNAL(triggered(bool)), model, SLOT(refreshWall()));
//            //        actions.append(openAct);
//            //    }

////            sepAct = new QAction(this);
////            sepAct -> setSeparator(true);
////            actions.append(sepAct);

//            actions.append((act = new QAction(QIcon(":/download"), "Download", this)));
//            connect(act, SIGNAL(triggered(bool)), this, SLOT(download()));

//            actions.append((act = new QAction(QIcon(":/download"), "Download All", this)));
//            connect(act, SIGNAL(triggered(bool)), this, SLOT(downloadAll()));
//        }
//    }

//    if (mdl -> rowCount() > 0) {
//        actions.append((act = new QAction(QIcon(":/shuffle"), "Shuffle", this)));
//        connect(act, SIGNAL(triggered(bool)), this, SLOT(shuffle()));

//        if (mdl -> containerType() != list) {
//            actions.append((act = new QAction(this)));
//            act -> setSeparator(true);

//            actions.append((act = new QAction(QIcon(":/collapse"), "Collapse all", this)));
//            connect(act, SIGNAL(triggered(bool)), this, SLOT(collapseAll()));

//            actions.append((act = new QAction(QIcon(":/expand"), "Expand all", this)));
//            connect(act, SIGNAL(triggered(bool)), this, SLOT(expandAll()));
//        }
//    }

    if (actions.count() > 0)
        QMenu::exec(actions, event -> globalPos(), 0, this);
}

void DownloadView::removeProccessing(QModelIndexList & index_list) {
    qSort(index_list.begin(), index_list.end());

    QModelIndexList::Iterator eit = --index_list.end();
    for (; eit != index_list.begin(); --eit)
        removeRow((*eit));

    removeRow((*eit));

    index_list.clear();
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

void DownloadView::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> formats().contains(DROP_INNER_FORMAT) || event -> mimeData() -> formats().contains(DROP_OUTER_FORMAT))
        event -> accept();
    else
        event -> ignore();

//    QListView::dragEnterEvent(event);
}

void DownloadView::dragMoveEvent(QDragMoveEvent * event) {
    if (event -> mimeData() -> formats().contains(DROP_INNER_FORMAT) || event -> mimeData() -> formats().contains(DROP_OUTER_FORMAT))
        event -> accept();
    else
        event -> ignore();
//    QListView::dragMoveEvent(event);
}

void DownloadView::dropEvent(QDropEvent * event) {
    proceedDrop(event, Settings::obj().defaultDownloadPath());
}

void DownloadView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        selectionModel() -> clearSelection();

        if (list.size() > 1)
            removeProccessing(list);
        else {
            QModelIndex ind = currentIndex();
            if (currentIndex().isValid())
                removeRow(ind);
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
