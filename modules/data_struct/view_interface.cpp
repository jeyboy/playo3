#include "view_interface.h"
#include "dockbars.h"

using namespace Playo3;

ViewInterface::ViewInterface(ModelInterface * newModel, QWidget * parent, ViewSettings & settings)
    : QTreeView(parent), mdl(newModel), sttngs(settings), forwardOrder(true)  {

    setIndentation(12);
    setStyle(new TreeViewStyle);
    setStyleSheet(Stylesheets::treeViewStyles());

    setModel(mdl);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::CopyAction);

    setExpandsOnDoubleClick(true);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ContiguousSelection); // ExtendedSelection

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setItemDelegate((item_delegate = new ModelItemDelegate(this)));

    setContextMenuPolicy(Qt::DefaultContextMenu);
    int iconDimension = Settings::instance() -> getIconHeight();
    setIconSize(QSize(iconDimension, iconDimension));

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClick(const QModelIndex &)));
    connect(this, SIGNAL(expanded(const QModelIndex &)), mdl, SLOT(expanded(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), mdl, SLOT(collapsed(const QModelIndex &)));

    connect(mdl, SIGNAL(expandNeeded(const QModelIndex &)), this, SLOT(expand(const QModelIndex &)));
    connect(mdl, SIGNAL(spoilNeeded(const QModelIndex &)), this, SLOT(onSpoilNeeded(const QModelIndex &)));

    connect(Player::instance(), SIGNAL(itemExecError(QModelIndex &)), this, SLOT(itemError(QModelIndex &)));
    connect(Player::instance(), SIGNAL(itemNotSupported(QModelIndex &)), this, SLOT(itemNotSupported(QModelIndex &)));

//    connect(model, SIGNAL(itemsCountChanged(int)), parent, SLOT(updateHeader(int)));
//    connect(model, SIGNAL(showSpinner()), this, SLOT(startRoutine()));
//    connect(model, SIGNAL(hideSpinner()), this, SLOT(stopRoutine()));
//    connect(model, SIGNAL(updated()), this, SLOT(modelUpdate()));
//    connect(model, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)));

    header() -> hide();
//    header() -> setSectionResizeMode(0, QHeaderView::Interactive);
//    header()->setStretchLastSection(false);


    setDragDropOverwriteMode(true); // TODO: need modifications in ModelInterface::dropMimeData

    ////    setTreePosition(2);
    ////    setRootIndex();
    ////    setRowHidden(0, rootIndex(), true);

    ////    expandAll();
}

ViewInterface::~ViewInterface() {
    delete mdl;
}

void ViewInterface::scrollToActive() {
    if (Player::instance() -> playedIndex().isValid())
        scrollTo(Player::instance() -> playedIndex());
}

void ViewInterface::execPrevIndex(bool deleteCurrent) {
    forwardOrder = false;
    findAndExecIndex(deleteCurrent);
}

void ViewInterface::execNextIndex(bool deleteCurrent) {
    forwardOrder = true;
    findAndExecIndex(deleteCurrent);
}

//void View::markSelectedAsLiked() {
//    ModelItem *temp;
//    foreach (const QModelIndex &index, selectedIndexes()) {
//        if (index.isValid()) {
//            temp = model -> getItem(index);
//            if (!temp -> isFolder()) {
//                temp -> setState(STATE_LIKED);
//                model -> refreshItem(temp);
//            }
//        }
//    }
//}

bool ViewInterface::execIndex(const QModelIndex & node) {
    Dockbars::instance() -> setCurrent((DockBar *)parent());

    if (node.isValid() && node.data(IPLAYABLE).toBool()) {
        if (Settings::instance() -> isSpoilOnActivation())
            scrollTo(node);

        Player::instance() -> playIndex(node, false);
        return true;
    }

    return false;
}

//QModelIndex ViewInterface::removeCandidate(QModelIndex item) {
//    ModelItem * parent = item -> parent();

//    while(parent -> childCount() == 1 && parent -> parent() != 0) {
//        item = parent;
//        parent = parent -> parent();
//    }

//    return item;
//}

//void ViewInterface::removeItem(QModelIndex & item) { //TODO: rewrite
//    Library::instance() -> removeRemoteItem(item);
//    item = removeCandidate(item);
//    QModelIndex modelIndex = model -> index(item);
//    QString delPath = item -> fullPath();
//    bool isFolder = item -> isFolder();

