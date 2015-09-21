#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <qfile.h>

#include "item_interface.h"
#include "playlist.h"

#define DEFAULT_TITLE QString()

namespace Core {
    class File : public IItem {
    public:
        inline File(QVariantMap & hash, Playlist * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline File(QJsonObject * hash, Playlist * parent = 0) : IItem(parent, hash) {}
        inline File(QString fileName, Playlist * parent = 0, int pos = -1) : IItem(parent, DEFAULT_TITLE, pos) {
            QString ext;
            if (FilenameConversions::extractExtension(fileName, ext))
                setExtension(ext);

            setTitle(fileName);
        }
        inline File(QString filePath, QString fileName, Playlist * parent = 0, int pos = -1)
            : IItem(parent, DEFAULT_TITLE, pos) {

            QString ext;
            if (FilenameConversions::extractExtension(fileName, ext))
                setExtension(ext);

            setTitle(fileName);
            setPath(filePath);
        }

        inline virtual ~File() {
            if (is(mark_on_removing))
                removePhysicalObject();
        }

        inline int itemType() const { return ITEM; }
        inline bool removePhysicalObject() { return QFile::remove(fullPath()); }

        inline bool isExist() const { return QFile::exists(fullPath()); }
    };
}

#endif // FILE_ITEM_H
