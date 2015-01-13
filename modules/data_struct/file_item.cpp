#include "file_item.h"
#include <QDebug>


FileItem::FileItem(QJsonObject *hash, ModelItem *parent) : ModelItem(hash, parent) {
}

FileItem::FileItem(QString filePath, ModelItem *parent, int genre_id, QString itemDuration, int itemSize, QString itemInfo, int initState)
    : ModelItem(filePath, filePath.section('/', -1, -1), parent, genre_id, itemDuration, itemSize, itemInfo, initState) {
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
