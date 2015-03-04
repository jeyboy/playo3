#include "download_model_item.h"

DownloadModelItem::DownloadModelItem(const QVector<QVariant> & data, DownloadModelItem * parent) {
     parentItem = parent;
     itemData = data;
}

DownloadModelItem::~DownloadModelItem() {
    qDeleteAll(childItems);
}

DownloadModelItem * DownloadModelItem::child(int number) {
    return childItems.value(number);
}

int DownloadModelItem::childCount() const {
    return childItems.count();
}

int DownloadModelItem::childNumber() const {
    if (parentItem)
        return parentItem -> childItems.indexOf(const_cast<DownloadModelItem *>(this));

    return 0;
}

int DownloadModelItem::columnCount() const {
    return itemData.count();
}

QVariant DownloadModelItem::data(int column) const {
    return itemData.value(column);
}

bool DownloadModelItem::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        DownloadModelItem * item = new DownloadModelItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

//bool DownloadModelItem::insertColumns(int position, int columns) {
//    if (position < 0 || position > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.insert(position, QVariant());

//    foreach (DownloadModelItem * child, childItems)
//        child -> insertColumns(position, columns);

//    return true;
//}

DownloadModelItem * DownloadModelItem::parent() {
    return parentItem;
}

QList<DownloadModelItem *> DownloadModelItem::childList() {
    return childItems;
}

bool DownloadModelItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

//bool DownloadModelItem::removeColumns(int position, int columns) {
//    if (position < 0 || position + columns > itemData.size())
//        return false;

//    for (int column = 0; column < columns; ++column)
//        itemData.remove(position);

//    foreach (DownloadModelItem * child, childItems)
//        child -> removeColumns(position, columns);

//    return true;
//}

bool DownloadModelItem::setData(int column, const QVariant & value) {
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

void DownloadModelItem::appendChilds(QList<DownloadModelItem *> * newChilds) {
    childItems.append(*newChilds);
}
