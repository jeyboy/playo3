#include "view_interface.h"
#include "dockbars.h"

using namespace Playo3;

IView::IView(IModel * newModel, QWidget * parent, ViewSettings & settings)
    : QTreeView(parent), mdl(newModel), sttngs(settings), forwardOrder(true), blockRepaint(false) {

    setIndentation(Settings::instance() -> treeIndentation());
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
    int iconDimension = Settings::instance() -> iconHeight();
    setIconSize(QSize(iconDimension, iconDimension));

    connect(
        this, SIGNAL(showAlert(const QString &, const QString &, QMessageBox::StandardButtons)),
        UserDialogBox::instance(), SLOT(alert(const QString &, const QString &, QMessageBox::StandardButtons)),
        Qt::BlockingQueuedConnection
    );
    connect(this, SIGNAL(threadedRowRemoving(const QModelIndex &, int, bool)), this, SLOT(removeRow(const QModelIndex &, int, bool)), Qt::BlockingQueuedConnection);

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


//    setDragDropOverwriteMode(true); // TODO: need modifications in IModel::dropMimeData

    ////    setTreePosition(2);
    ////    setRootIndex();
    ////    setRowHidden(0, rootIndex(), true);

    ////    expandAll();
}

IView::~IView() {
    delete mdl;
}

void IView::scrollToActive() {
    if (Player::instance() -> playedIndex().isValid())
        scrollTo(Player::instance() -> playedIndex());
}

void IView::execPrevIndex(bool deleteCurrent) {
    forwardOrder = false;
    findAndExecIndex(deleteCurrent);
}

void IView::execNextIndex(bool deleteCurrent) {
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

bool IView::execPath(const QString path, bool paused, uint start) {
    QModelIndex ind = mdl -> fromPath(path);
    if (ind.isValid())
        return execIndex(ind, paused, start);
    else
        return false;
}

bool IView::execIndex(const QModelIndex & node, bool paused, uint start) {
    if (node.isValid() && node.data(IPLAYABLE).toBool()) {
        qDebug() << "PLAYED " << node.data();
        Dockbars::instance() -> setPlayed((DockBar *)parent());

        if (Settings::instance() -> isSpoilOnActivation())
            scrollTo(node);

        return Player::instance() -> playIndex(node, paused, start);
    }

    return false;
}

//QModelIndex IView::removeCandidate(QModelIndex item) {
//    ModelItem * parent = item -> parent();

//    while(parent -> childCount() == 1 && parent -> parent() != 0) {
//        item = parent;
//        parent = parent -> parent();
//    }

//    return item;
//}

//void IView::removeItem(QModelIndex & item) { //TODO: rewrite
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
void IView::onUpdateAttr(const QModelIndex ind, int attr, QVariant val) {
    mdl -> setData(ind, val, attr);
}

void IView::expandeAll() {
    mdl -> expandeAll();
    QTreeView::expandAll();
}
void IView::collapseAll() {
    mdl -> collapseAll();
    QTreeView::collapseAll();
}

void IView::onSpoilNeeded(const QModelIndex & node) {
    if (node.isValid()) {
        setCurrentIndex(node);
        scrollTo(node);
    }
}

void IView::updateSelection(QModelIndex & node) {
    if (node.isValid()) {
        if (!node.data(IPLAYABLE).toBool())
            findExecutable(node);

        onSpoilNeeded(node);
    }
}

void IView::openLocation() {
    IItem * item = mdl -> item(currentIndex());
    item -> openLocation();
}

void IView::drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex & index) const {
    IItem * node = mdl -> item(index);

    bool is_interactive = Settings::instance() -> isInteractiveProc();
    bool is_proceeded = node -> is(ItemState::proceeded);

    if (!is_proceeded) {
        node -> set(ItemState::proceeded);
        if (!node -> isContainer())
            Library::instance() -> restoreItemState(index);
    }

    if (is_interactive && is_proceeded)
        node -> unset(ItemState::proceeded);

    if (node -> is(ItemState::expanded)) // required for uncanonical delition and after loading state reconstruction
        emit mdl -> expandNeeded(index);

    QTreeView::drawRow(painter, options, index);
}

void IView::paintEvent(QPaintEvent * event) {
    if (!blockRepaint)
        QTreeView::paintEvent(event);
}

void IView::resizeEvent(QResizeEvent * event) {
    if (event -> oldSize().height() != size().height()) {
        if (event -> size().height() > 0) {
            int count = (event -> size().height() / Settings::instance() -> totalItemHeight()) + 2;
            Library::instance() -> setWaitListLimit(count);
        }
    }

    QTreeView::resizeEvent(event);
}

void IView::focusInEvent(QFocusEvent *) {
    Dockbars::instance() -> setActive((DockBar *)parent());
}

