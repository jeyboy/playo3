#include "model_interface.h"

using namespace Playo3;

IModel::IModel(QJsonObject * hash, QObject * parent) : QAbstractItemModel(parent), addWatcher(0) { //TODO: rewrite
    sync = new QMutex(QMutex::NonRecursive);
    if (hash != 0) {
        rootItem = new FolderItem(hash);
//        items_count = hash -> value(JSON_TYPE_TAB_ITEMS_COUNT).toInt();
    } else {
        rootItem = new FolderItem();
//        items_count = 0;
    }

    qDebug() << this << " " << rootItem -> itemsCountInBranch();
}

IModel::~IModel() {
    delete rootItem;
    delete addWatcher;
    delete sync;
}

QVariant IModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    return item(index) -> data(role);
}

bool IModel::setData(const QModelIndex & model_index, const QVariant & value, int role) {
    bool result = false;

    IItem * node = item(model_index);

    if (role == Qt::CheckStateRole) {
        bool checked = !node -> is(ItemState::checked);

        if (node -> isContainer()) {
            FolderItem * it = dynamic_cast<FolderItem *>(node);
            it -> propagateCheckedState(checked);
            emit dataChanged(model_index, index(it -> child(it -> childCount() - 1)));
            return true;
        }
        else node -> updateCheckedState(checked);

        result = true;
    } else if (role == ISTATERESTORE) {
        int iState = value.toInt();
        node -> setStates(iState);
        result = iState != -ItemState::proceeded && iState != -ItemState::mark_on_removing;
    } else if (role == ISTATE) {
        Library::instance() -> setItemState(model_index, value.toInt());
        node -> setStates(value.toInt());
        result = true;
    } else if (role == Qt::EditRole)
        result = node -> setData(model_index.column(), value);

    if (result)
        emit dataChanged(model_index, model_index);

    return result;
}

QVariant IModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem -> data(section);

    return QVariant();
}

bool IModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

Qt::ItemFlags IModel::flags(const QModelIndex & index) const {
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    Qt::ItemFlags fl = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);


    if (Settings::instance() -> isCheckboxShow())
        fl |= Qt::ItemIsUserCheckable;

    if (!index.data(IFOLDER).toBool())
        fl |= Qt::ItemNeverHasChildren;

    return fl;
}

IItem * IModel::item(const QModelIndex & index) const {
    if (index.isValid()) {
        IItem * item = static_cast<IItem *>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QModelIndex IModel::index(IItem * item) const {
    if (item == rootItem)
        return QModelIndex();
    else
        return createIndex(item -> row(), item -> column(), item);
}

QModelIndex IModel::index(int row, int column, const QModelIndex & parent, bool orLastChild) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    FolderItem * parentItem = item<FolderItem>(parent);
    IItem * childItem = parentItem -> child(row);
    if (orLastChild && !childItem)
        childItem = parentItem -> child(parentItem -> childCount() - 1);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
bool IModel::insertColumns(int position, int columns, const QModelIndex & parent) {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
//    success = rootItem -> insertColumns(position, columns);
    success = true;
    endInsertColumns();

    return success;
}

//void IModel::appendRow(IItem * item) {
////    int position = parentItem -> childCount();
////    beginInsertRows(index(parentItem), position, position);
//    if (!item -> isFolder()) {
//        emit itemsCountChanged(++count);
//    }
////    endInsertRows();

////    emit dataChanged(parent, parent);
//}

//bool IModel::insertRows(int position, int rows, const QModelIndex & parent) {
//    FolderItem * parentItem = item<FolderItem>(parent);
//    bool success = parentItem != 0;

//    if (success) {
//        beginInsertRows(parent, position, position + rows - 1);
//        success = parentItem -> insertChildren(position, rows, rootItem -> columnCount());
//        endInsertRows();
//    }

//    return success;
//}

QModelIndex IModel::parent(const QModelIndex & index) const {
    if (!index.isValid())
        return QModelIndex();

    IItem * childItem = item(index);
    IItem * parentItem = childItem -> parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem -> row(), 0, parentItem);
}

bool IModel::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
//    success = rootItem -> removeColumns(position, columns);
    success = true;
    endRemoveColumns();

    if (rootItem -> columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

DropData * IModel::threadlyProcessingRowsInsertion(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    if (list.isEmpty()) return 0;

    emit moveInProcess();

    DropData * res = new DropData();
    recalcParentIndex(parent, pos, res -> eIndex, res -> eRow, list.first());
    res -> limitRow = res -> eRow + (parent == res -> eIndex ? list.length() - 1 : 0);
    dropProcession(parent, pos, list);
    return res;
}

bool IModel::threadlyInsertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    addWatcher = new QFutureWatcher<DropData *>();
    connect(addWatcher, SIGNAL(finished()), this, SLOT(finishingItemsAdding()));
    addWatcher -> setFuture(QtConcurrent::run(this, &IModel::threadlyProcessingRowsInsertion, list, pos, parent));
    return true;
}

bool IModel::insertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    if (list.isEmpty()) return false;

    QModelIndex exIndex;
    int exRow;

    recalcParentIndex(parent, pos, exIndex, exRow, list.first());
    beginInsertRows(exIndex, exRow, exRow + (parent == exIndex ? list.length() - 1 : 0));
    dropProcession(parent, pos, list);
    endInsertRows();
    emit spoilNeeded(parent);
    return true;
}

