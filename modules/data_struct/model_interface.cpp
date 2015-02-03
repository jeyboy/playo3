#include "model_interface.h"
#include <qdebug.h>

using namespace Playo3;

ModelInterface::ModelInterface(QJsonObject * hash, QObject * parent) : QAbstractItemModel(parent) { //TODO: rewrite
    if (hash != 0) {
        rootItem = new FolderItem(hash);
//        items_count = hash -> value(JSON_TYPE_TAB_ITEMS_COUNT).toInt();
    } else {
        rootItem = new FolderItem();
//        items_count = 0;
    }
}

ModelInterface::~ModelInterface() {
    delete rootItem;
}

QVariant ModelInterface::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    return item(index) -> data(role);
}

bool ModelInterface::setData(const QModelIndex & model_index, const QVariant &value, int role) {
    bool result = false;

    ItemInterface * node = item(model_index);

    if (role == Qt::CheckStateRole) {
        node -> updateCheckedState(!node -> is(ItemState::checked));

        if (node -> isContainer()) {
            FolderItem * it = dynamic_cast<FolderItem *>(node);
            if (it -> childCount() > 0) {
                emit dataChanged(model_index, index(it -> child(it -> childCount() - 1)));
                return true;
            }
        }

        result = true;
    }
    else if (role == ISTATE) {
        node -> setStates(value.toInt());
        result = true;
    } else if (role == Qt::EditRole)
        result = node -> setData(model_index.column(), value);

    if (result)
        emit dataChanged(model_index, model_index);

    return result;
}

QVariant ModelInterface::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem -> data(section);

    return QVariant();
}

bool ModelInterface::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

Qt::ItemFlags ModelInterface::flags(const QModelIndex & index) const {
    if (!index.isValid())
        return 0;

    Qt::ItemFlags fl = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);


    if (Settings::instance() -> isCheckboxShow())
        fl |= Qt::ItemIsUserCheckable;

    if (!index.data(IFOLDER).toBool())
        fl |= Qt::ItemNeverHasChildren;

    return fl;
}

