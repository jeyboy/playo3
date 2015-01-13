#include "view_interface.h"
//#include "media/library.h"
//#include "web/download.h"
#include "tree_view_style.h"

using namespace Playo3;

View::View(Model * newModel, QWidget *parent, CBHash settingsSet) : QTreeView(parent) {
    settings = settingsSet;
    setModel((model = newModel));

    setIndentation(12);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    setStyle(new TreeViewStyle);

    setDragDropMode(QAbstractItemView::DragDrop);
    setDefaultDropAction(Qt::CopyAction);

    setExpandsOnDoubleClick(true);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

//  setDragDropOverwriteMode(true);
//  setMovement(QTreeView::Snap);

//  setFlow(QListView::TopToBottom);

//    setTreePosition(2);
//    setRootIndex();
//    setRowHidden(0, rootIndex(), true);

//    expandAll();

    setItemDelegate(new ModelItemDelegate(this));

    setContextMenuPolicy(Qt::CustomContextMenu);
    int iconDimension = Settings::instance() -> getIconHeight();
    setIconSize(QSize(iconDimension, iconDimension));

    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onDoubleClick(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &)));
    connect(this, SIGNAL(expanded(const QModelIndex &)), model, SLOT(expanded(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), model, SLOT(collapsed(const QModelIndex &)));

    connect(model, SIGNAL(expandNeeded(const QModelIndex &)), this, SLOT(expand(const QModelIndex &)));
    connect(model, SIGNAL(spoilNeeded(const QModelIndex &)), this, SLOT(updateSelection(QModelIndex)));

    connect(model, SIGNAL(itemsCountChanged(int)), parent, SLOT(updateHeader(int)));
    connect(model, SIGNAL(showSpinner()), this, SLOT(startRoutine()));
    connect(model, SIGNAL(hideSpinner()), this, SLOT(stopRoutine()));
    connect(model, SIGNAL(updated()), this, SLOT(modelUpdate()));
    connect(model, SIGNAL(showMessage(QString)), this, SLOT(showMessage(QString)));

    connect(Download::instance(), SIGNAL(slotChanged(QString)), this, SLOT(setHeaderText(QString)));

//    connect(model, SIGNAL(selectionChangeNeeded(const QModelIndex &index)), this, SLOT(changeSelection(const QModelIndex &index)));
//    connect(model, SIGNAL(selectionUpdateNeeded()), this, SLOT(updateSelection()));

    header() -> setSectionResizeMode(0, QHeaderView::Interactive);
//    header()->setStretchLastSection(false);
}

View::~View() {
    delete model;
}

QJsonObject View::toJSON() {
    QJsonObject res = model -> getItem(rootIndex()) -> toJSON();

    QJsonObject set = QJsonObject();
    foreach(QString c, settings.keys()) {
        set[c] = settings.value(c);
    }

    res["p"] = QString();
    res["set"] = set;
    res["l"] = model -> itemsCount();
    return res;
}

void View::scrollToActive() {
    if (Player::instance() -> playedItem()) {
        scrollTo(model -> index(Player::instance() -> playedItem()));
    }
}

void View::proceedPrev() {
    ModelItem * item = activeItem(false);
    if (item == 0) return;

    item = prevItem(item);
    execItem(item);
}

void View::proceedNext() {
    ModelItem * item = activeItem();
    if (item == 0) return;

    item = nextItem(item);
    execItem(item);
}

void View::deleteCurrentProceedNext() {
    ModelItem * item = activeItem();
    if (item == 0) return;

    item = nextItem(item);

    if (Player::instance() -> currentPlaylist() == this) {
        if (Player::instance() -> playedItem()) {
            removeItem(Player::instance() -> playedItem());
        }
    }

    execItem(item);
}

ModelItem * View::fromPath(QString path) {
    QStringList parts = path.split(' ', QString::SkipEmptyParts);
    ModelItem * curr = getModel() -> root();
    int level;

    while(parts.length() > 0) {
        level = parts.takeFirst().toInt();
        curr = curr -> child(level);
    }

    return curr;
}

