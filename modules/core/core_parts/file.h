#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <qfile.h>

#include "item_interface.h"
#include "playlist.h"

#define DEFAULT_TITLE QString()

namespace Core {
    class File : public IItem {
    public:
        inline File(QVariantHash & hash, Playlist * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline File(QJsonObject * hash, Playlist * parent = 0) : IItem(parent, hash) {}
        File(QString fileName, Playlist * parent = 0, int pos = -1);
        File(const QString & filePath, QString fileName, Playlist * parent = 0, int pos = -1);
        File(const QString & filePath, const QString & fileName, const QString & fileExtension, Playlist * parent = 0, int pos = -1);

//        inline virtual ~File() {
//            if (has(flag_mark_on_removing) && !isPartial())
//                removePhysicalObject();
//        }
    };
}

#endif // FILE_ITEM_H
