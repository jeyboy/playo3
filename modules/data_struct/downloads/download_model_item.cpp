#include "download_model_item.h"

DownloadModelItem::DownloadModelItem(const QVariantMap & data, DownloadModelItem * parent) {
     parentItem = parent;
     itemData = data;

     if (parent)
         parent -> childItems.append(parent);
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
    return 1;
}

QVariant DownloadModelItem::data(int column) const {
    return itemData.value(QString::number(column));
}

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

bool DownloadModelItem::setData(int column, const QVariant & value) {
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[QString::number(column)] = value;
    return true;
}
