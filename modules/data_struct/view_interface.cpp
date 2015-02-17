#include "view_interface.h"
#include "dockbars.h"

using namespace Playo3;

ViewInterface::ViewInterface(ModelInterface * newModel, QWidget * parent, ViewSettings & settings)
    : QTreeView(parent), mdl(newModel), sttngs(settings), forwardOrder(true)  {

    setIndentation(12);
    setStyle(new TreeViewStyle);
//    setStyleSheet(Stylesheets::treeViewStyles());

    setModel(mdl);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::CopyAction);

    setExpandsOnDoubleClick(true);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection); // ContiguousSelection

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

    connect(mdl, SIGNAL(moveInProcess()), parent, SLOT(onMoveInProcess()));
    connect(mdl, SIGNAL(moveOutProcess()), parent, SLOT(onMoveOutProcess()));
    connect(mdl, SIGNAL(setProgress(int)), parent, SLOT(onSetProgress(int)));
    connect(mdl, SIGNAL(setProgress2(int)), parent, SLOT(onSetProgress2(int)));

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


//    setDragDropOverwriteMode(true); // TODO: need modifications in ModelInterface::dropMimeData

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

bool ViewInterface::execPath(const QString path, bool paused, uint start) {
    return execIndex(mdl -> fromPath(path), paused, start);
}

