#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <QFile>
#include "item_interface.h"
#include "folder_item.h"

namespace Playo3 {
    class FileItem : public ItemInterface {
    public:
        FileItem(QJsonObject * hash, FolderItem * parent = 0);
        FileItem(const QString filepath, FolderItem * parent = 0);
        ~FileItem();

        bool removePhysicalObject();

        bool isExist() const;

        QJsonObject toJson();
//    protected:
//        QString fileExtension(QString filePath);
//        QString fileTitle(QString filePath);
    };
}

#endif // FILE_ITEM_H
