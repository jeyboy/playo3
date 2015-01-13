#include "item_interface.h"
//#include <qstringlist.h>

using namespace Playo3;

ItemInterface::ItemInterface(const QVector<QVariant> &data, ItemInterface * parent) {
    parentItem = parent;
    itemData = data;
}

ItemInterface::~ItemInterface() {
    qDeleteAll(childItems);
}

int ItemInterface::childNumber() const {
    if (parentItem)
        return parentItem -> childItems.indexOf(const_cast<ItemInterface *>(this));

    return 0;
}

bool ItemInterface::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        ItemInterface * item = new ItemInterface(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool ItemInterface::insertColumns(int position, int columns) {
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (ItemInterface * child, childItems)
        child -> insertColumns(position, columns);

    return true;
}

bool ItemInterface::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool ItemInterface::removeColumns(int position, int columns) {
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (ItemInterface * child, childItems)
        child -> removeColumns(position, columns);

    return true;
}

bool ItemInterface::setData(int column, const QVariant &value) {
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
