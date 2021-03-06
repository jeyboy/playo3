#include "view_interface.h"
#include "dockbars.h"
#include "toolbars.h"

using namespace Core;
using namespace Views;
using namespace Controls;

void IView::registerParent(QWidget * newParent) {
    setParent(newParent);
    mdl -> setObjectName(newParent -> objectName() % MODEL_POSTFIX);

    connect(mdl, SIGNAL(moveInBackgroundProcess()), newParent, SLOT(onMoveInBackgroundProcess()));
    connect(mdl, SIGNAL(changeIco(QImage)), newParent, SLOT(onSetDefaultIco(QImage)));

    mdl -> updateIcon();

    connect(mdl, SIGNAL(moveOutBackgroundProcess()), newParent, SLOT(onMoveOutBackgroundProcess()));
    connect(mdl, SIGNAL(setBackgroundProgress(int)), newParent, SLOT(onSetBackgroundProgress(int)));

    connect(mdl, SIGNAL(moveInProcess()), newParent, SLOT(onMoveInProcess()));
    connect(mdl, SIGNAL(moveOutProcess()), newParent, SLOT(onMoveOutProcess()));
    connect(mdl, SIGNAL(setProgress(int)), newParent, SLOT(onSetProgress(int)));
    connect(mdl, SIGNAL(setProgress2(int)), newParent, SLOT(onSetProgress2(int)));

    connect(mdl, SIGNAL(updateRemovingBlockation(bool)), this, SLOT(updateRemovingBlockation(bool)));
}

IView::IView(IModel * newModel, QWidget * parent)
    : QTreeView(parent), block_repaint(false), block_deletion(false), mdl(newModel), direction(IModel::forward) {

    connect(this, SIGNAL(registerSync(QAbstractItemModel*,QMutex*)), &DataFactory::obj(), SLOT(registerSync(QAbstractItemModel*,QMutex*)), Qt::DirectConnection);
    connect(this, SIGNAL(unregisterSync(QAbstractItemModel*)), &DataFactory::obj(), SLOT(unregisterSync(QAbstractItemModel*)), Qt::DirectConnection);
    connect(this, SIGNAL(discardSync(QAbstractItemModel*)), &DataFactory::obj(), SLOT(discardSync(QAbstractItemModel*)), Qt::DirectConnection);
    connect(this, SIGNAL(infoInvalidation(QModelIndex)), &DataFactory::obj(), SLOT(proceedInfo(QModelIndex)), Qt::DirectConnection);
    connect(this, SIGNAL(infoInvalidationAsync(QModelIndex, bool)), &DataFactory::obj(), SLOT(proceedInfoAsync(QModelIndex, bool)));
    connect(this, SIGNAL(changeCadrSize(QAbstractItemModel*,int)), &DataFactory::obj(), SLOT(changeCadrSize(QAbstractItemModel*,int)));

    setIndentation(Settings::obj().treeIndentation());
    setStyle((tree_style = new TreeViewStyle()));
    setEditTriggers(QAbstractItemView::NoEditTriggers);

//    /////////////////////////////////
//    setAutoFillBackground(false);
//    setAllColumnsShowFocus(false);
//    setAttribute(Qt::WA_NoSystemBackground, true);
//    setAttribute(Qt::WA_OpaquePaintEvent, true);
//    /////////////////////////////////

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::CopyAction);

    setExpandsOnDoubleClick(true);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection); // ContiguousSelection

    setUniformRowHeights(Settings::obj().isHeightUnificate());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setItemDelegate((item_delegate = new ModelItemDelegate(this)));

    setContextMenuPolicy(Qt::DefaultContextMenu);
    int iconDimension = Settings::obj().iconHeight();
    setIconSize(QSize(iconDimension, iconDimension));

    connect(this, SIGNAL(threadedRowRemoving(QModelIndex,bool,int,int)), this, SLOT(removeRow(QModelIndex,bool,int,int)), Qt::BlockingQueuedConnection);

    connect(this, SIGNAL(activated(const QModelIndex &)), this, SLOT(onDoubleClick(const QModelIndex &)));
    connect(this, SIGNAL(expanded(const QModelIndex &)), mdl, SLOT(expanded(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), mdl, SLOT(collapsed(const QModelIndex &)));

    connect(mdl, SIGNAL(expandNeeded(const QModelIndex &)), this, SLOT(expand(const QModelIndex &)));
    connect(mdl, SIGNAL(collapseNeeded(const QModelIndex &)), this, SLOT(collapse(const QModelIndex &)));
    connect(mdl, SIGNAL(spoilNeeded(const QModelIndex &)), this, SLOT(onSpoilNeeded(const QModelIndex &)));
    connect(mdl, SIGNAL(playNeeded(const QModelIndex &)), this, SLOT(onPlayNeeded(const QModelIndex &)));
    connect(mdl, SIGNAL(fetchNeeded(const QModelIndex &)), this, SLOT(onFetchNeeded(const QModelIndex &)));
//    connect(this -> verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollValueChanged(int)));

    setMinimumSize(100, 60);

    setModel(mdl);
    emit registerSync(mdl, mdl -> syncMutex());

    registerParent(parent);

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
    connect(selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentRowChanging(QModelIndex,QModelIndex)));
}

