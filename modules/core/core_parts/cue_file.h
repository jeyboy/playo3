#ifndef CUE_ITEM_H
#define CUE_ITEM_H

#include "item_interface.h"

namespace Core {
    class CueFile : public IItem {
    public:
        inline CueFile(QJsonObject * hash, Playlist * parent = 0) : IItem(parent, hash) {}
        inline CueFile(QVariantMap & hash, Playlist * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline CueFile(uint startPos, const QString & filePath, const QString & fileName, Playlist * parent = 0)
            : IItem(parent, fileName) {
            setStartPos(startPos);
            setPath(filePath);
        }

        inline int itemType() const { return CUE_FILE; }

        inline bool removePhysicalObject() {
            //TODO: add realization
            return false;
        }
        inline bool isExist() const {
            //TODO: add realization
            return true;
        }
    };
}

#endif // CUE_ITEM_H