//    QModelIndex parentIndex = modelIndex.parent();
//    if (!parentIndex.isValid())
//        parentIndex = rootIndex();
//    int row = modelIndex.row();
//    QModelIndex selCandidate = parentIndex.child(row + 1, 0);

//    if (isFolder && item -> childTreeCount() > 1) {
//        if (QMessageBox::warning(
//                    parentWidget(),
//                    "Folder deletion",
//                    "Are you shure what you want remove the not empty folder ?",
//                    QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
//            return;
//    }

//    clearSelection();

//    if (Player::instance() -> playedItem()) {
//        if (Player::instance() -> playedItem() -> fullPath().startsWith(delPath))
//            Player::instance() -> removePlaylist();
//    }

//    if (isRemoveFileWithItem()) {
//        item -> removePhysicalObject();

////        if (isFolder) {
//////                QDir delDir(delPath);
//////                if (delPath.split('/').length() >= 2) {
//////                    delDir.removeRecursively();
//////                }


////        } else {
////            QFile::remove(delPath);
////        }
//    }

//    if (model -> removeRow(row, parentIndex)) {
//        if (isFolder) {
//            updateSelection(selCandidate);
//        } else {
//            QModelIndex next = parentIndex.child(row, 0);
//            if (!next.isValid() || (next.isValid() && !next.data(FOLDERID).toBool())) {
//                updateSelection(selCandidate);
//            }
//        }
//    }
//}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////

void ViewInterface::onSpoilNeeded(const QModelIndex & node) {
    if (node.isValid()) {
        setCurrentIndex(node);
        scrollTo(node);
    }
}

void ViewInterface::updateSelection(QModelIndex & node) {
    if (node.isValid()) {
        if (!node.data(IPLAYABLE).toBool())
            findExecutable(node);

        onSpoilNeeded(node);
    }
}

void ViewInterface::openLocation() {
    ItemInterface * item = mdl -> item(currentIndex());
    item -> openLocation();
}

void ViewInterface::drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex & index) const { // TODO: rewrite
    //TODO: maybe use options for some manipulations with row


    // TODO: add initiated items to hash for update later on the same name state change
    ItemInterface * node = mdl -> item(index);

    if (!node -> is(ItemState::proceeded)) {
        node -> set(ItemState::proceeded);
//        if (!node -> isContainer()) {
//            Library::instance() -> initItem(item, model, SLOT(libraryResponse()));
//        }

        if (node -> is(ItemState::expanded))
            emit mdl -> expandNeeded(index);
    }

    QTreeView::drawRow(painter, options, index);
}

void ViewInterface::resizeEvent(QResizeEvent * event) { // TODO: rewrite // need separate item initializator for each view
//    if (event -> oldSize().height() != size().height()) {
//        if (event -> size().height() > 0) {
//            int count = (event -> size().height() / Settings::instance() -> getTotalItemHeight()) + 2;
//            Library::instance() -> setRemoteItemMax(count);
//        }
//    }

    QTreeView::resizeEvent(event);
}

