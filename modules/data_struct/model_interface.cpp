#include "model_interface.h"

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

    ItemInterface * item;

    switch(role) {
        case Qt::DisplayRole: {
           item = getItem(index);
           return item -> data(/*TITLEID*/index.column());
        }
//        case Qt::DecorationRole: {
//        //QPixmap pixmap(26, 26);
//        //pixmap.fill(color);
//        //QIcon icon(pixmap);


//           item = getItem(index);

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
//                item = getItem(index);
//                return item -> getState() -> isChecked();
//            } else return QVariant();
//        }

//        case Qt::ToolTipRole:
//            item = getItem(index);
//            return item -> data(TITLEID).toString();
//        case Qt::SizeHintRole:
//            item = getItem(index);
//            if (item -> isFolder())
//                return QSize(0, Settings::instance() -> getItemHeight());
//            else
//                return QSize(0, Settings::instance() -> getTotalItemHeight());
//        case Qt::TextAlignmentRole:
//            item = getItem(index);
//            if (item -> isFolder() || !Settings::instance() -> isShowInfo())
//                return Qt::AlignVCenter;
//            else
//                return Qt::AlignLeft;
//        case Qt::FontRole:
//            return Settings::instance() -> getItemFont();
//        case EXTENSIONID:
//            item = getItem(index);
//            return item -> data(EXTENSIONID);
//        case ADDFONTID:
//            return Settings::instance() -> getItemInfoFont();
//        case Qt::UserRole:
//            item = getItem(index);
//            return item -> getState() -> currStateValue();
//        case PROGRESSID:
//            item = getItem(index);
//            return Download::instance() -> getProgress(item);
//        case INFOID:
//            item = getItem(index);
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

ItemInterface * ModelInterface::getItem(const QModelIndex & index) const {
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

QModelIndex ModelInterface::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0) // !hasIndex(row, column, parent)
        return QModelIndex();

    FolderItem * parentItem = getItem<FolderItem>(parent);
    ItemInterface * childItem = parentItem -> child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool ModelInterface::insertColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
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

bool ModelInterface::insertRows(int position, int rows, const QModelIndex & parent) {
    FolderItem * parentItem = getItem<FolderItem>(parent);
    bool success = parentItem != 0;

    if (success) {
        beginInsertRows(parent, position, position + rows - 1);
        success = parentItem -> insertChildren(position, rows, rootItem -> columnCount());
        endInsertRows();
    }

    return success;
}

QModelIndex ModelInterface::parent(const QModelIndex & index) const {
    if (!index.isValid())
        return QModelIndex();

    ItemInterface * childItem = getItem(index);
    ItemInterface * parentItem = childItem -> parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem -> row(), 0, parentItem);
}

bool ModelInterface::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem -> removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem -> columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool ModelInterface::removeRows(int position, int rows, const QModelIndex &parent) {
    FolderItem * parentItem = getItem<FolderItem>(parent);
    bool success = parentItem != 0;

    if (success) {
        beginRemoveRows(parent, position, position + rows - 1);
        success = parentItem -> removeChildren(position, rows);
        endRemoveRows();
    }

    return success;
}

int ModelInterface::rowCount(const QModelIndex &parent) const {
    //        if (parent.column() > 0)
    //            return 0;

    FolderItem * parentItem = getItem<FolderItem>(parent);
    return parentItem ? parentItem -> childCount() : 0;
}

bool ModelInterface::setData(const QModelIndex &index, const QVariant &value, int role) {
    //        ModelItem * item;

    //        if (role == Qt::CheckStateRole) {
    //            item = getItem(index);

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

    //        item = getItem(index);
    //        bool result = item -> setData(index.column(), value);

    //        if (result)
    //            emit dataChanged(index, index);

    //        return result;



    if (role != Qt::EditRole)
        return false;

    ItemInterface * item = getItem(index);
    bool result = item -> setData(index.column(), value);

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
//        ModelItem * parentItem = getItem(parentIndex);
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
//        ModelItem *parentItem = getItem(parent);
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

//    /////////////////////////////////////////////////////////

//    ModelItem * Model::buildPath(QString path) {
//        QStringList list = path.split('/', QString::SkipEmptyParts);
//        ModelItem * curr = rootItem;

//        foreach(QString piece, list) {
//            curr = addFolder(piece, curr);
//        }

//        return curr;
//    }

//    bool Model::isFolderExist(QString folderName, ModelItem * parent) {
//        return parent -> foldersList() -> contains(folderName);
//    }

//    //TODO: improve model insertion (add emit of rows insertion)


//    ModelItem * Model::addFolder(QString folderPath, QString folderName, ModelItem * parent, QString remoteID) {
//        ModelItem * curr = parent;

//        if (isFolderExist(folderName, curr)) {
//            curr = curr -> foldersList() -> value(folderName);
//        } else {
//            if (!remoteID.isEmpty())
//                curr = new VkFolder(folderPath, remoteID, folderName, curr);
//            else
//                curr = new FolderItem(folderPath, folderName, curr);
//        }

//        return curr;
//    }

//    ModelItem * Model::addFolder(QString folderPath, ModelItem * parent, QString remoteID) {
//        return addFolder(folderPath, folderPath, parent, remoteID);
//    }

//    //////////////////////// slots //////////////////////////

//    void Model::expanded(const QModelIndex &index) {
//        ModelItem * item = getItem(index);
//        item -> getState() -> setExpanded();
//    }
//    void Model::collapsed(const QModelIndex &index) {
//        ModelItem * item = getItem(index);
//        item -> getState() -> unsetExpanded();
//    }

//    /////////////////////////////////////////////////////////

//    Qt::DropActions Model::supportedDropActions() const {
//        return Qt::CopyAction | Qt::MoveAction;
//    }

//    QStringList Model::mimeTypes() const {
//        QStringList types;
//        types << "text/uri-list";
//        return types;
//    }

//    QMimeData * Model::mimeData(const QModelIndexList &indexes) const {
//        QMimeData *mimeData = new QMimeData();
//        QList<QUrl> list;
//        ModelItem * temp;

//        foreach (const QModelIndex &index, indexes) {
//            if (index.isValid()) {
//                temp = getItem(index);
//                list.append(temp -> toUrl());
//            }
//        }

//        mimeData -> setUrls(list);
//        return mimeData;
//    }

//    bool Model::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parentIndex) {
//    //    if (action == Qt::CopyAction) {

//    //    } else {

//    //    }

//        if (data -> hasUrls()) {
//            ExtensionDialog(QApplication::activeWindow()).exec();
//            //TODO: add parentIndex to the dropProcession
//            QModelIndex modelIndex = dropProcession(data -> urls());
//            refresh();
//            emit spoilNeeded(modelIndex);
//        }

//        return true;
//    }

//    ModelItem * Model::createItem(QString path, ModelItem * parent) {
//       return (new FileItem(path, parent)) -> toModelItem();
//    }
