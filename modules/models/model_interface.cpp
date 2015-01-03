#include "model_interface.h"

using namespace Playo;

ModelInterface::ModelInterface(const QStringList &headers, const QString &data, QObject *parent)
    : QAbstractItemModel(parent) {
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new ItemInterface(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

ModelInterface::~ModelInterface() {
    delete rootItem;
}

int ModelInterface::columnCount(const QModelIndex & /* parent */) const {
    return rootItem -> columnCount();
}

QVariant ModelInterface::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    ItemInterface *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags ModelInterface::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

ItemInterface * ModelInterface::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        ItemInterface * item = static_cast<ItemInterface *>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant ModelInterface::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex ModelInterface::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    ItemInterface * parentItem = getItem(parent);
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

bool ModelInterface::insertRows(int position, int rows, const QModelIndex &parent) {
    ItemInterface * parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem -> insertChildren(position, rows, rootItem -> columnCount());
    endInsertRows();

    return success;
}

QModelIndex ModelInterface::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    ItemInterface * childItem = getItem(index);
    ItemInterface * parentItem = childItem -> parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem -> childNumber(), 0, parentItem);
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
    ItemInterface * parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem -> removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int ModelInterface::rowCount(const QModelIndex &parent) const {
    ItemInterface * parentItem = getItem(parent);

    return parentItem -> childCount();
}

bool ModelInterface::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole)
        return false;

    ItemInterface *item = getItem(index);
    bool result = item -> setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool ModelInterface::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem -> setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void ModelInterface::setupModelData(const QStringList &lines, ItemInterface * parent) {
    QList<ItemInterface *> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            ItemInterface *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }
}

