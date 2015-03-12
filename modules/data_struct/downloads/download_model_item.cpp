#include "download_model_item.h"
#include <qdebug.h>

DownloadModelItem::DownloadModelItem(const QVariantMap & data, DownloadModelItem * parent) {
    parentItem = parent;
    itemData = data;

    if (parent)
        parent -> childItems.append(this);
}

DownloadModelItem::DownloadModelItem(QJsonObject * data, DownloadModelItem * parent) {
     parentItem = parent;

     if (data) {
         if (data -> contains("childs")) {
             QJsonArray ar = data -> take("childs").toArray();
             QJsonObject iterObj;

             foreach(QJsonValue obj, ar) {
                 iterObj = obj.toObject();
                 new DownloadModelItem(&iterObj, this);
             }
         }

         itemData = data -> toVariantMap();
     }

     if (parent)
         parent -> childItems.append(this);
}

DownloadModelItem::~DownloadModelItem() {
    qDeleteAll(childItems);
}

QJsonObject DownloadModelItem::toJson() {
    QJsonObject root = QJsonObject::fromVariantMap(itemData);

    if (childItems.length() > 0) {
        QJsonArray ar = QJsonArray();
        for(int i = 0; i < childItems.length(); i++)
            ar.append(childItems.at(i) -> toJson());

        root["childs"] = ar;
    }

    return root;
}

DownloadModelItem * DownloadModelItem::child(int number) {
    return childItems.value(number);
}

int DownloadModelItem::childCount() const {
    return childItems.count();
}

int DownloadModelItem::row() const {
    if (parentItem)
        return parentItem -> childItems.indexOf(const_cast<DownloadModelItem *>(this));

    return 0;
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
    if (column < 0)
        return false;

    itemData.insert(QString::number(column), value);
    return true;
}
