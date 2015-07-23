#include "view_interface.h"
#include "dockbars.h"

using namespace Playo3;

IView::IView(IModel * newModel, QWidget * parent, ViewSettings & settings)
    : QTreeView(parent), mdl(newModel), sttngs(settings), direction(IModel::forward), blockRepaint(false) {

    setIndentation(Settings::instance() -> treeIndentation());
    setStyle(new TreeViewStyle);
//    setStyleSheet(Stylesheets::treeViewStyles());

    Library::instance() -> registerListSync(mdl, mdl -> syncMutex());

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

    setUniformRowHeights(Settings::instance() -> isHeightUnificate());
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
    connect(this, SIGNAL(threadedRowRemoving(const QModelIndex &, bool, int, bool)), this, SLOT(removeRow(const QModelIndex &, bool, int, bool)), Qt::BlockingQueuedConnection);

    connect(this, SIGNAL(activated(const QModelIndex &)), this, SLOT(onDoubleClick(const QModelIndex &)));
    connect(this, SIGNAL(expanded(const QModelIndex &)), mdl, SLOT(expanded(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), mdl, SLOT(collapsed(const QModelIndex &)));

    connect(mdl, SIGNAL(expandNeeded(const QModelIndex &)), this, SLOT(expand(const QModelIndex &)));
    connect(mdl, SIGNAL(spoilNeeded(const QModelIndex &)), this, SLOT(onSpoilNeeded(const QModelIndex &)));

    connect(mdl, SIGNAL(moveInBackgroundProcess()), parent, SLOT(onMoveInBackgroundProcess()));
    connect(mdl, SIGNAL(moveOutBackgroundProcess()), parent, SLOT(onMoveOutBackgroundProcess()));
    connect(mdl, SIGNAL(setBackgroundProgress(int)), parent, SLOT(onSetBackgroundProgress(int)));

    connect(mdl, SIGNAL(moveInProcess()), parent, SLOT(onMoveInProcess()));
    connect(mdl, SIGNAL(moveOutProcess()), parent, SLOT(onMoveOutProcess()));
    connect(mdl, SIGNAL(setProgress(int)), parent, SLOT(onSetProgress(int)));
    connect(mdl, SIGNAL(setProgress2(int)), parent, SLOT(onSetProgress2(int)));

//    connect(model, SIGNAL(itemsCountChanged(int)), parent, SLOT(updateHeader(int)));
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
    while (!mdl -> syncMutex() -> tryLock())
        QApplication::processEvents();

    Library::instance() -> unregisterListSync(model());
    mdl -> syncMutex() -> unlock();

    delete mdl;
}

void IView::scrollToActive() {
    if (Player::instance() -> playedIndex().isValid())
        scrollTo(Player::instance() -> playedIndex(), QAbstractItemView::PositionAtCenter);
}

void IView::execPrevIndex(bool deleteCurrent) {
    direction = IModel::backward;
    findAndExecIndex(deleteCurrent);
}

void IView::execNextIndex(bool deleteCurrent) {
    direction = IModel::forward;
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
    if (ind.isValid()) {
        setFocus();
        return execIndex(ind, paused, start);
    } else
        return false;
}

bool IView::execIndex(const QModelIndex & node, bool paused, uint start) {
    if (node.isValid() && node.data(IPLAYABLE).toBool()) {
        qDebug() << "PLAYED " << node.data();
        Dockbars::instance() -> setPlayed((DockBar *)parent());

        if (Settings::instance() -> isSpoilOnActivation())
            scrollTo(node, (Settings::instance() -> isHeightUnificate() ? QAbstractItemView::EnsureVisible : QAbstractItemView::PositionAtCenter));

        if (Player::instance() -> playedIndex() == node) {
            Player::instance() -> playPause();
            return true;
        } else
            return Player::instance() -> playIndex(node, paused, start);
    }

    return false;
}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////
void IView::startInnerSearch(QString predicate, QModelIndex ind) {
    bool empty = predicate.isEmpty();

    QModelIndex it;

    for(int row = 0; ; row++) {
        it = mdl -> index(row, 0, ind);
        if (it.isValid()) {
            if (it.data(IFOLDER).toBool())
                startInnerSearch(predicate, it);
            else
                setRowHidden(row, ind, !(empty || it.data().toString().contains(predicate, Qt::CaseInsensitive)));
        } else break;
    }

    if (ind == QModelIndex())
        emit searchFinished();
}

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
        scrollTo(node, QAbstractItemView::PositionAtCenter);
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

void IView::copyToClipboard() {
    QModelIndex ind = currentIndex();
    mdl -> copyTitleToClipboard(ind);
}

void IView::importIds() {
    ExportDialog d(this);
    if (d.exec() == QDialog::Accepted)
        mdl -> importIds(this, d.getUids());
}

void IView::copyIdsToClipboard() {
    QModelIndexList indexes = selectedIndexes();
    mdl -> copyIdsToClipboard(indexes);
}

void IView::openRecomendationsforUser() {
    ViewSettings settings(vk_rel, false, false, false, true, sttngs.uid, user_rel);
    Dockbars::instance() -> createDocBar(QStringLiteral("Rec for user ") % sttngs.uid, settings, 0, true, true);
}
void IView::openRecomendationsforItemUser() {
    WebItem * it = mdl -> item<WebItem>(currentIndex());
    if (it -> owner().isValid()) {
        ViewSettings settings(vk_rel, false, false, false, true, it -> owner().toString(), user_rel);
        Dockbars::instance() -> createDocBar(QStringLiteral("Rec for user ") % it -> owner().toString(), settings, 0, true, true);
    }
}
void IView::openRecomendationsforItem() {
    WebItem * it = mdl -> item<WebItem>(currentIndex());
    if (it -> uid().isValid()) {
        ViewSettings settings(vk_rel, false, false, false, true, it -> toUid().toString(), song_rel);
        Dockbars::instance() -> createDocBar(QStringLiteral("Rec for song ") % it -> title().toString(), settings, 0, true, true);
    }
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
            Library::instance() -> setWaitListLimit(mdl, count);
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
        actions.append((act = new QAction(QIcon(QStringLiteral(":/settings")), QStringLiteral("View settings"), this)));
        connect(act, SIGNAL(triggered(bool)), Dockbars::instance(), SLOT(editActiveBar()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    actions.append((act = new QAction(QIcon(QStringLiteral(":/refresh")), QStringLiteral("Refresh items"), this)));
    connect(act, SIGNAL(triggered(bool)), mdl, SLOT(refresh()));

    actions.append((act = new QAction(this)));
    act -> setSeparator(true);

    actions.append((act = new QAction(QIcon(QStringLiteral(":/search")), QStringLiteral("Find"), this)));
    act -> setShortcut(QKeySequence(tr("Ctrl+F", "Find items")));
    connect(act, SIGNAL(triggered(bool)), parent(), SLOT(showSearch()));

    actions.append((act = new QAction(this)));
    act -> setSeparator(true);

    if (Player::instance() -> playedIndex().isValid()) {
        actions.append((act = new QAction(QIcon(QStringLiteral(":/active_tab")), QStringLiteral("Show active elem"), this)));
//        act -> setShortcut(QKeySequence(tr("Ctrl+P", "Played elem")));
        connect(act, SIGNAL(triggered(bool)), Dockbars::instance(), SLOT(scrollToActive()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    if (mdl -> containerType() == vk) {
        actions.append((act = new QAction(QIcon(/*":/active_tab"*/), QStringLiteral("Recommendations for user"), this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(openRecomendationsforUser()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    QModelIndex ind = indexAt(event -> pos());

    if (ind.isValid()) {
        if (ind.data(ITYPE).toInt() == VK_ITEM) {
            actions.append((act = new QAction(QIcon(/*":/active_tab"*/), QStringLiteral("Recommendations for item user"), this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(openRecomendationsforItemUser()));
            actions.append((act = new QAction(QIcon(/*":/active_tab"*/), QStringLiteral("Recommendations for item"), this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(openRecomendationsforItem()));
            actions.append((act = new QAction(this)));
            act -> setSeparator(true);
        }

        actions.append((act = new QAction(QIcon(QStringLiteral(":/copy")), QStringLiteral("Copy name to clipboard"), this)));
        act -> setShortcut(QKeySequence(tr("Ctrl+C", "Copy")));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(copyToClipboard()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);

        if (!ind.data(IFULLPATH).toString().isEmpty()) {
            actions.append((act = new QAction(QIcon(QStringLiteral(":/open")), QStringLiteral("Open location"), this)));
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

//            if (mdl -> containerType() == vk) {
//                actions.append((act = new QAction(QIcon(":/refresh"), "Parse/Refresh Wall", this)));
//                connect(act, SIGNAL(triggered(bool)), mdl, SLOT(refreshWall()));
//            }

//            sepAct = new QAction(this);
//            sepAct -> setSeparator(true);
//            actions.append(sepAct);
        }

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);
    }

    actions.append((act = new QAction(QIcon(QStringLiteral(":/import")), QStringLiteral("Import Ids"), this)));
    act -> setShortcut(QKeySequence(tr("Ctrl+Shift+V", "Insert ids")));
    connect(act, SIGNAL(triggered(bool)), this, SLOT(importIds()));

    if (mdl -> rowCount() > 0) {
        if (!selectedIndexes().isEmpty()) {
            actions.append((act = new QAction(QIcon(QStringLiteral(":/export")), QStringLiteral("Copy Ids to Clipboard"), this)));
            act -> setShortcut(QKeySequence(tr("Ctrl+Shift+C", "Copy ids")));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(copyIdsToClipboard()));

            actions.append((act = new QAction(this)));
            act -> setSeparator(true);
        }       


        if (Settings::instance() -> isCheckboxShow()) {
            actions.append((act = new QAction(this)));
            act -> setSeparator(true);

//            actions.append((act = new QAction(QIcon(":/move"), "Copy Checked To New Tab", this)));
//            connect(act, SIGNAL(triggered(bool)), this, SLOT(moveCheckedToNewTab()));

            actions.append((act = new QAction(QIcon(QStringLiteral(":/move")), QStringLiteral("Check All"), this)));
            connect(act, SIGNAL(triggered(bool)), mdl, SLOT(markAllAsChecked()));

            actions.append((act = new QAction(QIcon(QStringLiteral(":/move")), QStringLiteral("Uncheck All"), this)));
            connect(act, SIGNAL(triggered(bool)), mdl, SLOT(markAllAsUnchecked()));


            actions.append((act = new QAction(QIcon(QStringLiteral(":/move")), QStringLiteral("Check Liked"), this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(markLikedAsChecked()));

            actions.append((act = new QAction(QIcon(QStringLiteral(":/move")), QStringLiteral("Check New"), this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(markNewAsChecked()));

            actions.append((act = new QAction(QIcon(QStringLiteral(":/move")), QStringLiteral("Check Listened"), this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(markListenedAsChecked()));

            actions.append((act = new QAction(this)));
            act -> setSeparator(true);
        }


        actions.append((act = new QAction(QIcon(QStringLiteral(":/download")), QStringLiteral("Download"), this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(downloadSelected()));

        actions.append((act = new QAction(QIcon(QStringLiteral(":/download")), QStringLiteral("Download All"), this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(downloadAll()));

        actions.append((act = new QAction(this)));
        act -> setSeparator(true);

        actions.append((act = new QAction(QIcon(QStringLiteral(":/shuffle")), QStringLiteral("Shuffle"), this)));
        connect(act, SIGNAL(triggered(bool)), this, SLOT(shuffle()));

        if (mdl -> containerType() != list) {
            actions.append((act = new QAction(this)));
            act -> setSeparator(true);

            actions.append((act = new QAction(QIcon(QStringLiteral(":/collapse")), QStringLiteral("Collapse all"), this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(collapseAll()));

            actions.append((act = new QAction(QIcon(QStringLiteral(":/expand")), QStringLiteral("Expand all"), this)));
            connect(act, SIGNAL(triggered(bool)), this, SLOT(expandAll()));
        }
    }

    if (actions.count() > 0)
        QMenu::exec(actions, event -> globalPos(), 0, this);
}

void IView::checkByPredicate(IItem::ItemStateFlag flag) {
    emit mdl -> moveInProcess();
    QApplication::processEvents();

    bool ensureVisibility = false;
    QModelIndex curr = mdl -> index(0, 0);
    expandAll();

    while(true) {
        if (!curr.isValid()) break;

        IItem * node = mdl -> item(curr);
        if (!node -> is(ItemState::proceeded))
            if (!node -> isContainer())
                Library::instance() -> directItemStateRestoration(curr);

        node -> updateCheckedStateByPredicate(flag);

        if (!ensureVisibility && node -> is(flag)) {
            ensureVisibility = true;
            scrollTo(curr, (Settings::instance() -> isHeightUnificate() ? QAbstractItemView::EnsureVisible : QAbstractItemView::PositionAtCenter));
        }

        curr = indexBelow(curr);
    }

    emit mdl -> moveOutProcess();
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
        removeRow(node, isRemoveFileWithItem());
        node = mdl -> fromPath(nodePath, direction);
        findExecutable(node);
    }
    else findExecutable(node);

    if (node.isValid()) {
        if (!execIndex(node)) {
            QApplication::processEvents();
            findAndExecIndex(false);
        }
    }
}

bool IView::removeRow(const QModelIndex & node, bool remove_file_with_item, int selectionUpdate, bool usePrevAction) {
    bool isFolder = false;

    if (Settings::instance() -> isAlertOnFolderDeletion()) {
        if ((isFolder = node.data(IEXECCOUNTS) > 0)) {
            if (usePrevAction && _deleteFolderAnswer == QMessageBox::NoToAll)
                return false;

            if (!usePrevAction || (usePrevAction && _deleteFolderAnswer != QMessageBox::YesToAll)) {
                //INFO: need to send signal only in separate thread - Qt: Dead lock detected while activating a BlockingQueuedConnection
                if (QThread::currentThread() == QApplication::instance() -> thread()) {
                    UserDialogBox::instance() -> alert(
                        QStringLiteral("Folder deletion"),
                        QStringLiteral("Are you sure what you want to remove the not empty folder '") % node.data().toString() % QStringLiteral("' ?"),
                        QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll
                    );
                } else {
                    emit showAlert(
                        QStringLiteral("Folder deletion"),
                        QStringLiteral("Are you sure what you want to remove the not empty folder '") % node.data().toString() % QStringLiteral("' ?"),
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
        if (Player::instance() -> currentPlaylist() == mdl &&
             Player::instance() -> playedItemTreePath().startsWith(
            node.data(ITREESTR).toString()
        ))
            Player::instance() -> eject(false);
    }

    if (remove_file_with_item)
        mdl -> setData(node, ItemState::mark_on_removing, ISTATERESTORE);

    if (selectionUpdate != IModel::none) {
        QModelIndex newSel = QModelIndex();

        if (selectionUpdate & IModel::backward)
            newSel = candidateOnSelection(node, true);

        if (selectionUpdate & IModel::forward && !newSel.isValid()) { // this not worked for threaded deletion, because tree is not updated in process and contain a broken keys as result
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

void IView::removeProccessing(QModelIndexList & index_list, bool remove, bool inProcess) {
    float total = index_list.size(), temp = total;

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
    total /= 100.0;

    if (mdl -> containerType() == list || !inProcess) {
        for (; eit != index_list.begin(); --eit) {
            removeRow((*eit), remove, IModel::none, true);

            if (inProcess)
                emit mdl -> setProgress(--temp / total);
        }
    } else {
        for (; eit != index_list.begin(); --eit) {
            if ((*eit).data(IFOLDER).toBool())
                emit threadedRowRemoving((*eit), remove, IModel::none, true);
            else
                removeRow((*eit), remove, IModel::none, true);
            emit mdl -> setProgress(--temp / total);
        }
    }
    blockRepaint = false;

    if (inProcess)
        // in thread item selected in view, but not added to selected model :(
        emit threadedRowRemoving((*eit), remove, IModel::none, true); // if last elem is folder - throwned error if we in thread
    else
        removeRow((*eit), remove, IModel::backward | IModel::forward, true);

    index_list.clear();
    if (inProcess)
        emit mdl -> moveOutProcess();

    mdl -> syncMutex() -> unlock();
}

void IView::removeSelectedItems(bool remove) {
    QModelIndexList list = selectedIndexes();
    selectionModel() -> clearSelection();

    if (!list.isEmpty()) {
        while (!mdl -> syncMutex() -> tryLock())
            QApplication::processEvents();

        Library::instance() -> declineAllItemsRestoration(model());
    } else return;

    if (list.size() > 200)
        QtConcurrent::run(this, &IView::removeProccessing, list, remove && isRemoveFileWithItem(), true);
    else if (list.size() > 1)
        removeProccessing(list, isRemoveFileWithItem());
    else {
        QModelIndex ind = currentIndex();
        if (ind.isValid()) {
            removeRow(ind, remove && isRemoveFileWithItem(), true, false);
            mdl -> syncMutex() -> unlock();
        }
    }
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

    if (Settings::instance() -> isCheckboxShow()) {
        downloadChecked(path);
    } else {
        QModelIndexList indexes = selectedIndexes();
        for(QModelIndexList::Iterator index = indexes.begin(); index != indexes.end(); index++) {
            if ((*index).data(IFOLDER).toBool())
                downloadBranch((*index), path);
            else
                downloadItem((*index), path);
        }
    }
}

void IView::downloadChecked(QString & path, FolderItem * root) {
    if (!root) root = mdl -> item<FolderItem>(QModelIndex());
    QList<IItem *> children = root -> childrenList();

    for(QList<IItem *>::Iterator it = children.begin(); it != children.end(); it++) {
        if ((*it) -> is(IItem::checked)) {
            if ((*it) -> isContainer())
                downloadChecked(path, (FolderItem *)*it);
            else
                downloadItem(mdl -> index(*it), path);
        }
    }
}

void IView::markLikedAsChecked() {
    checkByPredicate(IItem::liked);
}
void IView::markNewAsChecked() {
    checkByPredicate(IItem::new_item);
}
void IView::markListenedAsChecked() {
    checkByPredicate(IItem::listened);
}

void IView::moveCheckedToNewTab(FolderItem * root) {
    //TODO: realisation needed
}




void IView::setIconSize(const QSize & size) {
    setIndentation(Settings::instance() -> treeIndentation());
    QTreeView::setIconSize(size);
    item_delegate -> recalcAttrs(size.width());
}

void IView::appendRows(QList<QUrl> & urls) {
    mdl -> threadlyInsertRows(urls, -1);
}

void IView::markSelectedAsLiked(bool liked) {
    int state = liked ? ItemState::liked : -ItemState::liked;

    QModelIndexList indexes = selectedIndexes();
    QModelIndexList::ConstIterator it = indexes.begin();
    for (; it != indexes.end(); ++it)
        mdl -> setData(*it, state, ISTATE);
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

    expand(curr.parent());

    if (direction & IModel::forward) {
        while(true) {
            if (model() -> hasChildren(curr))
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
    if (event -> source() == this)
        removeSelectedItems(false);

    QTreeView::dropEvent(event);
    event -> accept();
}

void IView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Space) {
        if (Player::instance() -> playedItem())
            Player::instance() -> playPause();
        else
           execNextIndex();
    } else if (event -> key() == Qt::Key_Enter || event -> key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        if (!list.isEmpty())
            execIndex(list.first());

    } else if (event -> key() == Qt::Key_Delete)
        removeSelectedItems();
    else if (event -> key() == Qt::Key_F && event -> modifiers() & Qt::CTRL)
        ((DockBar *)parent()) -> showSearch();
    else if (event -> key() == Qt::Key_C && event -> modifiers() & Qt::CTRL && event -> modifiers() & Qt::SHIFT)
        copyIdsToClipboard();
    else if (event -> key() == Qt::Key_V && event -> modifiers() & Qt::CTRL && event -> modifiers() & Qt::SHIFT)
        importIds();
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
            drag -> setPixmap(QPixmap(QStringLiteral(":drag")));
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }
    else
        QTreeView::mouseMoveEvent(event); // if we add this to common loop - after dnd we lost focus on widget
}
