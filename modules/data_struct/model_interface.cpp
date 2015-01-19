#include "model_interface.h"
#include <qdebug.h>

using namespace Playo3;

ModelInterface::ModelInterface(QJsonObject * hash, QObject * parent) : QAbstractItemModel(parent) {
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

QVariant ModelInterface::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    switch(role) {
        case Qt::DisplayRole:
        case FOLDERID:
            return item(index) -> data(role);

//        case Qt::DecorationRole: {
//        //QPixmap pixmap(26, 26);
//        //pixmap.fill(color);
//        //QIcon icon(pixmap);


//        ItemInterface * item;
//           item = item(index);

//           if (item -> getState() -> isNotExist()) {
//               return IconProvider::missedIcon();
//           }
//           else if (item -> isFolder())
//               return QVariant();
//    //               return IconProvider::fileIcon("", "");
//           else
//               return IconProvider::fileIcon(item -> fullPath(), (item -> data(EXTENSIONID).toString()));
//        }
//        case Qt::CheckStateRole: {
//            if (Settings::instance() -> isCheckboxShow()) {
//                item = item(index);
//                return item -> getState() -> isChecked();
//            } else return QVariant();
//        }

//        case Qt::ToolTipRole:
//            item = item(index);
//            return item -> data(TITLEID).toString();
//        case Qt::SizeHintRole:
//            item = item(index);
//            if (item -> isFolder())
//                return QSize(0, Settings::instance() -> itemHeight());
//            else
//                return QSize(0, Settings::instance() -> getTotalItemHeight());
//        case Qt::TextAlignmentRole:
//            item = item(index);
//            if (item -> isFolder() || !Settings::instance() -> isShowInfo())
//                return Qt::AlignVCenter;
//            else
//                return Qt::AlignLeft;
//        case Qt::FontRole:
//            return Settings::instance() -> itemFont();
//        case EXTENSIONID:
//            item = item(index);
//            return item -> data(EXTENSIONID);
//        case ADDFONTID:
//            return Settings::instance() -> itemInfoFont();
//        case Qt::UserRole:
//            item = item(index);
//            return item -> getState() -> currStateValue();
//        case PROGRESSID:
//            item = item(index);
//            return Download::instance() -> getProgress(item);
//        case INFOID:
//            item = item(index);
//            return QVariant(item -> getInfo());

        default: return QVariant();
    }
}

Qt::ItemFlags ModelInterface::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;

    //         if (Settings::instance() -> isCheckboxShow())
    //            return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsSelectable | QAbstractItemModel::flags(index);
    //         else
    //            return Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | QAbstractItemModel::flags(index);

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

ItemInterface * ModelInterface::item(const QModelIndex & index) const {
    if (index.isValid()) {
        ItemInterface * item = static_cast<ItemInterface *>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant ModelInterface::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem -> data(section);

    return QVariant();
}

QModelIndex ModelInterface::index(ItemInterface * item) const {
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
    beginInsertRows(parent, pos, pos + list.length() - 1);
    QModelIndex node = dropProcession(parent, pos, list);
    endInsertRows();
    emit spoilNeeded(node);
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

int ModelInterface::rowCount(const QModelIndex  &parent) const {
    //        if (parent.column() > 0)
    //            return 0;

    FolderItem * parentItem = item<FolderItem>(parent);
    return parentItem ? parentItem -> childCount() : 0;
}

bool ModelInterface::setData(const QModelIndex &index, const QVariant &value, int role) {
    //        ModelItem * item;

    //        if (role == Qt::CheckStateRole) {
    //            item = item(index);

    //            item -> changeCheckedState(!item -> getState() -> isChecked());

    //            if (item -> isFolder() && item -> childCount() > 0) {
    //                emit dataChanged(index, this -> index(item -> child(item -> childCount() - 1)));
    //            } else {
    //                emit dataChanged(index, index);
    //            }
    //            return true;
    //        }

    //        if (role != Qt::EditRole)
    //            return false;

    //        item = item(index);
    //        bool result = item -> setData(index.column(), value);

    //        if (result)
    //            emit dataChanged(index, index);

    //        return result;



    if (role != Qt::EditRole)
        return false;

    ItemInterface * node = item(index);
    bool result = node -> setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool ModelInterface::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
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

Qt::DropActions ModelInterface::supportedDropActions() const {
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList ModelInterface::mimeTypes() const {
    QStringList types;
    types << DROP_INNER_FORMAT;
    types << DROP_OUTER_FORMAT;
    return types;
}

QMimeData * ModelInterface::mimeData(const QModelIndexList & indexes) const { //TODO: need to additionaly add inner format
//    if (indexes.count() <= 0)
//   return 0;
//   QStringList types = mimeTypes();
//   if (types.isEmpty())
//   return 0;
//   QMimeData *data = new QMimeData();
//   QString format = types.at(0);
//   QByteArray encoded;
//   QDataStream stream(&encoded, QIODevice::WriteOnly);
//   encodeData(indexes, stream);
//   data->setData(format, encoded);
//   return data;


    QMimeData * mimeData = new QMimeData();
    QList<QUrl> list;
    ItemInterface * temp;

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

bool ModelInterface::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parentIndex) {
    if (!data || !(action == Qt::CopyAction || action == Qt::MoveAction))
        return false;

    if (action == Qt::CopyAction) {
        if (data -> hasUrls()) {
            if (dropKeyModifiers & Qt::ControlModifier)
                ExtensionDialog(QApplication::activeWindow()).exec();

            if (row < 0) row = rowCount(parentIndex);
            return insertRows(data -> urls(), row, parentIndex);
        }
    } else {
        if (!data -> hasFormat(DROP_INNER_FORMAT))
            return false;

        int row_count = rowCount(parentIndex);
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