IView::~IView() {
    while (!mdl -> syncMutex() -> tryLock())
        QApplication::processEvents();

    emit unregisterSync(mdl);
    selectionModel() -> deleteLater();
    mdl -> syncMutex() -> unlock();
    mdl -> deleteLater();
    delete tree_style;
}

void IView::scrollToActive() {
    QModelIndex ind = DataFactory::obj().playedIndex();
    if (ind.isValid())
        scrollTo(ind, QAbstractItemView::PositionAtCenter);
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

bool IView::restoreSelection(const QString & path) {
    QModelIndex ind = mdl -> fromPath(path);
    if (ind.isValid()) {
        setCurrentIndex(ind);
        return true;
    }
    else return false;
}

bool IView::execPath(const QString & path, IPlayer::InitializationState init_state, uint start) {
    QModelIndex ind = mdl -> fromPath(path);
    if (ind.isValid()) {
        setFocus();
        execIndex(ind, init_state, start);
        return true;
    } else
        return false;
}

bool IView::execIndex(const QModelIndex & node, IPlayer::InitializationState init_state, uint start) {
    if (node.isValid()) { // INFO: play playable and choosed by user
        IItem * itm = mdl -> item(node);

        if (!itm -> isContainer()) {
            qDebug() << "PLAYED " << itm -> title();
            Presentation::Dockbars::obj().setPlayed((DockBar *)parent());

            if (Settings::obj().isSpoilOnActivation())
                scrollTo(node, scrollHint());

            if (DataFactory::obj().playedItem() == itm) {
                DataFactory::obj().proceedPauseToggling();
                return true;
            } else {
                DataFactory::obj().proceedPlaying(this, itm, start, init_state);
                return true;
            }
        }
    }

    return false;
}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////
bool IView::startInnerSearch(const QString & predicate, const QModelIndex & ind) {
    bool empty = predicate.isEmpty(), has_item = false;
    mdl -> blockFetching();

    QModelIndex it;

    for(int row = 0; ; row++) {
        it = mdl -> index(row, 0, ind);
        if (it.isValid()) {
            if (it.data(IFOLDER).toBool()) {
                bool has_items = startInnerSearch(predicate, it);
                has_item |= has_items;
                setRowHidden(row, ind, !has_items);
            } else {
                bool valid = empty || it.data().toString().contains(predicate, Qt::CaseInsensitive);
                has_item |= valid;
                setRowHidden(row, ind, !valid);
            }
        } else break;
    }

    if (ind == QModelIndex())
        emit searchFinished();
    else
        scrollToTop();

    return has_item;
}

void IView::endInnerSearch() {
    mdl -> blockFetching(false);
    onSpoilNeeded(activeIndex());
}

void IView::onUpdateAttr(const QModelIndex & ind, int attr, const QVariant & val) {
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
        scrollTo(node, scrollHint());
    }
}

void IView::onPlayNeeded(const QModelIndex & node) {
    if (node.isValid()) {
        onSpoilNeeded(node);
        execIndex(node);
    }
}