bool IModel::removeRows(int position, int rows, const QModelIndex & parent) {
    FolderItem * parentItem = item<FolderItem>(parent);
    bool success = parentItem != 0;
    int deleted;

    if (success) {
        if (parentItem -> parent() && position == 0 && parentItem -> childCount() == rows) { // remove empty parents for current deletion
            rows = 1;

            while(true) {
                position = parentItem -> parent() -> childRow(parentItem);
                parentItem = parentItem -> parent();

                if (!parentItem -> parent() || parentItem -> childCount() != 1) break;
            }

            (const_cast<QModelIndex &>(parent)) = index(parentItem);
        }

        beginRemoveRows(parent, position, position + rows - 1);
        bool blockSignal = signalsBlocked();
        blockSignals(true);
        deleted = parentItem -> removeChildren(position, rows);
        blockSignals(blockSignal);
        endRemoveRows();

        success = deleted != 1;

        if (deleted < 0)
            emit itemsCountChanged(deleted);
    }

    return success;
}

int IModel::rowCount(const QModelIndex & parent) const {
    //        if (parent.column() > 0)
    //            return 0;

    FolderItem * parentItem = item<FolderItem>(parent);
    return parentItem ? parentItem -> childCount() : 0;
}

void IModel::shuffle() {
    beginResetModel();
    rootItem -> shuffle();
    endResetModel();
}

//    bool Model::removeRow(int row, const QModelIndex &parentIndex) {
//        int removeCount = 1;
//        ModelItem * parentItem = item(parentIndex);
//        ModelItem * item = parentItem -> child(row);
//        QString folderName;
//        bool isUnprocessed = item -> isFolder();
//        bool res;

//        if (isUnprocessed) {
//            folderName = item -> data(TITLEID).toString();

//            removeCount = item -> childTreeCount();
//        }

//        beginRemoveRows(parentIndex, row, row);
//        res = parentItem -> removeChildren(row, 1);
//        endRemoveRows();

//        if (res) {
//            if (isUnprocessed) {
//                parentItem -> removeFolder(folderName);
//            }

//            if (removeCount > 0)
//                emit itemsCountChanged(count -= removeCount);
//        }

//        return res;
//    }

//    bool Model::removeRows(int position, int rows, const QModelIndex &parent) {
//        ModelItem *parentItem = item(parent);
//        bool success = true;

