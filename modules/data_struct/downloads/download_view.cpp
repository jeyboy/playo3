#include "download_view.h"

using namespace Playo3;

DownloadView * DownloadView::self = 0;

DownloadView * DownloadView::instance(QJsonObject * hash, QWidget * parent) {
    if(!self)
        self = new DownloadView(hash, parent);
    return self;
}

DownloadView::DownloadView(QJsonObject * hash, QWidget * parent)
    : QListView(parent), mdl(new DownloadModel(hash, this)), networkManager(new CustomNetworkAccessManager()) {

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
    qDeleteAll(bussyWatchers);

    delete mdl;
}

void DownloadView::scrollToActive() {
    scrollTo(currentIndex());
}

bool DownloadView::proceedDownload(QModelIndex & ind) {
    QFutureWatcher<QModelIndex> * newItem = 0;

    if (watchers.isEmpty()) {
        if (watchers.size() + bussyWatchers.size() < qMax(1,  QThread::idealThreadCount())) {
            newItem = new QFutureWatcher<QModelIndex>();
            connect(newItem, SIGNAL(finished()), this, SLOT(downloadCompleted()));
        }
        else return false;
    }
    else newItem = watchers.takeLast();

    mdl -> setData(ind, 0, DOWNLOAD_PROGRESS);
    bussyWatchers.append(newItem);
    newItem -> setFuture(QtConcurrent::run(this, &DownloadView::downloading, ind));
    return true;
}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////
void DownloadView::onUpdateAttr(const QModelIndex ind, int attr, QVariant val) {
    mdl -> setData(ind, val, attr);
}

void DownloadView::downloadCompleted() {
    QFutureWatcher<QModelIndex> * obj = (QFutureWatcher<QModelIndex> *)sender();
    QModelIndex ind = obj -> result();

    if (!ind.data(DOWNLOAD_ERROR).isValid()) {
//        QVariant v = ind.data();
//        qDebug() << v << " : " << removeRow(ind);
        mdl -> setData(ind, -100, DOWNLOAD_PROGRESS);
        setRowHidden(ind.row(), true);
    } else {
        qDebug() << "!ERR " << ind.data() << " " << ind.data(DOWNLOAD_ERROR);
        mdl -> setData(ind, -2, DOWNLOAD_PROGRESS);
//        mdl -> moveRow(QModelIndex(), ind.row(), QModelIndex(), mdl -> root() -> childCount()); //TODO: is broken
    }

    bussyWatchers.removeOne(obj);
    watchers.append(obj);
    proceedDownload();
}

void DownloadView::addRow(QUrl from, QString to, QString name) {
    QVariantMap data;
    data.insert(QString::number(DOWNLOAD_FROM), from);
    data.insert(QString::number(DOWNLOAD_TO), to.endsWith('/') ? to.mid(0, to.size() - 1) : to);
    data.insert(QString::number(DOWNLOAD_TITLE), name);
    data.insert(QString::number(DOWNLOAD_IS_REMOTE), !from.isLocalFile());
    data.insert(QString::number(DOWNLOAD_PROGRESS), -1);

    QModelIndex ind = mdl -> appendRow(data);
    proceedDownload(ind);
}

bool DownloadView::removeRow(const QModelIndex & node) {
    return mdl -> removeRow(node.row(), node.parent());
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
            if ((*it) -> data(DOWNLOAD_PROGRESS).toInt() == -1) {
                ind = mdl -> index((*it));
                if (!proceedDownload(ind))
                    return;
            }
        }
    }
}

QModelIndex DownloadView::downloading(QModelIndex & ind) {    
    DownloadModelItem * itm = mdl -> item(ind);

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
        int bufferLength, readed;
        double limit;
        qint64 pos = 0;
        QIODevice * source;

        QUrl from = itm -> data(DOWNLOAD_FROM).toUrl();

        if (itm -> data(DOWNLOAD_IS_REMOTE).toBool()) {
            source = networkManager -> openUrl(from);
            bufferLength = qMin(source -> bytesAvailable(), qint64(1024 * 1024 * 1)); //1 mb
        } else {
            source = new QFile(from.toLocalFile());

            if (!source -> open(QIODevice::ReadOnly)) {
                emit updateAttr(ind, DOWNLOAD_ERROR, source -> errorString());
                source -> close();
                delete source;
                toFile.close();
                return ind;
            }

            bufferLength = qMin(source -> bytesAvailable(), qint64(1024 * 1024 * 20)); //10 mb
        }

        limit = source -> bytesAvailable() / 100;

        if (!toFile.resize(source -> bytesAvailable())) {
            emit updateAttr(ind, DOWNLOAD_ERROR, source -> errorString());
            source -> close();
            delete source;
            toFile.close();
            return ind;
        }

        char * buffer = new char[bufferLength];

        while(!source -> atEnd()) {
            pos += (readed = source -> read(buffer, bufferLength));
            toFile.write(buffer, readed);

            emit updateAttr(ind, DOWNLOAD_PROGRESS, pos / limit);
        }

        emit updateAttr(ind, DOWNLOAD_PROGRESS, 100);

        source -> close();
        delete source;
        toFile.close();
        delete [] buffer;
    }
    else emit updateAttr(ind, DOWNLOAD_ERROR, toFile.errorString());

    return ind;
}

void DownloadView::contextMenuEvent(QContextMenuEvent * event) {
    event -> accept();

    QList<QAction *> actions;
    QAction * act;

    actions.append((act = new QAction("Restart all blocked", this)));
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
    QListView::dragEnterEvent(event);
}

void DownloadView::dragMoveEvent(QDragMoveEvent * event) {
    QListView::dragMoveEvent(event);
}

void DownloadView::dropEvent(QDropEvent * event) {
    QListView::dropEvent(event);
    event -> accept();
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
            drag -> setPixmap(QPixmap(":drag"));
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }
    else
        QListView::mouseMoveEvent(event); // if we add this to common loop - after dnd we lost focus on widget
}
