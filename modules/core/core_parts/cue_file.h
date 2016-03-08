#ifndef CUE_ITEM_H
#define CUE_ITEM_H

#include "item_interface.h"

namespace Core {
    class CueFile : public IItem {
    public:
        inline CueFile(QJsonObject * hash, Playlist * parent = 0) : IItem(parent, hash) {}
        inline CueFile(QVariantHash & hash, Playlist * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline CueFile(uint startPos, uint duration, const QString & filePath, const QString & fileName, const QString & extension, bool is_parted, Playlist * parent = 0)
            : IItem(parent, fileName) {

            if (duration > 0)
                setDuration(Duration::fromMillis(duration));
            setExtension(extension);
            setStartPos(startPos);
            setPath(filePath);
            setParted(is_parted);
        }
    };
}

#endif // CUE_ITEM_H