//        beginRemoveRows(parent, position, position + rows - 1);
//        success = parentItem -> removeChildren(position, rows);
//        endRemoveRows();

//        if (success)
//            emit itemsCountChanged((count -= rows));

//        return success;
//    }

//    void Model::clearAll(bool refresh) {
//        if(hasChildren()) {
//            emit updated();
//            removeRows(0, rowCount());
//            count = 0;
//            root() -> foldersList() -> clear();
//        }

//        if (refresh)
//            this -> refresh();
//    }


//    // no idea how to realize this correctly
//    void Model::libraryResponse() {
//        QFutureWatcher<ModelItem *> * obj = (QFutureWatcher<ModelItem *> *)sender();

//        if (obj -> result() != 0)
//            refreshItem(obj -> result());

//        obj -> deleteLater();
//    }

//    void Model::refresh() {
//        beginResetModel();
//        endResetModel();
//    //    emit itemsCountChanged(count);
//    }

//    void Model::refreshItem(ModelItem * item) {
//        qDebug() << item -> fullPath();
//        QModelIndex ind = index(item);
//        if (ind.isValid()) {
//    //        emit dataChanged(ind.parent(), ind);
//            emit dataChanged(ind, ind);
//        }
//    }

//    // remove temp item - folder
//    void Model::removeFolderPrebuild(ModelItem * temp) {
//        temp -> parent() -> removeFolder(temp -> data(TITLEID).toString());
//        temp -> parent() -> removeChildren(temp -> row(), 1);
//    }

//////////////////////// slots //////////////////////////

void IModel::copyTitleToClipboard(const QModelIndex & index) {
    if (index.isValid())
        QApplication::clipboard() -> setText(index.data().toString());
}

void IModel::expandeAll() {
    rootItem -> propagateFolderSetFlag(ItemState::expanded);
}

void IModel::expanded(const QModelIndex & index) {
    IItem * node = item(index);
    node -> set(ItemState::expanded);
}

void IModel::collapseAll() {
    rootItem -> propagateFolderUnsetFlag(ItemState::expanded);
}

void IModel::collapsed(const QModelIndex & index) {
    IItem * node = item(index);
    node -> unset(IItem::expanded);
}

void IModel::finishingItemsAdding() {
    DropData * res = addWatcher -> result();
    delete addWatcher;
    addWatcher = 0;
    if (!res) return;


    beginInsertRows(res -> eIndex, res -> eRow, res -> limitRow);
    endInsertRows();

    emit moveOutProcess();
    emit spoilNeeded(res -> eIndex);
}

/////////////////////////////////////////////////////////

void IModel::recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl /*url*/) {
    exIndex = dIndex;
    exRow = dRow < 0 ? 0 : dRow;
}

QModelIndex IModel::fromPath(QString path) {
    QStringList parts = path.split(' ', QString::SkipEmptyParts);

    if (parts.isEmpty())
        return QModelIndex();

    FolderItem * curr = rootItem;

    while(parts.length() > 1) {
        curr = dynamic_cast<FolderItem *>(curr -> child(parts.takeFirst().toInt()));
        if (curr == 0) return QModelIndex(); // while not fixed correct played item removing
    }

    IItem * res = curr -> child(parts.takeFirst().toInt());
    return res ? index(res) : QModelIndex();
}

Qt::DropActions IModel::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList IModel::mimeTypes() const {
    QStringList types;
    types << DROP_INNER_FORMAT;
    types << DROP_OUTER_FORMAT;
    return types;
}

