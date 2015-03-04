#include "download_view.h"

using namespace Playo3;

DownloadView * DownloadView::self = 0;

DownloadView * DownloadView::instance(QWidget * parent) {
    if(!self)
        self = new DownloadView(parent);
    return self;
}

DownloadView::DownloadView(QWidget * parent)
    : QListView(parent), mdl(new DownloadModel(this)) {

    setModel(mdl);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setDragDropMode(QAbstractItemView::InternalMove);
//    setDefaultDropAction(Qt::CopyAction);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection); // ContiguousSelection

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//    setItemDelegate((item_delegate = new DownloadDelegate(this)));

    setContextMenuPolicy(Qt::DefaultContextMenu);
}

DownloadView::~DownloadView() {
    delete mdl;
}

void DownloadView::scrollToActive() {
    scrollTo(currentIndex());
}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////

void DownloadView::openLocation() {
    DownloadModelItem * item = mdl -> item(currentIndex());
//    item -> openLocation();
}

void DownloadView::addRow(const QVector<QVariant> & data) {
    mdl -> appendRow(data);
}

bool DownloadView::removeRow(const QModelIndex & node) {
    return mdl -> removeRow(node.row(), node.parent());
}

void DownloadView::contextMenuEvent(QContextMenuEvent * event) {
    event -> accept();

    QList<QAction *> actions;
    QAction * act;

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

void DownloadView::removeProccessing(QModelIndexList & index_list, bool inProcess) {
    int total = index_list.size(), temp = total;

//    if (inProcess)
//        emit mdl -> moveInProcess();

    qSort(index_list.begin(), index_list.end());

    QModelIndexList::Iterator eit = --index_list.end();

    for (; eit != index_list.begin(); --eit) {
        removeRow((*eit));

//        if (inProcess)
//            emit mdl -> setProgress(--temp * 100.0 / total);
    }

//    if (inProcess)
//        emit threadedRowRemoving((*eit));
//    else
        removeRow((*eit));

    index_list.clear();
//    if (inProcess)
//        emit mdl -> moveOutProcess();
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

//        if (list.size() > 200)
//            QtConcurrent::run(this, &DownloadView::removeProccessing, list, true);
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
