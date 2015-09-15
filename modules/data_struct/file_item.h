#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <qfile.h>
#include "item_interface.h"
#include "folder_item.h"

#define DEFAULT_TITLE QString()

namespace Playo3 {
    class FileItem : public IItem {
    public:
        inline FileItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline FileItem(Json::Obj & hash, FolderItem * parent = 0) : IItem(parent, hash) {}
        inline FileItem(QString fileName, FolderItem * parent = 0, int pos = -1) : IItem(parent, DEFAULT_TITLE, pos) {
            QString ext;
            if (FilenameConversions::extractExtension(fileName, ext))
                setExtension(ext);

            setTitle(fileName);
        }
        inline FileItem(QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1)
            : IItem(parent, DEFAULT_TITLE, pos) {

            QString ext;
            if (FilenameConversions::extractExtension(fileName, ext))
                setExtension(ext);

            setTitle(fileName);
            setPath(filePath);
        }

        inline virtual ~FileItem() {
            if (is(mark_on_removing))
                removePhysicalObject();
        }

        inline int itemType() const { return ITEM; }
        inline bool removePhysicalObject() { return QFile::remove(fullPath()); }

        inline bool isExist() const { return QFile::exists(fullPath()); }
    };
}

#endif // FILE_ITEM_H