void IView::onFetchNeeded(const QModelIndex & node) {
    Playlist * item = mdl -> item<Playlist>(node);
    QScrollBar * scroll_bar = verticalScrollBar();
    bool has_scroll = scroll_bar -> isVisible();
    QRect view_rect = rect();

    QModelIndex top_index = indexAt(rect().topLeft());
    QModelIndex bottom_index = indexAt(rect().bottomRight());

    bool rejected = !(top_index.isValid() || bottom_index.isValid()); // block fetching on first drawing of tab

    if (has_scroll && !rejected) {
        int slider_pos = scroll_bar -> sliderPosition();
        rejected = !node.isValid() && slider_pos == scroll_bar -> minimum(); // block fetching if we on the top of scroll in root parent

        if (!rejected) { // if we not in root parent or not on the top of scroll
            rejected = !(!node.isValid() && slider_pos == scroll_bar -> maximum());

            if (rejected) {
                QModelIndex last_child_index = index(item -> lastChild());
                QRect child_rect = visualRect(last_child_index);
                rejected = !child_rect.intersects(view_rect); // reject if last child of fetchable container not on the screen
            }
        }
    }

    item -> setStates(IItem::flag_not_in_proc);

    if (rejected) return;
    mdl -> fetchMore(node);
}

void IView::updateSelection(QModelIndex & node) {
    if (node.isValid()) {
        if (!node.data(IPLAYABLE).toBool())
            findExecutable(node);

        onSpoilNeeded(node);
    }
}

void IView::runItemCmd(IItem * it) {
    IItem * item = it ? it : mdl -> item(currentIndex());

    if (item -> isContainer())
        mdl -> fetchMore(index(item));
    else
        mdl -> fetchMore(index(item -> parent()));
}

void IView::onCurrentRowChanging(QModelIndex, QModelIndex) {
    if (Presentation::Dockbars::obj().activeView() == this)
        Presentation::ToolBars::obj().selectedItemFeatures() -> updateToolbar();
}

void IView::onDoubleClick(const QModelIndex node) {
    if (!execIndex(node) && !node.data(IFOLDER).toBool()) { // find first valid for exec
        if (Settings::obj().isCheckboxShow()) {
            QModelIndex node = QModelIndex();
            findExecutable(node);
            execIndex(node);
        }
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
    UserActionDialog dialog(this);
    dialog.buildImportForm();

    if (dialog.exec() == QDialog::Accepted)
        mdl -> importIds(
            dialog.getValue(dialog.text_key).split(QRegularExpression(LSTR("[^\\w\\^-]")), QString::SkipEmptyParts)
        );
}

void IView::copyIdsToClipboard() {
    QModelIndexList indexes = selectedIndexes();
    mdl -> copyIdsToClipboard(indexes);
}

void IView::drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex & index) const {
    IItem * node = static_cast<IItem *>(index.internalPointer());

    if (node -> expandRequired()) // required for uncanonical delition and after loading state reconstruction
        emit mdl -> expandNeeded(index);


//    Playlist parent_node = node -> parent();
//    if (parent_node -> hasMoreItems() && !parent_node -> is(IItem::flag_in_proc)) { // check fetch for sibling folders
//        QModelIndex parent_index = mdl -> index(parent_node);
//        if (parent_index.isValid())
//            mdl -> canFetchMore(parent_index);
//    }

    if (!node -> is(IItem::flag_proceeded)) {
        node -> set(IItem::flag_proceeded);
        emit infoInvalidationAsync(index, node -> isRemote());
    }

    QTreeView::drawRow(painter, options, index);
}

void IView::paintEvent(QPaintEvent * event) {
    if (!block_repaint)
        QTreeView::paintEvent(event);
}

void IView::resizeEvent(QResizeEvent * event) {   
    if (event -> size().height() > 0)
        emit changeCadrSize(
            mdl,
            (event -> size().height() / Settings::obj().totalItemHeight())/* + 1*/
        );

    QTreeView::resizeEvent(event);
}

void IView::focusInEvent(QFocusEvent *) {
    Presentation::Dockbars::obj().setActive((DockBar *)parent());
}

