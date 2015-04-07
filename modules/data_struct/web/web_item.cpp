#include "web_item.h"

using namespace Playo3;

WebItem::WebItem(QVariantMap & hash, FolderItem * parent, int pos) : IItem(parent, hash, pos) {
}

WebItem::WebItem(QJsonObject * hash, FolderItem * parent) : IItem(parent, hash) {
}

WebItem::WebItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent, int pos) : IItem(parent, fileName, pos) {
    setUid(uid);
    setPath(filePath);
}

WebItem::~WebItem() {
}