bool ViewInterface::execIndex(const QModelIndex & node, bool paused, uint start) {
    qDebug() << "PLAYED " << node.data();
    Dockbars::instance() -> setPlayed((DockBar *)parent());

    if (node.isValid() && node.data(IPLAYABLE).toBool()) {
        if (Settings::instance() -> isSpoilOnActivation())
            scrollTo(node);

        Player::instance() -> playIndex(node, paused, start);
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

void ViewInterface::expandeAll() {
    mdl -> expandeAll();
    QTreeView::expandAll();
}
void ViewInterface::collapseAll() {
    mdl -> collapseAll();
    QTreeView::collapseAll();
}

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

void ViewInterface::drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex & index) const {
    // TODO: add initiated items to hash for update later on the same name state change
    ItemInterface * node = mdl -> item(index);

    if (!node -> is(ItemState::proceeded)) {
        node -> set(ItemState::proceeded);
//        if (!node -> isContainer()) {
//            Library::instance() -> initItem(item, model, SLOT(libraryResponse()));
//        }
    }

    if (node -> is(ItemState::expanded)) // required for uncanonical delition and after loading state reconstruction
        emit mdl -> expandNeeded(index);

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

void ViewInterface::focusInEvent(QFocusEvent *) {
    Dockbars::instance() -> setActive((DockBar *)parent());
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

QModelIndex ViewInterface::candidateOnSelection(QModelIndex node, bool reverseOrder) {
    while(true) {
        if (model() -> hasChildren(node))
            expand(node);

        node = (!reverseOrder) ? indexBelow(node) : indexAbove(node);

        if (!node.isValid() || !node.data(IFOLDER).toBool())
            return node;
    }
}

void ViewInterface::findAndExecIndex(bool deleteCurrent) {
    QModelIndex node = activeIndex();

    if (deleteCurrent && node.isValid()) {
        QModelIndex removeNode = node;
        findExecutable(node);
        removeRow(removeNode);
    }
    else findExecutable(node);

    execIndex(node);
}

bool ViewInterface::removeRow(QModelIndex & node, bool updateSelection, bool usePrevAction) {
    bool isFolder = false;

    qDebug() << "REM: " << node.data();
    if (Settings::instance() -> isAlertOnFolderDeletion()) {
        if ((isFolder = node.data(IEXECCOUNTS) > 0)) {
            if (usePrevAction && Settings::instance() -> folderDeletionAnswer() == QMessageBox::NoToAll)
                return false;

            if (!usePrevAction || (usePrevAction && Settings::instance() -> folderDeletionAnswer() != QMessageBox::YesToAll)) {
                int dialogRes = QMessageBox::warning(
                                    parentWidget(),
                                    "Folder deletion",
                                    "Are you sure what you want to remove the not empty folder '" + node.data().toString() + "' ?",
                                    QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll
                                );

                Settings::instance() -> setfolderDeletionAnswer(dialogRes);

                if (dialogRes == QMessageBox::No || dialogRes == QMessageBox::NoToAll)
                    return false;
            }
        }
    }

    if (!isFolder && node.data(ISTATE).toInt() & ItemState::played)
        Player::instance() -> playIndex(QModelIndex());


    if (updateSelection) {
        QModelIndex newSel = candidateOnSelection(node);
        if (!newSel.isValid())
            newSel = candidateOnSelection(node, true);

        setCurrentIndex(newSel);

        return model() -> removeRow(node.row(), node.parent());



//        QModelIndex parentNode = node.parent();
//        int row = node.row();

//        bool res = mdl -> removeRow(row, parentNode);
//        if (needReset)
//            reset();

//        node = mdl -> index(row, 0, parentNode, true);
//        if (!node.isValid()) node = parentNode;
//        qDebug() << "!!v " << node.data();
//        node = candidateOnSelection(node);
//        setCurrentIndex(node);
    }
    else return model() -> removeRow(node.row(), node.parent());
}

void ViewInterface::removeProccessing(bool inProcess) {
    int total = selectedIndexes().size(), temp;

    if (inProcess)
        emit mdl -> moveInProcess();

    Settings::instance() -> setfolderDeletionAnswer(QMessageBox::No);

    if (inProcess)
        emit mdl -> setProgress(SPINNER_IS_CONTINIOUS);

    QModelIndexList l = selectedIndexes();
    qSort(l.begin(), l.end(), modelIndexComparator);









//    if (total < 200 || mdl -> containerType() != list) { // this version is very slow but is more canonical
//        bool loopReason = true;

//        while(loopReason && !selectedIndexes().isEmpty()) { // this did not work in not main thread :(
//            temp = selectedIndexes().size();
//            loopReason = !(temp == 1);
//            removeRow(selectedIndexes().last(), !loopReason, true);
//            if (inProcess)
//                emit mdl -> setProgress(temp * 100.0 / total);
//        }
//    } else {
////        use
////        protected Q_SLOTS:
////            void rowsRemoved(const QModelIndex &parent, int first, int last);

////        for type list





//        QModelIndex ind;
//        QModelIndexList l = selectedIndexes();

//        qSort(l.begin(), l.end());

//        model() -> blockSignals(true);
//        blockSignals(true);

//        while((temp = l.size()) > 1) {
//            ind = l.takeLast();
//            removeRow(ind, false, true);
//            if (inProcess) {
//                model() -> blockSignals(false);
//                emit mdl -> setProgress(temp * 100.0 / total);
//                model() -> blockSignals(true);
//            }
//        }

//        blockSignals(false);
//        model() -> blockSignals(false);

//        reset();

//        if(l.size() > 0) {
//            ind = l.takeLast();
//            removeRow(ind, true, true);
//        }
//    }

    if (inProcess)
        emit mdl -> moveOutProcess();
}

//bool ViewInterface::removeRows(QModelIndexList & nodes, bool updateSelection) {
//    if (nodes.isEmpty())
//        return false;

//    QModelIndex node, parentNode;
//    int row = 0;

//    if (updateSelection) {
//        node = nodes.first();
//        parentNode = node.parent();
//        row = node.row();
//    }

//    bool res = true;
//    Settings::instance() -> setfolderDeletionAnswer(QMessageBox::No);

//    foreach(QModelIndex node, nodes)
//        res |= removeRow(node, false, true);

//    if (updateSelection)
//        setCurrentIndex(candidateOnSelection(mdl -> index(row, 0, parentNode)));

//    return res;
//}

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

    if (ind.model() != model())
        ind = QModelIndex();

    if (!ind.isValid()) {
        ind = currentIndex();

        if (!ind.isValid())
            ind = rootIndex();
    }

    return ind;
}

void ViewInterface::findExecutable(QModelIndex & curr) {
    if (!curr.isValid()) {
        curr = mdl -> index(0, 0);
        if (curr.data(IPLAYABLE).toBool())
            return;
    }

    if (Player::instance() -> playedIndex() != curr && curr.data(IPLAYABLE).toBool())
        return;

    if (forwardOrder) {
        while(true) {
            if (model() -> hasChildren(curr)) // maybe try to expand all items
                expand(curr);

            curr = indexBelow(curr);

            if (!curr.isValid() || curr.data(IPLAYABLE).toBool())
                return;
        }
    } else {
        QModelIndex temp = curr;

        while(true) {
            if (model() -> hasChildren(curr) && temp.parent() != curr) {
                expand(curr);
                curr = temp;
            }
            else temp = curr;

            curr = indexAbove(curr);

            if (!curr.isValid() || curr.data(IPLAYABLE).toBool())
                return;
        }
    }
}

void ViewInterface::dragEnterEvent(QDragEnterEvent * event) {
    QTreeView::dragEnterEvent(event);
}

void ViewInterface::dragMoveEvent(QDragMoveEvent * event) {
    mdl -> setDropKeyboardModifiers(event -> keyboardModifiers());
    QTreeView::dragMoveEvent(event);
}

void ViewInterface::dropEvent(QDropEvent * event) {
    QTreeView::dropEvent(event);
    event -> accept();
}

void ViewInterface::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Enter || event -> key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        if (!list.isEmpty())
            execIndex(list.first());

    } else if (event -> key() == Qt::Key_Delete) {
        //TODO: removing did nor work correctly in separate thread :(
//        if (selectedIndexes().size() > 10000 && mdl -> containerType() == list)
//            QtConcurrent::run(this, &ViewInterface::removeProccessing, true);
//        else
            removeProccessing();
    }

    QTreeView::keyPressEvent(event);
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
            QMimeData * mimeData = model() -> mimeData(selectedIndexes());
            drag -> setPixmap(QPixmap(":drag"));
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }
    else
        QTreeView::mouseMoveEvent(event); // if we add this to common loop - after dnd we lost focus on widget
}