void IView::contextMenuEvent(QContextMenuEvent * event) { // FIXME: shortcuts is not work properly // need to use addActions() maybe
    QMenu menu(this);

    if (isConfigurable()) {
        menu.addAction(QIcon(LSTR(":/settings")), LSTR("Playlist settings"), &Presentation::Dockbars::obj(), SLOT(editActiveBar()));
        menu.addSeparator();
    }

    if (isRefresheable()) {
        menu.addAction(QIcon(LSTR(":/refresh")), LSTR("Refresh items"), mdl, SLOT(refresh()));
        menu.addSeparator();
    }

    QModelIndex ind = indexAt(event -> pos());
    IItem * itm = mdl -> item(ind);   

    menu.addAction(QIcon(LSTR(":/search")), LSTR("Find"), parent(), SLOT(showSearch()), QKeySequence(Qt::CTRL + Qt::Key_F));
    menu.addSeparator();

    if (DataFactory::obj().playedIndex().isValid()) {
        menu.addAction(
            QIcon(LSTR(":/active_tab")), LSTR("Show active elem"),
            &Presentation::Dockbars::obj(), SLOT(scrollToActive()), QKeySequence(tr("Ctrl+P", "Played elem"))
        );
        menu.addSeparator();
    }

    if (ind.isValid()) {
        Presentation::ToolBars::obj().selectedItemFeatures() -> appendToMenu(&menu);
        menu.addSeparator();

        menu.addAction(QIcon(LSTR(":/copy")), LSTR("Copy name to clipboard"), this, SLOT(copyToClipboard()), QKeySequence(tr("Ctrl+C", "Copy")));
        menu.addSeparator();

        if (!ind.data(IFULLPATH).toString().isEmpty()) {
            menu.addAction(QIcon(LSTR(":/open")), LSTR("Open location"), this, SLOT(openLocation()));
            menu.addSeparator();
        }
    }

    menu.addAction(
        QIcon(LSTR(":/import")), LSTR("Import Ids"),
        this, SLOT(importIds()), QKeySequence(tr("Ctrl+Shift+V", "Insert ids"))
    );

    if (mdl -> rowCount() > 0) {
        if (!selectedIndexes().isEmpty()) {
            menu.addAction(
                QIcon(LSTR(":/export")), LSTR("Copy Ids to Clipboard"),
                this, SLOT(copyIdsToClipboard()), QKeySequence(tr("Ctrl+Shift+C", "Copy ids"))
            );
            menu.addSeparator();
        }       

        if (Settings::obj().isCheckboxShow()) {
            menu.addSeparator();

//            menu.addAction(QIcon(":/move"), "Copy Checked To New Tab", this, SLOT(moveCheckedToNewTab()));

            menu.addAction(QIcon(LSTR(":/move")), LSTR("Check All"), mdl, SLOT(markAllAsChecked()));
            menu.addAction(QIcon(LSTR(":/move")), LSTR("Uncheck All"), mdl, SLOT(markAllAsUnchecked()));
            menu.addAction(QIcon(LSTR(":/move")), LSTR("Check Liked"), this, SLOT(markLikedAsChecked()));
            menu.addAction(QIcon(LSTR(":/move")), LSTR("Check New"), this, SLOT(markNewAsChecked()));
            menu.addAction(QIcon(LSTR(":/move")), LSTR("Check Listened"), this, SLOT(markListenedAsChecked()));
            menu.addSeparator();
        }

        menu.addAction(QIcon(LSTR(":/download")), LSTR("Download"), this, SLOT(downloadSelected()));
        menu.addAction(QIcon(LSTR(":/download")), LSTR("Download All"), this, SLOT(downloadAll()));
        menu.addSeparator();

        menu.addAction(QIcon(LSTR(":/shuffle")), LSTR("Shuffle"), this, SLOT(shuffle()));

        if (mdl -> ignoreListHasItems()) {
            menu.addSeparator();
            menu.addAction(QIcon(), LSTR("Clear ignore list"), mdl, SLOT(clearIgnoreList()));
        }

        if (mdl -> playlistType() != dt_level) {
            menu.addSeparator();

            if (itm -> parent())
                menu.addAction(QIcon(LSTR(":/collapse")), LSTR("Collapse parent"), this, SLOT(collapseParent()));
            menu.addAction(QIcon(LSTR(":/collapse")), LSTR("Collapse all"), this, SLOT(collapseAll()));
            menu.addAction(QIcon(LSTR(":/expand")), LSTR("Expand all"), this, SLOT(expandAll()));
        }

        menu.addAction(QIcon(), LSTR("Clear"), this, SLOT(clear()));
    }

    if (!menu.isEmpty()) {
        menu.exec(event -> globalPos());
        event -> accept();
    } else event -> ignore();
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
        if (!node -> isContainer() && !node -> is(IItem::flag_proceeded)) {
            node -> set(IItem::flag_proceeded);
            emit infoInvalidation(curr);
        }

        node -> updateCheckedStateByPredicate(flag);

        if (!ensureVisibility && node -> is(flag)) {
            ensureVisibility = true;
            scrollTo(curr, scrollHint());
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
        bool is_valid = node.isValid();

        if (!is_valid || (is_valid && !node.data(IFOLDER).toBool()))
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

bool IView::removeRow(const QModelIndex & node, bool remove_file_with_item, int selection_update, int flags) {
    bool is_folder = false;

    // did not allow manuall removing of dummies
    if (node.data(ITYPE) == dt_dummy)
        return false;

    if (Settings::obj().isAlertOnFolderDeletion()) {
        QVariant folder_items_amount = node.data(IEXECCOUNTS);
        is_folder = (folder_items_amount.isValid() && folder_items_amount.toInt() > 0);

        if (is_folder) {
            bool mimic_prev_action = flags & use_prev_action;
            if (mimic_prev_action && _delete_folder_answer == QMessageBox::NoToAll)
                return false;

            if (!mimic_prev_action || (mimic_prev_action && _delete_folder_answer != QMessageBox::YesToAll)) {
                UserDialogBox::obj().alert(
                    this,
                    LSTR("Folder deletion"),
                    LSTR("Are you sure what you want to remove the not empty folder '") % node.data().toString() % LSTR("' ?"),
                    QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll
                );

                _delete_folder_answer = UserDialogBox::obj().lastAnswer();
                if (_delete_folder_answer == QMessageBox::No || _delete_folder_answer == QMessageBox::NoToAll)
                    return false;
            }
        }
    }

    if (!(flags & dont_remove_played)) {
        if (DataFactory::obj().playedIndex().isValid()) {
            if (DataFactory::obj().currentPlaylist() == this &&
                 DataFactory::obj().playedItemTreePath().startsWith(
                node.data(ITREESTR).toString()
            ))
                DataFactory::obj().resetPlaying();
        }

        if (remove_file_with_item)
            mdl -> setData(node, IItem::flag_mark_on_removing, ISTATERESTORE);
    }

    if (selection_update != IModel::none) {
        QModelIndex newSel = QModelIndex();

        if (selection_update & IModel::backward)
            newSel = candidateOnSelection(node, true);

        if (selection_update & IModel::forward && !newSel.isValid()) { // this not worked for threaded deletion, because tree is not updated in process and contain a broken keys as result
            if (is_folder) {
                collapse(newSel);
                newSel = indexBelow(newSel);
            }
            newSel = candidateOnSelection(node);
        }

        setCurrentIndex(newSel);
    }

    return model() -> removeRow(node.row(), node.parent());
}

void IView::removeProccessing(QModelIndexList & index_list, bool remove, int flags) {
    float total = index_list.size(), temp = total;
    flags |= use_prev_action;
    bool inProcess = !ThreadUtils::inMainThread();

    if (inProcess)
        emit mdl -> moveInProcess();

    _delete_folder_answer = QMessageBox::No;

//    if (mdl -> playlistType() == level) {
//        qSort(index_list.begin(), index_list.end());
//    } else {
        qSort(index_list.begin(), index_list.end(), modelIndexComparator());

        //INFO - remove dependent items
        mdl -> setProgress2(SPINNER_IS_CONTINIOUS);
        QMutableListIterator<QModelIndex> i(index_list);
        QString path = "--", elem_path;
        QModelIndex ind;

        while(i.hasNext()) {
            ind = i.next();
            IItem * itm = mdl -> item(ind);
//            if (itm -> dataType() == dt_dummy) continue; //INFO: not tested

            elem_path = itm -> buildTreePath(); // ind.data(ITREEPATH).toString();
            if (!elem_path.startsWith(path)) {
                if (itm -> isContainer()) //ind.data(IFOLDER).toBool())
                    path = elem_path;
            }
            else i.remove();
        }

        mdl -> setProgress2(SPINNER_NOT_SHOW_SECOND);
        temp = total = index_list.size();
        //////
//    }

    block_repaint = true; // list type sometimes trying redraw yourself in process :(
    QModelIndexList::Iterator eit = --index_list.end();
    total /= 100.0;

    if (/*mdl -> playlistType() == level || */!inProcess) {
        for (; eit != index_list.begin(); --eit) {
            removeRow((*eit), remove, IModel::none, flags);

            if (inProcess)
                emit mdl -> setProgress(--temp / total);
        }
    } else {
        for (; eit != index_list.begin(); --eit) {
            if ((*eit).data(IFOLDER).toBool())
                emit threadedRowRemoving((*eit), remove, IModel::none, flags);
            else
                removeRow((*eit), remove, IModel::none, flags);
            emit mdl -> setProgress(--temp / total);
        }
    }
    block_repaint = false;

    if (inProcess)
        // in thread item selected in view, but not added to selected model :(
        emit threadedRowRemoving((*eit), remove, IModel::none, flags); // if last elem is folder - throwned error if we in thread
    else
        removeRow((*eit), remove, IModel::backward | IModel::forward, flags);

    index_list.clear();
    if (inProcess)
        emit mdl -> moveOutProcess();

    mdl -> syncMutex() -> unlock();
}

void IView::removeSelectedItems(bool remove, int flags) {
    if (block_deletion) return;

    QModelIndexList list = selectedIndexes();
    selectionModel() -> clearSelection();

    if (!list.isEmpty()) {
        while (!mdl -> syncMutex() -> tryLock())
            QApplication::processEvents();

        emit discardSync(mdl);
    } else return;

    if (list.size() > 200) {
        QtConcurrent::run(this, &IView::removeProccessing, list, remove && isRemoveFileWithItem(), flags);
    } else if (list.size() > 1)
        removeProccessing(list, remove && isRemoveFileWithItem());
    else {
        QModelIndex ind = currentIndex();
        if (ind.isValid()) {
            removeRow(ind, remove && isRemoveFileWithItem(), true, flags);
            mdl -> syncMutex() -> unlock();
        }
    }
}

void IView::downloadItems(const QModelIndexList & nodes, const QString & save_path) {
    if (nodes.isEmpty()) return;
    QDropEvent * event = new QDropEvent(QPointF(0,0), Qt::CopyAction, mdl -> mimeData(nodes), Qt::NoButton, Qt::NoModifier);
    DownloadView::obj().proceedDrop(event, save_path);
}

void IView::downloadBranch(const QModelIndex & node, const QString & save_path) {
    Playlist * curr = mdl -> item<Playlist>(node);
    IItem * item;
    QModelIndexList list;

    for(int i = 0; i < curr -> childCount(); i++) {
        item = curr -> child(i);

        if (item -> isContainer())
            downloadBranch(mdl -> index(item), save_path);
        else
            list << mdl -> index(item);
    }

    downloadItems(list, save_path);
}

void IView::downloadAll() {
    downloadBranch(QModelIndex(), Settings::obj().defaultDownloadPath());
}

void IView::downloadSelected() {
    QString path = Settings::obj().defaultDownloadPath();

    if (Settings::obj().isCheckboxShow())
        downloadChecked(path);
    else {
        QModelIndexList indexes = selectedIndexes();
        downloadItems(indexes, path);
    }
}

void IView::downloadChecked(QString & path, Playlist * root) {
    if (!root) root = mdl -> item<Playlist>(QModelIndex());
    QList<IItem *> children = root -> childrenList();
    QModelIndexList list;

    for(QList<IItem *>::Iterator it = children.begin(); it != children.end(); it++)
        if ((*it) -> is(IItem::flag_checked)) {
            if ((*it) -> isContainer())
                downloadChecked(path, (Playlist *)*it);
            else
                list << mdl -> index(*it);
        }

    downloadItems(list, path);
}

void IView::markLikedAsChecked() {
    checkByPredicate(IItem::flag_liked);
}
void IView::markNewAsChecked() {
    checkByPredicate(IItem::flag_new_item);
}
void IView::markListenedAsChecked() {
    checkByPredicate(IItem::flag_listened);
}

void IView::moveCheckedToNewTab(Playlist * /*root*/) {
    //TODO: realisation needed
}




void IView::setIconSize(const QSize & size) {
    setIndentation(Settings::obj().treeIndentation());
    QTreeView::setIconSize(size);
    item_delegate -> recalcAttrs(size.width());
}

void IView::appendRows(const QList<QUrl> & urls, const bool & play) {
    mdl -> threadlyInsertRows(urls, -1, QModelIndex(), play);
}

void IView::markSelectedAsLiked(bool liked) {
    int state = liked ? IItem::flag_liked : IItem::flag_not_liked;

    QModelIndexList indexes = selectedIndexes();
    QModelIndexList::ConstIterator it = indexes.begin();
    for (; it != indexes.end(); ++it)
        mdl -> setData(*it, state, ISTATE);
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

QModelIndex IView::activeIndex() {
    QModelIndex ind = DataFactory::obj().playedIndex();

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

    if (DataFactory::obj().playedIndex() != curr && curr.data(IPLAYABLE).toBool())
        return;

    mdl -> blockFetching();
    expand(curr.parent());

    if (direction & IModel::forward) { // need to block loadability and proc it manualy - background loading of items broke app
        while(true) {
            if (model() -> hasChildren(curr)) {
                /////////// fetchable block logic ////////////
                bool take_next = true;
                QModelIndex temp = indexAbove(curr);
                IItem * itm = mdl -> item(temp);
                if (itm) {
                    itm = itm -> parent();
                    if (itm && itm -> hasMoreItems()) {
                        curr = temp;
                        take_next = false;
                        mdl -> fetchMore(index(itm), false);
                    }
                }

                if (take_next) {
                    ////////////////////////////////////////
                    itm = mdl -> item(curr);
                    if (itm && itm -> hasMoreItems())
                        mdl -> fetchMore(curr, false);
                    expand(curr);
                }
            }

            curr = indexBelow(curr);

            if (!curr.isValid() || curr.data(IPLAYABLE).toBool()) {
                mdl -> blockFetching(false);
                return;
            }
        }
    } else {
        QModelIndex temp = curr;

        while(true) {
            if (model() -> hasChildren(curr) && temp.parent() != curr) {
                IItem * itm = mdl -> item(curr);
                if (itm -> hasMoreItems())
                    mdl -> fetchMore(curr, false);

                expand(curr);
                curr = temp;
            }
            else temp = curr;

            curr = indexAbove(curr);

            if (!curr.isValid() || curr.data(IPLAYABLE).toBool()) {
                mdl -> blockFetching(false);
                return;
            }
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
        mdl -> setDNDList(selectedIndexes());
//    if (event -> source() == this)
//        removeSelectedItems(false, dont_remove_played);

    QTreeView::dropEvent(event);
    event -> accept();
}

void IView::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Space) {
        if (DataFactory::obj().playedItem())
            DataFactory::obj().proceedPauseToggling();
        else
           execNextIndex();
    } else if (event -> key() == Qt::Key_Enter || event -> key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        if (!list.isEmpty())
            execIndex(list.first());

    } else if (event -> key() == Qt::Key_Delete)
        removeSelectedItems();
    // monkey patch for unworked shortcuts on menu
    else if (event -> key() == Qt::Key_F && event -> modifiers() & Qt::CTRL)
        ((DockBar *)parent()) -> showSearch();
    else if (event -> key() == Qt::Key_C && event -> modifiers() & Qt::CTRL && event -> modifiers() & Qt::SHIFT)
        copyIdsToClipboard();
    else if (event -> key() == Qt::Key_V && event -> modifiers() & Qt::CTRL && event -> modifiers() & Qt::SHIFT)
        importIds();
    else if (event -> key() == Qt::Key_P && event -> modifiers() & Qt::CTRL)
        Presentation::Dockbars::obj().scrollToActive();

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
            drag -> setPixmap(QPixmap(LSTR(":drag")));
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }
    else
        QTreeView::mouseMoveEvent(event); // if we add this to common loop - after dnd we lost focus on widget
}