Model * View::getModel() const {
    return model;
}

//template<class T> T * View::getModel() const {
//    return dynamic_cast<T *>(model);
//}

CBHash View::getSettings() const {
    return settings;
}
void View::setSettings(CBHash newSettings) {
    settings = newSettings;
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

QModelIndexList View::selectedItems() const {
    return selectedIndexes();
}

bool View::execItem(ModelItem * item, bool paused) {
    if (item) {
        if (Settings::instance() -> isSpoilOnActivation())
            scrollTo(model -> index(item));
        if (item -> isExist()) {
            Player::instance() -> playItem(this, item, paused);
            return true;
        } else {
            item -> getState() -> setNotExist();
            model -> refreshItem(item);
        }
    } else return true;

    return false;
}

ModelItem * View::removeCandidate(ModelItem * item) {
    ModelItem * parent = item -> parent();

    while(parent -> childCount() == 1 && parent -> parent() != 0) {
        item = parent;
        parent = parent -> parent();
    }

    return item;
}

void View::removeItem(ModelItem * item) {
    Library::instance() -> removeRemoteItem(item);
    item = removeCandidate(item);
    QModelIndex modelIndex = model -> index(item);
    QString delPath = item -> fullPath();
    bool isFolder = item -> isFolder();

    QModelIndex parentIndex = modelIndex.parent();
    if (!parentIndex.isValid())
        parentIndex = rootIndex();
    int row = modelIndex.row();
    QModelIndex selCandidate = parentIndex.child(row + 1, 0);

    if (isFolder && item -> childTreeCount() > 1) {
        if (QMessageBox::warning(
                    parentWidget(),
                    "Folder deletion",
                    "Are you shure what you want remove the not empty folder ?",
                    QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
            return;
    }

    clearSelection();

    if (Player::instance() -> playedItem()) {
        if (Player::instance() -> playedItem() -> fullPath().startsWith(delPath))
            Player::instance() -> removePlaylist();
    }

    if (isRemoveFileWithItem()) {
        item -> removePhysicalObject();

//        if (isFolder) {
////                QDir delDir(delPath);
////                if (delPath.split('/').length() >= 2) {
////                    delDir.removeRecursively();
////                }


//        } else {
//            QFile::remove(delPath);
//        }
    }

    if (model -> removeRow(row, parentIndex)) {
        if (isFolder) {
            updateSelection(selCandidate);
        } else {
            QModelIndex next = parentIndex.child(row, 0);
            if (!next.isValid() || (next.isValid() && !next.data(FOLDERID).toBool())) {
                updateSelection(selCandidate);
            }
        }
    }
}

int View::itemsCount() const {
    return model -> itemsCount();
}

//////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////

void View::startRoutine() {
    emit showSpinner();
}
void View::stopRoutine() {
    emit hideSpinner();
}

void View::setHeaderText(QString newText) {
    model -> root() -> setData(0, newText);
    setRootIndex(rootIndex());
}

void View::showMessage(QString text) {
    QMessageBox::warning(this, "Bla bla bla", text);
}

void View::shuffle() {
    getModel() -> root() -> shuffle();
    getModel() -> refresh();
}

void View::updateSelection(QModelIndex candidate) {
    if (candidate.isValid()) {
        ModelItem * item = getModel() -> getItem(candidate);

        if (item -> isFolder()) {
            item = nextItem(item);
        }

        if (item) {
            QModelIndex newIndex = getModel() -> index(item);
            setCurrentIndex(newIndex);
            expand(newIndex);
            scrollTo(newIndex);
        }
    }
}

void View::onDoubleClick(const QModelIndex &index) {
    ModelItem * item = model -> getItem(index);

    if (!item -> isFolder()) {
        execItem(item);
    }
}

void View::showContextMenu(const QPoint& pnt) {
    QList<QAction *> actions;
    QModelIndex ind = indexAt(pnt);
    ModelItem * item = model -> getItem(ind);
    QAction * openAct;
    QAction * sepAct;

    if (Player::instance() -> playedItem()) {
        openAct = new QAction(QIcon(":/active_tab"), "Show active elem", this);
        connect(openAct, SIGNAL(triggered(bool)), QApplication::activeWindow(), SLOT(showActiveElem()));
        actions.append(openAct);

        sepAct = new QAction(this);
        sepAct -> setSeparator(true);
        actions.append(sepAct);
    }

    if (isEditable()) {
        openAct = new QAction(QIcon(":/settings"), "Tab settings", this);
        connect(openAct, SIGNAL(triggered(bool)), QApplication::activeWindow(), SLOT(showAttCurrTabDialog()));
        actions.append(openAct);

        sepAct = new QAction(this);
        sepAct -> setSeparator(true);
        actions.append(sepAct);
    }

    openAct = new QAction(QIcon(":/refresh"), "Refresh", this);
    connect(openAct, SIGNAL(triggered(bool)), model, SLOT(refresh()));
    actions.append(openAct);

    if (QString(metaObject() -> className()) == QString("VkView")) {
        openAct = new QAction(QIcon(":/refresh"), "Parse/Refresh Wall", this);
        connect(openAct, SIGNAL(triggered(bool)), model, SLOT(refreshWall()));
        actions.append(openAct);
    }

    openAct = new QAction(QIcon(":/shuffle"), "Shuffle", this);
    connect(openAct, SIGNAL(triggered(bool)), this, SLOT(shuffle()));
    actions.append(openAct);

    sepAct = new QAction(this);
    sepAct -> setSeparator(true);
    actions.append(sepAct);

    if (ind.isValid()) {
        if (!item -> fullPath().isEmpty()) {
            openAct = new QAction(QIcon(":/open"), "Open location", this);
            connect(openAct, SIGNAL(triggered(bool)), this, SLOT(openLocation()));
            actions.append(openAct);
        }

        if (item -> isRemote()) {
            sepAct = new QAction(this);
            sepAct -> setSeparator(true);
            actions.append(sepAct);

            openAct = new QAction(QIcon(":/download"), "Download", this);
            connect(openAct, SIGNAL(triggered(bool)), this, SLOT(download()));
            actions.append(openAct);
            openAct = new QAction(QIcon(":/download"), "Download All", this);
            connect(openAct, SIGNAL(triggered(bool)), this, SLOT(downloadAll()));
            actions.append(openAct);
        }
    }

    sepAct = new QAction(this);
    sepAct -> setSeparator(true);
    actions.append(sepAct);

    if (model -> getApi() != 0) {
        openAct = new QAction(QIcon(":/refresh"), "Friends (groups) audio", this);
        connect(openAct, SIGNAL(triggered(bool)), QApplication::activeWindow(), SLOT(showVKRelTabDialog()));
        actions.append(openAct);
    }

    sepAct = new QAction(this);
    sepAct -> setSeparator(true);
    actions.append(sepAct);

    if (model -> rowCount() > 0) {
        openAct = new QAction(QIcon(":/collapse"), "Collapse all", this);
        connect(openAct, SIGNAL(triggered(bool)), this, SLOT(collapseAll()));
        actions.append(openAct);

        openAct = new QAction(QIcon(":/expand"), "Expand all", this);
        connect(openAct, SIGNAL(triggered(bool)), this, SLOT(expandAll()));
        actions.append(openAct);
    }

    if (actions.count() > 0)
        QMenu::exec(actions, mapToGlobal(pnt));
}

void View::openLocation() {
    ModelItem * item = model -> getItem(this -> currentIndex());
    item -> openLocation();
}

void View::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const {
    // TODO: add initiated items to hash for update later on the same name state change
    ModelItem * item = model -> getItem(index);

    if (!item -> getState() -> isProceed()) {
        item -> getState() -> setProceed();
        if (!item -> isFolder()) {
            Library::instance() -> initItem(item, model, SLOT(libraryResponse()));
        }

        if (item -> getState() -> isExpanded()) {
            emit model -> expandNeeded(index);
        }
    }

    QTreeView::drawRow(painter, options, index);
}

void View::resizeEvent(QResizeEvent * event) {
    if (event -> oldSize().height() != size().height()) {
        if (event -> size().height() > 0) {
            int count = (event -> size().height() / Settings::instance() -> getTotalItemHeight()) + 2;
            Library::instance() -> setRemoteItemMax(count);
        }
    }

    QTreeView::resizeEvent(event);
}

bool View::prepareDownloading(QString path) {
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return true;
}

void View::downloadItem(ModelItem * item, QString savePath) {
    QString prepared_path = savePath + item -> getDownloadTitle();
    if (QFile::exists(prepared_path)) {
        QFile::remove(prepared_path);
    }

    if (item -> isRemote()) {
        if (model -> getApi() == 0) {
            QMessageBox::warning(this, "Remote download", "Some shit happened :(");
            return;
        }

        Download::instance() -> start(model, item, QUrl::fromLocalFile(prepared_path));
    } else {
        QFile f(item -> fullPath());
        if (!f.copy(prepared_path))
            QMessageBox::warning(this, "Bla", f.errorString());
    }
}

void View::downloadBranch(ModelItem * rootNode, QString savePath) {
    prepareDownloading(savePath);
    QList<ModelItem *> * children = rootNode -> childItemsList();
    ModelItem * item;

    for(int i = 0; i < children -> length(); i++) {
        item = children -> at(i);

        if (item -> isFolder()) {
            downloadBranch(item, savePath + item -> getDownloadTitle() + "/");
        } else {
            downloadItem(item, savePath);
        }
    }
}

void View::download() {
    downloadSelected(Settings::instance() -> getDownloadPath());
}

void View::downloadAll() {
    QString savePath = Settings::instance() -> getDownloadPath();
    if (!prepareDownloading(savePath)) return;
    downloadBranch(model -> root(), savePath);
}

void View::modelUpdate() {
    if (Player::instance() -> currentPlaylist() == this) {
        if (Player::instance() -> playedItem())
            Player::instance() -> playedItem() -> getState() -> unsetPlayed();
        Player::instance() -> removePlaylist();
    }
}

void View::downloadSelected(QString savePath, bool markAsLiked) {
    if (!prepareDownloading(savePath)) return;

    ModelItem * item;

    foreach(QModelIndex index, selectedIndexes()) {
        item = model -> getItem(index);

        if (item -> isFolder()) {
            downloadBranch(item, savePath);
        } else {
            downloadItem(item, savePath);
            if (markAsLiked)
                item -> getState() -> setLiked();
        }
    }
}

//TODO: add copy func
void View::copyItemsFrom(View * /*otherView*/) {
//    QModelIndexList indexes = otherView -> selectedItems();

//    foreach(QModelIndex index, indexes) {
//        //TODO: copy logic
//    }
}

//////////////////////////////////////////////////////
/// PROTECTED
//////////////////////////////////////////////////////

ModelItem * View::activeItem(bool next) {
    ModelItem * item = 0;

    if (Player::instance() -> currentPlaylist() == this) {
        if (Player::instance() -> playedItem()) {
            item = Player::instance() -> playedItem();
        }
    }

    if (item == 0) {
        QModelIndexList list = selectedIndexes();

        if (list.count() > 0) {
            item = model -> getItem(list.first());

            if (!item -> isFolder()) {
                QModelIndex m;
                if (next) {
                    m = this -> indexAbove(list.first());
                } else { m = this -> indexBelow(list.first()); }

                if (m.isValid()) {
                   item = model -> getItem(m);
                } else {
                   item = model -> getItem(list.first().parent());
                }
            }
        } else {
            item = model -> getItem(this -> rootIndex());
        }
    }

    return item;
}
//// test needed / update need
//ModelItem * View::nextItem(QModelIndex currIndex) {
//    QModelIndex newIndex;
//    ModelItem * item;

//    while(true) {
//        newIndex = currIndex.parent().child(currIndex.row()+1, 0); //indexBelow(currIndex);

//        if (newIndex.isValid()) {
//            item = model -> getItem(newIndex);
//            qDebug() << item -> data(0);
//            if (!item->getState() -> isUnprocessed()) {
//                return item;
//            } else {
//                currIndex = newIndex.child(0, 0);
//            }
//        } else {
//            currIndex = currIndex.parent();
//            item = model -> getItem(newIndex);

//            if (!currIndex.isValid())
//                return model -> root();
//        }
//    }
//}

ModelItem * View::nextItem(ModelItem * curr) {
    ModelItem * item = curr;
    bool first_elem = curr -> parent() == 0 || curr -> isFolder();

    while(true) {
        if (first_elem) {
            first_elem = false;
        } else {
            item = item -> parent() -> child(item -> row() + 1);
        }

        if (item != 0) {
            if (item -> isPlayable()) {
                return item;
            } else {
                curr = item;
                item = curr -> child(0);
                first_elem = true;
            }
        } else {
            if (curr -> parent() == 0)
                return 0;

            item = curr;
            curr = curr -> parent();
        }
    }
}
ModelItem * View::prevItem(ModelItem * curr) {
    ModelItem * item = curr;
    bool last_elem = false;

    if (curr -> parent() == 0)
        return 0;

    while(true) {
        if (last_elem) {
            last_elem = false;
        } else {
            item = item -> parent() -> child(item -> row() - 1);
        }

        if (item != 0) {
            if (item -> isPlayable()) {
                return item;
            } else {
                curr = item;
                item = curr -> child(item -> childCount() - 1);
                last_elem = true;
            }
        } else {
            if (curr -> parent() == 0)
                return 0;

            item = curr;
            curr = curr -> parent();
        }
    }
}

void View::dragEnterEvent(QDragEnterEvent *event) {
    QTreeView::dragEnterEvent(event);
    event -> setDropAction(
                event -> source() == this ? Qt::MoveAction : Qt::CopyAction
    );

    if (event -> mimeData() -> hasFormat("text/uri-list"))
        event -> accept();
    else event -> ignore();
}

void View::dragMoveEvent(QDragMoveEvent * event) {
    QTreeView::dragMoveEvent(event);
    if (event -> mimeData() -> hasFormat("text/uri-list")) {
        event -> accept();
    } else
        event -> ignore();
}

void View::dropEvent(QDropEvent *event) {
    QTreeView::dropEvent(event);
}

void View::keyPressEvent(QKeyEvent *event) {
    if (event -> key() == Qt::Key_Enter || event -> key() == Qt::Key_Return) {
        QModelIndexList list = selectedIndexes();

        if (list.count() > 0) {
            ModelItem * item = model -> getItem(list.first());
            execItem(item);
        }
    } else if (event -> key() == Qt::Key_Delete) {
        QModelIndexList list = selectedIndexes();
        QModelIndex modelIndex;

        for(int i = list.count() - 1; i >= 0; i--) {
            modelIndex = list.at(i);
            removeItem(model -> getItem(modelIndex));
        }
    } else { QTreeView::keyPressEvent(event); }
}

void View::mousePressEvent(QMouseEvent *event) {
    if (event -> button() == Qt::LeftButton) {
        dragStartPoint = event -> pos();
    }

    QTreeView::mousePressEvent(event);
}

void View::mouseMoveEvent(QMouseEvent * event) {
    if ((event -> buttons() == Qt::LeftButton) && (dragStartPoint - event -> pos()).manhattanLength() >= 5){
        if (selectedIndexes().length() > 0) {
            QDrag * drag = new QDrag(this);
            QMimeData * mimeData = model -> mimeData(selectedIndexes());
            drag -> setPixmap(QPixmap(":drag"));
            drag -> setMimeData(mimeData);
            drag -> exec(Qt::CopyAction, Qt::CopyAction);
        }
    }

    QTreeView::mouseMoveEvent(event);
}