bool IModel::decodeInnerData(int row, int /*column*/, const QModelIndex & parent, QDataStream & stream) {
    int totalAdded = 0;
    QModelIndex dIndex;
    InnerData * data;
    FolderItem * parentFolder;
    QHash<FolderItem *, int> counts;
    bool containPath, isRemote, requirePath = !isRelative();

    while (!stream.atEnd()) {
        data = new InnerData();
        stream >> data -> url >> isRemote >> data -> attrs;

        if (!isRemote) {
            containPath = data -> attrs.contains(JSON_TYPE_PATH);

            if (requirePath) {
                if (!containPath)
                    data -> attrs.insert(JSON_TYPE_PATH, data -> url.toLocalFile().section('/', 0, -2));
            } else {
                if (containPath)
                    data -> attrs.remove(JSON_TYPE_PATH);
            }
        }
        else data -> url = REMOTE_DND_URL;

        data -> dRow = row;
        dIndex = parent;

        recalcParentIndex(dIndex, data -> dRow, data -> eIndex, data -> eRow, data -> url);

        parentFolder = item<FolderItem>(dIndex);

        beginInsertRows(data -> eIndex, data -> eRow, data -> eRow);
        switch(data -> attrs.take(JSON_TYPE_ITEM_TYPE).toInt()) {
            case ITEM: {
                counts[parentFolder]++;
                new FileItem(data -> attrs, parentFolder, data -> dRow);
                break;
            }
            case VK_ITEM: {
                counts[parentFolder]++;
                new VkItem(data -> attrs, parentFolder, data -> dRow);
                break;
            }
            case SOUNDCLOUD_ITEM: {
                counts[parentFolder]++;
                new SoundcloudItem(data -> attrs, parentFolder, data -> dRow);
                break;
            }
//                case CUE_ITEM: {
//                    added++;
//                    new CueItem(data -> attrs, *p_item, data -> dRow);
//                    break;
//                }

            default: {
                qDebug() << "ITEM TYPE NOT SUPPORTED YET";
            }
        }
        endInsertRows();
        delete data;
    }

    QHash<FolderItem *, int>::Iterator it = counts.begin();

    for(; it != counts.end(); it++) {
        it.key() -> backPropagateItemsCountInBranch(it.value());
        totalAdded += it.value();
    }

    if (totalAdded > 0) emit itemsCountChanged(totalAdded);

    emit spoilNeeded(parent);
    return true;
}

QMimeData * IModel::mimeData(const QModelIndexList & indexes) const {
    if (indexes.isEmpty())
        return 0;

    QMimeData * mimeData = new QMimeData();
    QHash<QUrl, int> list;

    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);

    QModelIndexList::ConstIterator it = indexes.begin();
    for (; it != indexes.end(); ++it)
        item((*it)) -> packToStream(list, stream);

    mimeData -> setData(DROP_INNER_FORMAT, encoded);
    mimeData -> setUrls(list.keys());

    return mimeData;
}

//Если бросили точно на ноду - в row и column -1 а index - нода.
//Если выше ноды - index - родитель ноды а row = нода.row.
//Если ниже ноды - index - родитель ноды а row = нода.row + 1.
//Ежели бросили мимо - index = root.
bool IModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parentIndex) {
    if (!data || !(action == Qt::CopyAction || action == Qt::MoveAction))
        return false;

    if (row == -1 && !parentIndex.data(IFOLDER).toBool()) {
        row = parentIndex.row();
        (const_cast<QModelIndex &>(parentIndex)) = parentIndex.parent();
    }

    int row_count = rowCount(parentIndex);
    if (row > row_count) row = -1;

    if (dropKeyModifiers & Qt::ControlModifier)
        ExtensionDialog(QApplication::activeWindow()).exec();

    if (data -> hasFormat(DROP_INNER_FORMAT)) {
        //        if (column == -1)
        //            column = 0;

        QByteArray encoded = data -> data(DROP_INNER_FORMAT);
        QDataStream stream(&encoded, QIODevice::ReadOnly);
        return decodeInnerData(row, column, parentIndex, stream);
    } else if (data -> hasUrls())
        return /*insertRows*/threadlyInsertRows(data -> urls(), row, parentIndex);

    return false;
}

void IModel::initiateSearch(QString predicate, FolderItem * destination, FolderItem * search_source) {
    if (search_source == 0)
        search_source = rootItem;


}
