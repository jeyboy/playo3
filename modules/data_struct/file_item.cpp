#include "file_item.h"

using namespace Playo3;

FileItem::FileItem(QJsonObject * hash, ItemInterface * parent) : ItemInterface(hash, parent) {
}

FileItem::FileItem(QString filePath, ItemInterface * parent)
    : ItemInterface(filePath, filePath.section('/', -1, -1), parent) {
    extension = title.section('.', -1, -1);
    if (extension != title)
        title = title.section('.', 0, -2);
    else extension = QString();
}

FileItem::~FileItem() {

}

bool FileItem::removePhysicalObject() {
    return QFile::remove(fullPath());
}

bool FileItem::isExist() const {
    return QFile::exists(fullPath());
}

QJsonObject FileItem::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = FILE_ITEM;

    return root;
}