void IView::contextMenuEvent(QContextMenuEvent * event) {
    event -> accept();

    QList<QAction *> actions;
    QAction * act;

    if (isEditable()) {
        actions.append((act = new QAction(QIcon(":/settings"), "View settings", this)));
        connect(act, SIGNAL(triggered(bool)), Dockbars::instance(), SLOT(editActiveBar()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    actions.append((act = new QAction(QIcon(":/refresh"), "Refresh items", this)));
    connect(act, SIGNAL(triggered(bool)), mdl, SLOT(refresh()));

    actions.append((act = new QAction(this)));
    act -> setSeparator(true);

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
        }

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    if (mdl -> rowCount() > 0) {
        actions.append((act = new QAction(QIcon(":/download"), "Download", this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(downloadSelected()));

        actions.append((act = new QAction(QIcon(":/download"), "Download All", this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(downloadAll()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);

        actions.append((act = new QAction(QIcon(":/shuffle"), "Shuffle", this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(shuffle()));

        if (mdl -> containerType() != list) {
            actions.append((act = new QAction(this)));
            act -> setSeparator(true);

            actions.append((act = new QAction(QIcon(":/collapse"), "Collapse all", this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(collapseAll()));

            actions.append((act = new QAction(QIcon(":/expand"), "Expand all", this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(expandAll()));
        }
    }

    if (actions.count() > 0)
        QMenu::exec(actions, event -> globalPos(), 0, this);
}

QModelIndex IView::candidateOnSelection(QModelIndex node, bool reverseOrder) {
    while(true) {
        if (model() -> hasChildren(node))
            expand(node);

        node = (!reverseOrder) ? indexBelow(node) : indexAbove(node);

        if (!node.isValid() || !node.data(IFOLDER).toBool())
            return node;
    }
}

void IView::findAndExecIndex(bool deleteCurrent) {
    QModelIndex node = activeIndex();

    if (deleteCurrent && node.isValid()) {
        QString nodePath = node.data(ITREEPATH).toString();
        Player::instance() -> eject(false);
        removeRow(node);
        node = mdl -> fromPath(nodePath);
        findExecutable(node);
    }
    else findExecutable(node);

    execIndex(node);
}

bool IView::removeRow(const QModelIndex & node, int selectionUpdate, bool usePrevAction) {
    bool isFolder = false;

//    qDebug() << "REM: " << node.data() << " ||| " << node.data(ITREEPATH).toString();
    if (Settings::instance() -> isAlertOnFolderDeletion()) {
        if ((isFolder = node.data(IEXECCOUNTS) > 0)) {
            if (usePrevAction && _deleteFolderAnswer == QMessageBox::NoToAll)
                return false;

            if (!usePrevAction || (usePrevAction && _deleteFolderAnswer != QMessageBox::YesToAll)) {
                //INFO: need to send signal only in separate thread - Qt: Dead lock detected while activating a BlockingQueuedConnection
                if (QThread::currentThread() == QApplication::instance() -> thread()) {
                    UserDialogBox::instance() -> alert(
                        "Folder deletion",
                        "Are you sure what you want to remove the not empty folder '" + node.data().toString() + "' ?",
                        QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll
                    );
                } else {
                    emit showAlert(
                        "Folder deletion",
                        "Are you sure what you want to remove the not empty folder '" + node.data().toString() + "' ?",
                        QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll
                    );
                }

                _deleteFolderAnswer = UserDialogBox::instance() -> lastAnswer();
                if (_deleteFolderAnswer == QMessageBox::No || _deleteFolderAnswer == QMessageBox::NoToAll)
                    return false;
            }
        }
    }

    if (Player::instance() -> playedIndex().isValid()) {
        if (Player::instance() -> playedIndex().data(ITREESTR).toString().startsWith(
            node.data(ITREESTR).toString()
        ))
            Player::instance() -> eject();
    }

    if (isRemoveFileWithItem())
        mdl -> setData(node, ItemState::mark_on_removing, ISTATERESTORE);

    if (selectionUpdate != none) {
        QModelIndex newSel = QModelIndex();

        if (selectionUpdate & backward)
            newSel = candidateOnSelection(node, true);

        if (selectionUpdate & forward && !newSel.isValid()) { // this not worked for threaded deletion, because tree is not updated in process and contain a broken keys as result
            if (isFolder) {
                collapse(newSel);
                newSel = indexBelow(newSel);
            }
            newSel = candidateOnSelection(node);
        }

        setCurrentIndex(newSel);
    }

    return model() -> removeRow(node.row(), node.parent());
}

void IView::removeProccessing(QModelIndexList & index_list, bool inProcess) {
    int total = index_list.size(), temp = total;

    if (inProcess)
        emit mdl -> moveInProcess();

    _deleteFolderAnswer = QMessageBox::No;

    if (mdl -> containerType() == list) {
        qSort(index_list.begin(), index_list.end());
    } else {
        qSort(index_list.begin(), index_list.end(), modelIndexComparator());

        //INFO - remove dependent items
        mdl -> setProgress2(SPINNER_IS_CONTINIOUS);
        QMutableListIterator<QModelIndex> i(index_list);
        QString path = "--", elem_path;
        QModelIndex ind;

        while(i.hasNext()) {
            ind = i.next();
            elem_path = ind.data(ITREEPATH).toString();
            if (!elem_path.startsWith(path)) {
                if (ind.data(IFOLDER).toBool())
                    path = elem_path;
            }
            else i.remove();
        }

        mdl -> setProgress2(SPINNER_NOT_SHOW_SECOND);
        temp = total = index_list.size();
        //////
    }

    blockRepaint = true; // list type sometimes trying redraw yourself in process :(
    QModelIndexList::Iterator eit = --index_list.end();

    if (mdl -> containerType() == list || !inProcess) {
        for (; eit != index_list.begin(); --eit) {
            removeRow((*eit), none, true);

            if (inProcess)
                emit mdl -> setProgress(--temp * 100.0 / total);
        }
    } else {
        for (; eit != index_list.begin(); --eit) {
            if ((*eit).data(IFOLDER).toBool())
                emit threadedRowRemoving((*eit), none, true);
            else
                removeRow((*eit), none, true);
            emit mdl -> setProgress(--temp * 100.0 / total);
        }
    }
    blockRepaint = false;

    if (inProcess)
        // in thread item selected in view, but not added to selected model :(
        emit threadedRowRemoving((*eit), none, true); // if last elem is folder - throwned error if we in thread
    else
        removeRow((*eit), backward | forward, true);

    index_list.clear();
    if (inProcess)
        emit mdl -> moveOutProcess();
}

void IView::downloadItem(const QModelIndex & node, QString savePath) {
    DownloadView::instance() -> addRow(
        node.data(IURL).toUrl(),
        savePath,
        downloadTitle(node.data(ITITLE).toString(), node.data(IEXTENSION).toString())
    );
}

void IView::downloadBranch(const QModelIndex & node, QString savePath) {
    FolderItem * curr = mdl -> item<FolderItem>(node);
    IItem * item;

    for(int i = 0; i < curr -> childCount(); i++) {
        item = curr -> child(i);

        if (item -> isContainer())
            downloadBranch(mdl -> index(item), savePath);
        else
            downloadItem(mdl -> index(item), savePath);
    }
}

void IView::downloadAll() {
    downloadBranch(QModelIndex(), Settings::instance() -> defaultDownloadPath());
}

void IView::downloadSelected() {
    QString path = Settings::instance() -> defaultDownloadPath();

    foreach(QModelIndex index, selectedIndexes()) {
        if (index.data(IFOLDER).toBool())
            downloadBranch(index, path);
        else
            downloadItem(index, path);
    }
}

void IView::setIconSize(const QSize & size) {
    setIndentation(Settings::instance() -> treeIndentation());
    QTreeView::setIconSize(size);
    item_delegate -> recalcAttrs(size.width());
}

void IView::appendRows(QList<QUrl> & urls) {
    mdl -> threadlyInsertRows(urls, -1);
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

QModelIndex IView::activeIndex() {
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

void IView::findExecutable(QModelIndex & curr) {
    if (!curr.isValid())
        curr = mdl -> index(0, 0);

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

void IView::dragEnterEvent(QDragEnterEvent * event) {
    QTreeView::dragEnterEvent(event);
}

void IView::dragMoveEvent(QDragMoveEvent * event) {
    mdl -> setDropKeyboardModifiers(event -> keyboardModifiers());
    QTreeView::dragMoveEvent(event);
}

void IView::dropEvent(QDropEvent * event) {
    QTreeView::dropEvent(event);
    event -> accept();
}

void IView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Enter || event -> key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        if (!list.isEmpty())
            execIndex(list.first());

    } else if (event -> key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        selectionModel() -> clearSelection();

        if (!list.isEmpty())
            Library::instance() -> declineAllItemsRestoration();
        else return;

        if (list.size() > 200)
            QtConcurrent::run(this, &IView::removeProccessing, list, true);
        else if (list.size() > 1)
            removeProccessing(list);
        else {
            QModelIndex ind = currentIndex();
            if (currentIndex().isValid())
                removeRow(ind, true, false);
        }
    }
    else QTreeView::keyPressEvent(event);
}

void IView::mousePressEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton)
        dragPoint = event -> pos();

    QTreeView::mousePressEvent(event);
}

void IView::mouseMoveEvent(QMouseEvent * event) {
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
