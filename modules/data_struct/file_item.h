#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <QFile>
#include "item_interface.h"
#include "folder_item.h"

namespace Playo3 {
    class FileItem : public ItemInterface {
    public:
        FileItem(QJsonObject * hash, FolderItem * parent = 0);
        FileItem(QString fileName, FolderItem * parent = 0, int pos = -1);
        FileItem(QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);

        ~FileItem();

        bool removePhysicalObject();

        bool isExist() const;

        QJsonObject toJson();
    protected:
        QString proceedTitle(QString & title);
    };
}

#endif // FILE_ITEM_H
