#include "web_item.h"

using namespace Playo3;

WebItem::WebItem(QVariantMap & hash, WebFolderItem * parent, int pos) : IItem(parent, hash, pos) {
}

WebItem::WebItem(QJsonObject * hash, WebFolderItem * parent) : IItem(parent, hash) {
}

//FileItem::FileItem(QString fileName, FolderItem * parent, int pos) : IItem(parent, DEFAULT_TITLE, pos) {
//    proceedTitle(fileName);
//}

//FileItem::FileItem(QString filePath, QString fileName, FolderItem * parent, int pos)
//    : IItem(parent, DEFAULT_TITLE, pos) {
//    proceedTitle(fileName);
//    setPath(filePath);
//}

FileItem::~FileItem() {
    if (is(mark_on_removing))
        removePhysicalObject();
}
