#include "view_interface.h"
#include "dockbars.h"

using namespace Playo3;

ViewInterface::ViewInterface(ModelInterface * newModel, QWidget * parent, ViewSettings & settings)
    : QTreeView(parent), mdl(newModel), sttngs(settings)  {

    setIndentation(12);
//    setStyle(new TreeViewStyle);
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

//    setItemDelegate(new ItemDelegate(this));

    setContextMenuPolicy(Qt::DefaultContextMenu);
    int iconDimension = Settings::instance() -> getIconHeight();
    setIconSize(QSize(iconDimension, iconDimension));

//    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClick(const QModelIndex &))); // move logic to the void mouseDoubleClickEvent(QMouseEvent *)
    connect(this, SIGNAL(expanded(const QModelIndex &)), mdl, SLOT(expanded(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), mdl, SLOT(collapsed(const QModelIndex &)));

    connect(mdl, SIGNAL(expandNeeded(const QModelIndex &)), this, SLOT(expand(const QModelIndex &)));
    connect(mdl, SIGNAL(spoilNeeded(QModelIndex &)), this, SLOT(updateSelection(QModelIndex &)));

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

QJsonObject ViewInterface::toJson() {
    QJsonObject res = mdl -> toJson();
//    res["p"] = QString();
//    res["l"] = mdl -> itemsCount();
    return res;
}

void ViewInterface::scrollToActive() {
//    if (Player::instance() -> playedItem()) {
//        scrollTo(model -> index(Player::instance() -> playedItem()));
//    }
}

void ViewInterface::prevItem(bool deleteCurrent) {
//    ModelItem * item = activeItem(false);
//    if (item == 0) return;

//    item = prevItem(item);
//    execIndex(item);
}

void ViewInterface::nextItem(bool deleteCurrent) {
//    ModelItem * item = activeItem();
//    if (item == 0) return;

    //    item = nextItem(item);

    //    if (Player::instance() -> currentPlaylist() == this) {
    //        if (Player::instance() -> playedItem()) {
    //            removeItem(Player::instance() -> playedItem());
    //        }
    //    }
//    execIndex(item);
}

QModelIndex ViewInterface::fromPath(QString path) { //TODO: rewrite
//    QStringList parts = path.split(' ', QString::SkipEmptyParts);
//    ModelItem * curr = getModel() -> root();
//    int level;

//    while(parts.length() > 0) {
//        level = parts.takeFirst().toInt();
//        curr = curr -> child(level);
//    }

//    return curr;
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

bool ViewInterface::execIndex(const QModelIndex & item) { //TODO: rewrite
//    ItemInterface * item = mdl -> getItem(index);

//    if (!item -> isContainer())

//    if (item) {
//        if (Settings::instance() -> isSpoilOnActivation())
//            scrollTo(model -> index(item));
//        if (item -> isExist()) {
//            Player::instance() -> playItem(this, item, paused);
//            return true;
//        } else {
//            item -> getState() -> setNotExist();
//            model -> refreshItem(item);
//        }
//    } else return true;

//    return false;
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

//void View::showMessage(QString text) {
//    QMessageBox::warning(this, "Bla bla bla", text);
//}

void ViewInterface::updateSelection(QModelIndex & candidate) {
    if (candidate.isValid()) {
        if (candidate.data(FOLDERID).toBool())
            toNextItem(candidate);

        if (candidate.isValid()) {
            setCurrentIndex(candidate);
            scrollTo(candidate);
        }
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

void ViewInterface::resizeEvent(QResizeEvent * event) { // TODO: rewrite
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
    }

    act = new QAction(this);
    act -> setSeparator(true);
    actions.append(act);

//    if (Player::instance() -> playedItem()) {
//        act = new QAction(QIcon(":/active_tab"), "Show active elem", this);
//        connect(act, SIGNAL(triggered(bool)), QApplication::activeWindow(), SLOT(showActiveElem()));
//        actions.append(act);

//        act = new QAction(this);
//        act -> setSeparator(true);
//        actions.append(act);
//    }

    QModelIndex ind = indexAt(event -> pos());

    if (ind.isValid()) {
        qDebug() << ind.data(FULLPATHID).toString();
        if (!ind.data(FULLPATHID).toString().isEmpty()) {
            act = new QAction(QIcon(":/open"), "Open location", this);
            connect(act, SIGNAL(triggered(bool)), this, SLOT(openLocation()));
            actions.append(act);
        }

        if (ind.data(REMOTEID).toBool()) {
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

//            openAct = new QAction(QIcon(":/download"), "Download", this);
//            connect(openAct, SIGNAL(triggered(bool)), this, SLOT(download()));
//            actions.append(openAct);
//            openAct = new QAction(QIcon(":/download"), "Download All", this);
//            connect(openAct, SIGNAL(triggered(bool)), this, SLOT(downloadAll()));
//            actions.append(openAct);
        }
    }

    act = new QAction(QIcon(":/shuffle"), "Shuffle", this);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(shuffle()));
    actions.append(act);

    act = new QAction(this);
    act -> setSeparator(true);
    actions.append(act);

    if (mdl -> rowCount() > 0) {
        act = new QAction(QIcon(":/collapse"), "Collapse all", this);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(collapseAll()));
        actions.append(act);

        act = new QAction(QIcon(":/expand"), "Expand all", this);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(expandAll()));
        actions.append(act);
    }

    if (actions.count() > 0)
        QMenu::exec(actions, event -> globalPos(), 0, this);
}

bool ViewInterface::prepareDownloading(QString path) { //TODO: move to separate class
//    QDir dir(path);
//    if (!dir.exists()) {
//        dir.mkpath(".");
//    }

//    return true;
}

void ViewInterface::downloadItem(const QModelIndex & node, QString savePath) { //TODO: rewrite
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

void ViewInterface::downloadBranch(const QModelIndex & node, QString savePath) { //TODO: rewrite
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

void ViewInterface::downloadSelected(QString savePath, bool markAsLiked) { //TODO: rewrite
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

//TODO: add copy func
void ViewInterface::copyItemsFrom(ViewInterface * /*otherView*/) {
//    QModelIndexList indexes = otherView -> selectedItems();

//    foreach(QModelIndex index, indexes) {
//        //TODO: copy logic
//    }
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

QModelIndex ViewInterface::activeItem(bool next) { //TODO: rewrite
//    ModelItem * item = 0;

//    if (Player::instance() -> currentPlaylist() == this) {
//        if (Player::instance() -> playedItem()) {
//            item = Player::instance() -> playedItem();
//        }
//    }

//    if (item == 0) {
//        QModelIndexList list = selectedIndexes();

//        if (list.count() > 0) {
//            item = model -> getItem(list.first());

//            if (!item -> isFolder()) {
//                QModelIndex m;
//                if (next) {
//                    m = this -> indexAbove(list.first());
//                } else { m = this -> indexBelow(list.first()); }

//                if (m.isValid()) {
//                   item = model -> getItem(m);
//                } else {
//                   item = model -> getItem(list.first().parent());
//                }
//            }
//        } else {
//            item = model -> getItem(this -> rootIndex());
//        }
//    }

//    return item;
}

void ViewInterface::toNextItem(QModelIndex & curr) { //TODO: rewrite
//    QModelIndex index = view->selectionModel()->currentIndex();
//    QAbstractItemModel *model = view->model();
//    if (model -> hasChildren(index))
//        view->expand(index);

//    index = view -> indexBelow(index);
//    if (index.isValid()) {
//        view -> setCurrentIndex(index);
//    } else
//        statusBar()->showMessage(tr("Next not found"));


//    ModelItem * item = curr;
//    bool first_elem = curr -> parent() == 0 || curr -> isFolder();

//    while(true) {
//        if (first_elem) {
//            first_elem = false;
//        } else {
//            item = item -> parent() -> child(item -> row() + 1);
//        }

//        if (item != 0) {
//            if (item -> isPlayable()) {
//                return item;
//            } else {
//                curr = item;
//                item = curr -> child(0);
//                first_elem = true;
//            }
//        } else {
//            if (curr -> parent() == 0)
//                return 0;

//            item = curr;
//            curr = curr -> parent();
//        }
//    }
}
void ViewInterface::toPrevItem(QModelIndex & curr) { //TODO: rewrite
//    QModelIndex index = view->selectionModel()->currentIndex();
//    QAbstractItemModel *model = view->model();
//    if (model -> hasChildren(index))
//        view->expand(index);

//    index = view -> indexAbove(index);
//    if (index.isValid()) {
//        view -> setCurrentIndex(index);
//    } else
//        statusBar()->showMessage(tr("Prev not found"));


//    ModelItem * item = curr;
//    bool last_elem = false;

//    if (curr -> parent() == 0)
//        return 0;

//    while(true) {
//        if (last_elem) {
//            last_elem = false;
//        } else {
//            item = item -> parent() -> child(item -> row() - 1);
//        }

//        if (item != 0) {
//            if (item -> isPlayable()) {
//                return item;
//            } else {
//                curr = item;
//                item = curr -> child(item -> childCount() - 1);
//                last_elem = true;
//            }
//        } else {
//            if (curr -> parent() == 0)
//                return 0;

//            item = curr;
//            curr = curr -> parent();
//        }
//    }
}

void ViewInterface::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasFormat(DROP_OUTER_FORMAT) || event -> mimeData() -> hasFormat(DROP_INNER_FORMAT)) {
        event -> accept();
    } else event -> ignore();

//    QTreeView::dragEnterEvent(event);
}

void ViewInterface::dragMoveEvent(QDragMoveEvent * event) {
    if (event -> mimeData() -> hasFormat(DROP_OUTER_FORMAT) || event -> mimeData() -> hasFormat(DROP_INNER_FORMAT)) {
        event -> accept();
        mdl -> setDropKeyboardModifiers(event -> keyboardModifiers());
    } else
        event -> ignore();

//    QTreeView::dragMoveEvent(event);
}

void ViewInterface::dropEvent(QDropEvent *event) {
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
        QModelIndex modelIndex;

        for(int i = list.count() - 1; i >= 0; i--) {
            modelIndex = list.at(i);
            mdl -> removeRow(modelIndex.row(), modelIndex.parent());
        }
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
