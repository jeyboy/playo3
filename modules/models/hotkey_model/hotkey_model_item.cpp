#include "hotkey_model_item.h"

HotkeyModelItem::HotkeyModelItem(const QVector<QVariant> &data, HotkeyModelItem *parent) {
     parentItem = parent;
     itemData = data;
}

HotkeyModelItem::~HotkeyModelItem() {
    qDeleteAll(childItems);
}

HotkeyModelItem *HotkeyModelItem::child(int number) {
    return childItems.value(number);
}

int HotkeyModelItem::childCount() const {
    return childItems.count();
}

int HotkeyModelItem::childNumber() const {
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<HotkeyModelItem*>(this));

    return 0;
}

int HotkeyModelItem::columnCount() const {
    return itemData.count();
}

QVariant HotkeyModelItem::data(int column) const {
    return itemData.value(column);
}

bool HotkeyModelItem::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        HotkeyModelItem *item = new HotkeyModelItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool HotkeyModelItem::insertColumns(int position, int columns) {
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (HotkeyModelItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

HotkeyModelItem *HotkeyModelItem::parent() {
    return parentItem;
}

QList<HotkeyModelItem *> HotkeyModelItem::childList() {
    return childItems;
}

bool HotkeyModelItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool HotkeyModelItem::removeColumns(int position, int columns) {
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (HotkeyModelItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool HotkeyModelItem::setData(int column, const QVariant &value) {
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

void HotkeyModelItem::appendChilds(QList<HotkeyModelItem *> * newChilds) {
    childItems.append(*newChilds);
}
