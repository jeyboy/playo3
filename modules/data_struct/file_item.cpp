#include "file_item.h"

using namespace Playo3;

FileItem::FileItem(QVariantMap & hash, FolderItem * parent, int pos) : ItemInterface(parent, hash, pos) {
}

FileItem::FileItem(QJsonObject * hash, FolderItem * parent) : ItemInterface(parent, hash) {
}

FileItem::FileItem(QString fileName, FolderItem * parent, int pos) : ItemInterface(parent, DEFAULT_TITLE, pos) {
    proceedTitle(fileName);
}

FileItem::FileItem(QString filePath, QString fileName, FolderItem * parent, int pos)
    : ItemInterface(parent, DEFAULT_TITLE, pos) {
    proceedTitle(fileName);
    setPath(filePath);
}

FileItem::~FileItem() {

}

bool FileItem::removePhysicalObject() {
    return QFile::remove(fullPath());
}

bool FileItem::isExist() const {
    return QFile::exists(fullPath());
}

QJsonObject FileItem::toJson() {
    QJsonObject root = ItemInterface::toJson();

    root[JSON_TYPE_ITEM_TYPE] = itemType();

    return root;
}

void FileItem::proceedTitle(QString & fileName) {
    QString sExt = fileName.section('.', -1, -1);
    if (sExt != fileName && sExt.indexOf(' ') == -1) {
        fileName = fileName.section('.', 0, -2);
        setExtension(sExt);
    }

    setTitle(fileName);
}
