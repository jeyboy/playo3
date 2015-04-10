#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <QFile>
#include "item_interface.h"
#include "folder_item.h"

#define DEFAULT_TITLE QString()

namespace Playo3 {
    class FileItem : public IItem {
    public:
        FileItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1);
        FileItem(QJsonObject * hash, FolderItem * parent = 0);
        FileItem(QString fileName, FolderItem * parent = 0, int pos = -1);
        FileItem(QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);

        ~FileItem();

        inline int itemType() const { return ITEM; }
        bool removePhysicalObject();

        bool isExist() const;
    protected:
        void proceedTitle(QString & title);
    };
}

#endif // FILE_ITEM_H