ItemInterface * ModelInterface::item(const QModelIndex & index) const {
    if (index.isValid()) {
        ItemInterface * item = static_cast<ItemInterface *>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QModelIndex ModelInterface::index(ItemInterface * item) const {
    if (item == rootItem)
        return QModelIndex();
    else
        return createIndex(item -> row(), item -> column(), item);
}

QModelIndex ModelInterface::index(int row, int column, const QModelIndex & parent) const {
    if (parent.isValid() && parent.column() != 0) // !hasIndex(row, column, parent)
        return QModelIndex();

    FolderItem * parentItem = item<FolderItem>(parent);
    ItemInterface * childItem = parentItem -> child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool ModelInterface::insertColumns(int position, int columns, const QModelIndex & parent) {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
//    success = rootItem -> insertColumns(position, columns);
    success = true;
    endInsertColumns();

    return success;
}

//void ModelInterface::appendRow(ItemInterface * item) {
////    int position = parentItem -> childCount();
////    beginInsertRows(index(parentItem), position, position);
//    if (!item -> isFolder()) {
//        emit itemsCountChanged(++count);
//    }
////    endInsertRows();

////    emit dataChanged(parent, parent);
//}

//bool ModelInterface::insertRows(int position, int rows, const QModelIndex & parent) {
//    FolderItem * parentItem = item<FolderItem>(parent);
//    bool success = parentItem != 0;

//    if (success) {
//        beginInsertRows(parent, position, position + rows - 1);
//        success = parentItem -> insertChildren(position, rows, rootItem -> columnCount());
//        endInsertRows();
//    }

//    return success;
//}

QModelIndex ModelInterface::parent(const QModelIndex & index) const {
    if (!index.isValid())
        return QModelIndex();

    ItemInterface * childItem = item(index);
    ItemInterface * parentItem = childItem -> parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem -> row(), 0, parentItem);
}

bool ModelInterface::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
//    success = rootItem -> removeColumns(position, columns);
    success = true;
    endRemoveColumns();

    if (rootItem -> columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool ModelInterface::insertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent) {
    if (list.isEmpty()) return false;

    QModelIndex exIndex;
    int exRow;

    recalcParentIndex(parent, pos, exIndex, exRow, list.first());

    beginInsertRows(exIndex, exRow, exRow + (parent == exIndex ? list.length() - 1 : 0));
    dropProcession(parent, pos, list);
    endInsertRows();

    emit spoilNeeded(const_cast<QModelIndex &>(parent));
    return true;
}

bool ModelInterface::removeRows(int position, int rows, const QModelIndex &parent) {
    FolderItem * parentItem = item<FolderItem>(parent);
    bool success = parentItem != 0;

    if (success) {
        beginRemoveRows(parent, position, position + rows - 1);
        success = parentItem -> removeChildren(position, rows);
        endRemoveRows();
    }

    return success;
}

int ModelInterface::rowCount(const QModelIndex & parent) const {
    //        if (parent.column() > 0)
    //            return 0;

    FolderItem * parentItem = item<FolderItem>(parent);
    return parentItem ? parentItem -> childCount() : 0;
}

void ModelInterface::shuffle() {
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

void ModelInterface::expanded(const QModelIndex & index) {
    ItemInterface * node = item(index);
    node -> set(ItemState::expanded);
}
void ModelInterface::collapsed(const QModelIndex & index) {
    ItemInterface * node = item(index);
    node -> unset(ItemInterface::expanded);
}

/////////////////////////////////////////////////////////

QModelIndex ModelInterface::fromPath(QString /*path*/) { //TODO: rewrite
//    QStringList parts = path.split(' ', QString::SkipEmptyParts);
//    ModelItem * curr = getModel() -> root();
//    int level;

//    while(parts.length() > 0) {
//        level = parts.takeFirst().toInt();
//        curr = curr -> child(level);
//    }

//    return curr;
    return QModelIndex();
}

Qt::DropActions ModelInterface::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList ModelInterface::mimeTypes() const {
    QStringList types;
    types << DROP_INNER_FORMAT;
    types << DROP_OUTER_FORMAT;
    return types;
}

QMimeData * ModelInterface::mimeData(const QModelIndexList & indexes) const {
    if (indexes.count() <= 0)
        return 0;

    QMimeData * mimeData = new QMimeData();
    QList<QUrl> list;
    ItemInterface * temp;

     //TODO: folders not proceed correctly
    foreach (const QModelIndex & index, indexes) {
        if (index.isValid()) {
            temp = item(index);
            list.append(temp -> toUrl());
        }
    }

    mimeData -> setUrls(list);

    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);
    encodeData(indexes, stream);
    mimeData -> setData(DROP_INNER_FORMAT, encoded);

    return mimeData;
}

// row always equal to -1 // maybe need precalc on dnd drop in view
bool ModelInterface::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parentIndex) {
    if (!data || !(action == Qt::CopyAction || action == Qt::MoveAction))
        return false;

    int row_count = rowCount(parentIndex);

    if (dropKeyModifiers & Qt::ControlModifier)
        ExtensionDialog(QApplication::activeWindow()).exec();

    if (action == Qt::CopyAction) {
        if (data -> hasUrls()) {
            if (row > row_count) row = -1;
            return insertRows(data -> urls(), row, parentIndex);
        }
    } else {
        if (!data -> hasFormat(DROP_INNER_FORMAT))
            return false;

        if (row > row_count || row == -1)
            row = row_count;

        if (column == -1)
            column = 0;

        QByteArray encoded = data -> data(DROP_INNER_FORMAT);
        QDataStream stream(&encoded, QIODevice::ReadOnly);
        return decodeData(row, column, parentIndex, stream);
//        bool beginMoveRows(const QModelIndex &sourceParent, int sourceFirst, int sourceLast, const QModelIndex &destinationParent, int destinationRow);
//        void endMoveRows();
    }

    return false;
}