void ViewInterface::contextMenuEvent(QContextMenuEvent * event) {
    event -> accept();

    QList<QAction *> actions;
    QAction * act;

    if (isEditable()) {
        actions.append((act = new QAction(QIcon(":/settings"), "View settings", this)));
        connect(act, SIGNAL(triggered(bool)), Dockbars::instance(), SLOT(editActiveBar()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    if (Player::instance() -> playedIndex().isValid()) {
        actions.append((act = new QAction(QIcon(":/active_tab"), "Show active elem", this)));
        connect(act, SIGNAL(triggered(bool)), Dockbars::instance(), SLOT(scrollToActive()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    QModelIndex ind = indexAt(event -> pos());

    if (ind.isValid()) {
        if (!ind.data(IFULLPATH).toString().isEmpty()) {
            actions.append((act = new QAction(QIcon(":/open"), "Open location", this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(openLocation()));
        }

        if (ind.data(IREMOTE).toBool()) {
            //    openAct = new QAction(QIcon(":/refresh"), "Refresh", this);
            //    connect(openAct, SIGNAL(triggered(bool)), model, SLOT(refresh()));
            //    actions.append(openAct);

            //    if (model -> getApi() != 0) {
            //        openAct = new QAction(QIcon(":/refresh"), "Friends (groups) audio", this);
            //        connect(openAct, SIGNAL(triggered(bool)), QApplication::activeWindow(), SLOT(showVKRelTabDialog()));
            //        actions.append(openAct);
            //    }

            //    if (QString(metaObject() -> className()) == QString("VkView")) {
            //        openAct = new QAction(QIcon(":/refresh"), "Parse/Refresh Wall", this);
            //        connect(openAct, SIGNAL(triggered(bool)), model, SLOT(refreshWall()));
            //        actions.append(openAct);
            //    }

//            sepAct = new QAction(this);
//            sepAct -> setSeparator(true);
//            actions.append(sepAct);

            actions.append((act = new QAction(QIcon(":/download"), "Download", this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(download()));

            actions.append((act = new QAction(QIcon(":/download"), "Download All", this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(downloadAll()));
        }
    }

    if (mdl -> rowCount() > 0) {
        actions.append((act = new QAction(QIcon(":/shuffle"), "Shuffle", this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(shuffle()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);

        actions.append((act = new QAction(QIcon(":/collapse"), "Collapse all", this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(collapseAll()));

        actions.append((act = new QAction(QIcon(":/expand"), "Expand all", this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(expandAll()));
    }

    if (actions.count() > 0)
        QMenu::exec(actions, event -> globalPos(), 0, this);
}

QModelIndex ViewInterface::candidateOnSelection(QModelIndex node) {
    while(true) {
        if (!node.isValid() || !node.data(IFOLDER).toBool())
            return node;

        if (mdl -> hasChildren(node))
            expand(node);

//        if (forwardOrder)
            node = indexBelow(node);
//        else
//            node = indexAbove(node);
    }

//    QModelIndex parentNode = node.parent();
//    if (!parentNode.isValid())
//        parentNode = rootIndex();
//    int row = node.row();

////    if (forwardOrder) // maybe use order
//    QModelIndex selCandidate = parentIndex.child(row + 1, 0);
}

void ViewInterface::findAndExecIndex(bool deleteCurrent) {
    QModelIndex node = activeIndex();
    if (!node.isValid()) return;

    if (deleteCurrent) {
        QModelIndex removeNode = node;
        findExecutable(node);
        removeRow(removeNode);
    }
    else findExecutable(node);

    execIndex(node);
}

bool ViewInterface::removeRow(QModelIndex & node, bool updateSelection) {


    //    if (isFolder && item -> childTreeCount() > 1) {
    //        if (QMessageBox::warning(
    //                    parentWidget(),
    //                    "Folder deletion",
    //                    "Are you shure what you want remove the not empty folder ?",
    //                    QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
    //            return;
    //    }


    if (updateSelection) {
        QModelIndex parentNode = node.parent();
        int row = node.row();

        bool res = mdl -> removeRow(node.row(), node.parent());
        setCurrentIndex(candidateOnSelection(mdl -> index(row, 0, parentNode)));

        return res;
    }
    else return mdl -> removeRow(node.row(), node.parent());
}

bool ViewInterface::removeRows(QModelIndexList & nodes, bool updateSelection) {
    if (nodes.isEmpty())
        return false;

    QModelIndex node, parentNode;
    int row;

    if (updateSelection) {
        node = nodes.first();
        parentNode = node.parent();
        row = node.row();
    }

    bool res = true;

    for(int i = nodes.count() - 1; i >= 0; i--) {
        node = nodes.at(i);
        res |= removeRow(node);
    }

    if (updateSelection)
        setCurrentIndex(candidateOnSelection(mdl -> index(row, 0, parentNode)));

    return res;
}

bool ViewInterface::prepareDownloading(QString /*path*/) { //TODO: move to separate class
//    QDir dir(path);
//    if (!dir.exists()) {
//        dir.mkpath(".");
//    }

    return true;
}

void ViewInterface::downloadItem(const QModelIndex & /*node*/, QString /*savePath*/) { //TODO: rewrite
//    QString prepared_path = savePath + item -> getDownloadTitle();
//    if (QFile::exists(prepared_path)) {
//        QFile::remove(prepared_path);
//    }

//    if (item -> isRemote()) {
//        if (model -> getApi() == 0) {
//            QMessageBox::warning(this, "Remote download", "Some shit happened :(");
//            return;
//        }

//        Download::instance() -> start(model, item, QUrl::fromLocalFile(prepared_path));
//    } else {
//        QFile f(item -> fullPath());
//        if (!f.copy(prepared_path))
//            QMessageBox::warning(this, "Bla", f.errorString());
//    }
}

void ViewInterface::downloadBranch(const QModelIndex & /*node*/, QString /*savePath*/) { //TODO: rewrite
//    prepareDownloading(savePath);
//    QList<ModelItem *> * children = rootNode -> childItemsList();
//    ModelItem * item;

//    for(int i = 0; i < children -> length(); i++) {
//        item = children -> at(i);

//        if (item -> isFolder()) {
//            downloadBranch(item, savePath + item -> getDownloadTitle() + "/");
//        } else {
//            downloadItem(item, savePath);
//        }
//    }
}

void ViewInterface::download() { //TODO: rewrite
//    downloadSelected(Settings::instance() -> getDownloadPath());
}

void ViewInterface::downloadAll() { //TODO: rewrite
//    QString savePath = Settings::instance() -> getDownloadPath();
//    if (!prepareDownloading(savePath)) return;
//    downloadBranch(model -> root(), savePath);
}

//void ViewInterface::modelUpdate() {
//    if (Player::instance() -> currentPlaylist() == this) {
//        if (Player::instance() -> playedItem())
//            Player::instance() -> playedItem() -> getState() -> unsetPlayed();
//        Player::instance() -> removePlaylist();
//    }
//}

void ViewInterface::downloadSelected(QString /*savePath*/, bool /*markAsLiked*/) { //TODO: rewrite
//    if (!prepareDownloading(savePath)) return;

//    ModelItem * item;

//    foreach(QModelIndex index, selectedIndexes()) {
//        item = model -> getItem(index);

//        if (item -> isFolder()) {
//            downloadBranch(item, savePath);
//        } else {
//            downloadItem(item, savePath);
//            if (markAsLiked)
//                item -> getState() -> setLiked();
//        }
//    }
}

void ViewInterface::setIconSize(const QSize & size) {
    QTreeView::setIconSize(size);
    item_delegate -> recalcAttrs(size.width());
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

QModelIndex ViewInterface::activeIndex() { //TODO: test
    QModelIndex ind = Player::instance() -> playedIndex();

    if (ind.model() != mdl)
        ind = QModelIndex();

    if (!ind.isValid()) {
        ind = selectionModel() -> currentIndex();

        if (!ind.isValid())
            ind = this -> rootIndex();
    }

    return ind;
}

void ViewInterface::findExecutable(QModelIndex & curr) {
    qDebug() << "-------------";
    qDebug() << curr.data();

    while(true) {
        if (mdl -> hasChildren(curr))
            expand(curr);

        if (forwardOrder)
            curr = indexBelow(curr);
        else
            curr = indexAbove(curr);

        qDebug() << curr.data();

        if (!curr.isValid() || curr.data(IPLAYABLE).toBool())
            return;
    }
}

void ViewInterface::dragEnterEvent(QDragEnterEvent * event) {
    QTreeView::dragEnterEvent(event);  // use this for highlighting

    if (event -> mimeData() -> hasFormat(DROP_OUTER_FORMAT) || event -> mimeData() -> hasFormat(DROP_INNER_FORMAT)) {
        event -> accept();
    } else event -> ignore();
}

void ViewInterface::dragMoveEvent(QDragMoveEvent * event) {
    QTreeView::dragMoveEvent(event); // use this for highlighting

    if (event -> mimeData() -> hasFormat(DROP_OUTER_FORMAT) || event -> mimeData() -> hasFormat(DROP_INNER_FORMAT)) {
        event -> accept();
        mdl -> setDropKeyboardModifiers(event -> keyboardModifiers());
    } else
        event -> ignore();
}

void ViewInterface::dropEvent(QDropEvent * event) {
    event -> setDropAction(
        event -> source() == this ? Qt::MoveAction : Qt::CopyAction
    );

    QTreeView::dropEvent(event);
}

void ViewInterface::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Enter || event -> key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        if (list.count() > 0)
            execIndex(list.first());

    } else if (event -> key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        removeRows(list, true);
    }
    else QTreeView::keyPressEvent(event);
}

void ViewInterface::mousePressEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton)
        dragPoint = event -> pos();

    QTreeView::mousePressEvent(event);
}

void ViewInterface::mouseMoveEvent(QMouseEvent * event) {
    if ((event -> buttons() == Qt::LeftButton) && (dragPoint - event -> pos()).manhattanLength() >= 10){
        if (selectedIndexes().length() > 0) {
            QDrag * drag = new QDrag(this);
            QMimeData * mimeData = mdl -> mimeData(selectedIndexes());
            drag -> setPixmap(QPixmap(":drag"));
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }

    QTreeView::mouseMoveEvent(event);
}
