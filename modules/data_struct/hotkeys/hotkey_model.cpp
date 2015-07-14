#include "hotkey_model.h"

HotkeyModel::HotkeyModel(QList<HotkeyModelItem * > * toplevelchilds, QObject * parent) : QAbstractItemModel(parent) {
    QVector<QVariant> rootData;
    rootData << "" << "";
    rootItem = new HotkeyModelItem(rootData);
    rootItem -> appendChilds(toplevelchilds);
}

QVariant HotkeyModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    HotkeyModelItem * item;

    if (role == Qt::UserRole) {
        item = getItem(index);
        return item -> data(2);
    } else if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    item = getItem(index);
    return item -> data(index.column());
}

Qt::ItemFlags HotkeyModel::flags(const QModelIndex & index) const {
    if (!index.isValid())
        return 0;

    if (index.column() > 0)
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

HotkeyModelItem * HotkeyModel::getItem(const QModelIndex & index) const {
    if (index.isValid()) {
        HotkeyModelItem * item = static_cast<HotkeyModelItem * >(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

QVariant HotkeyModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem -> data(section);

    return QVariant();
}

QModelIndex HotkeyModel::index(int row, int column, const QModelIndex & parent) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    HotkeyModelItem * parentItem = getItem(parent);

    HotkeyModelItem * childItem = parentItem -> child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex HotkeyModel::parent(const QModelIndex & index) const {
    if (!index.isValid())
        return QModelIndex();

    HotkeyModelItem * childItem = getItem(index);
    HotkeyModelItem * parentItem = childItem -> parent();

    if (parentItem == rootItem || parentItem == 0)
        return QModelIndex();

    return createIndex(parentItem -> childNumber(), 0, parentItem);
}

int HotkeyModel::rowCount(const QModelIndex & parent) const {
    HotkeyModelItem * parentItem = getItem(parent);
    return parentItem -> childCount();
}

bool HotkeyModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (role != Qt::EditRole)
        return false;

    HotkeyModelItem * item = getItem(index);
    bool result = item -> setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool HotkeyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}
