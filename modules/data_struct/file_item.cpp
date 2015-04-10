#include "file_item.h"

using namespace Playo3;

FileItem::FileItem(QVariantMap & hash, FolderItem * parent, int pos) : IItem(parent, hash, pos) {
}

FileItem::FileItem(QJsonObject * hash, FolderItem * parent) : IItem(parent, hash) {
}

FileItem::FileItem(QString fileName, FolderItem * parent, int pos) : IItem(parent, DEFAULT_TITLE, pos) {
    proceedTitle(fileName);
}

FileItem::FileItem(QString filePath, QString fileName, FolderItem * parent, int pos)
    : IItem(parent, DEFAULT_TITLE, pos) {
    proceedTitle(fileName);
    setPath(filePath);
}

FileItem::~FileItem() {
    if (is(mark_on_removing))
        removePhysicalObject();
}

bool FileItem::removePhysicalObject() {
    return QFile::remove(fullPath());
}

bool FileItem::isExist() const {
    return QFile::exists(fullPath());
}

void FileItem::proceedTitle(QString & fileName) {
    QString sExt = fileName.section('.', -1, -1);
    if (sExt != fileName && sExt.indexOf(' ') == -1) {
        fileName = fileName.section('.', 0, -2);
        setExtension(sExt);
    }

    setTitle(fileName);
}
