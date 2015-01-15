#include "file_item.h"

using namespace Playo3;

FileItem::FileItem(QJsonObject * hash, ItemInterface * parent) : ItemInterface(parent, hash) {
}

FileItem::FileItem(QString filePath, ItemInterface * parent)
    : ItemInterface(parent, filePath, filePath.section('/', -1, -1)) {
    _extension = _title.section('.', -1, -1);
    if (_extension != _title)
        _title = _title.section('.', 0, -2);
    else _extension = QString();
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
    QJsonObject root = ItemInterface::toJSON();

    root[JSON_TYPE_ITEM_TYPE] = FILE_ITEM;

    return root;
}
