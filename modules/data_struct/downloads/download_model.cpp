#include "download_model.h"
#include <QDebug>

QVariant DownloadModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    DownloadModelItem * node;

    if (role == Qt::SizeHintRole) {
        return QSize(0, 24);
    } else {
        node = item(index);

        if (role == Qt::EditRole)
            return node -> data(index.column());
        else
            return node -> data(role);
    }
}

Qt::ItemFlags DownloadModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;

    if (index.column() > 0)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

DownloadModelItem * DownloadModel::item(const QModelIndex & index) const {
    if (index.isValid()) {
        DownloadModelItem * node = static_cast<DownloadModelItem *>(index.internalPointer());
        if (node) return node;
    }
    return rootItem;
}

DownloadModelItem * DownloadModel::root() const {
    return rootItem;
}

QList<DownloadModelItem *> DownloadModel::toplevelItems() {
    return root() -> childList();
}

QVariant DownloadModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem -> data(section);

    return QVariant();
}

QModelIndex DownloadModel::index(DownloadModelItem * node) const {
    if (node == rootItem)
        return QModelIndex();
    else
        return createIndex(node -> row(), node -> column(), node);
}

QModelIndex DownloadModel::index(int row, int column, const QModelIndex & parent) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    DownloadModelItem * parentItem = item(parent);

    DownloadModelItem * childItem = parentItem -> child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DownloadModel::parent(const QModelIndex & index) const {
    if (!index.isValid())
        return QModelIndex();

    DownloadModelItem * childItem = item(index);
    DownloadModelItem * parentItem = childItem -> parent();

    if (parentItem == rootItem || parentItem == 0)
        return QModelIndex();

    return createIndex(parentItem -> row(), 0, parentItem);
}

//bool HotkeyModel::removeColumns(int position, int columns, const QModelIndex &parent) {
//    bool success;

//    beginRemoveColumns(parent, position, position + columns - 1);
//    success = rootItem->removeColumns(position, columns);
//    endRemoveColumns();

//    if (rootItem->columnCount() == 0)
//        removeRows(0, rowCount());

//    return success;
//}

//bool HotkeyModel::removeRows(int position, int rows, const QModelIndex &parent) {
//    DownloadModelItem *parentItem = getItem(parent);
//    bool success = true;

//    beginRemoveRows(parent, position, position + rows - 1);
//    success = parentItem->removeChildren(position, rows);
//    endRemoveRows();

//    return success;
//}

int DownloadModel::rowCount(const QModelIndex & parent) const {
    DownloadModelItem * parentItem = item(parent);
    return parentItem -> childCount();
}

QModelIndex DownloadModel::appendRow(const QVariantMap & data) {
    DownloadModelItem * newItem;
    beginInsertRows(QModelIndex(), rootItem -> childCount(), rootItem -> childCount());
    newItem = new DownloadModelItem(data, rootItem);
    endInsertRows();
    return index(newItem);
}

bool DownloadModel::removeRows(int position, int rows, const QModelIndex & parent) {
    DownloadModelItem * parentItem = item(parent);
    bool success = parentItem != 0;

    if (success) {
        beginRemoveRows(parent, position, position + rows - 1);
        bool blockSignal = signalsBlocked();
        blockSignals(true);
        success = parentItem -> removeChildren(position, rows);
        blockSignals(blockSignal);
        endRemoveRows();
    }

    return success;
}

bool DownloadModel::moveRows(const QModelIndex & sourceParent, int sourceRow, int count,
                      const QModelIndex &destinationParent, int destinationChild) {
    beginMoveRows(sourceParent, sourceRow, sourceRow + count, destinationParent, destinationChild);
    for(int loop1 = 0; loop1 < count; loop1++)
        rootItem -> childList().move(sourceRow, destinationChild);
    endMoveRows();
    return true;
}

bool DownloadModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    DownloadModelItem * node = item(index);
    bool result;

    if (role == Qt::EditRole)
        result = node -> setData(index.column(), value);
    else
        result = node -> setData(role, value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool DownloadModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant & value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}
