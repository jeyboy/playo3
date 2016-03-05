#ifndef SOUNDCLOUD_ITEM_H
#define SOUNDCLOUD_ITEM_H

#include "web_file.h"

namespace Core {
    class SoundcloudFile : public WebFile {
    public:
        inline SoundcloudFile(QJsonObject * hash, Playlist * parent = 0) : WebFile(hash, parent) {}
        inline SoundcloudFile(QVariantHash & hash, Playlist * parent = 0, int pos = -1) : WebFile(hash, parent, pos) {}
        inline SoundcloudFile(const QVariant & uid, const QString & filePath, const QString & fileName, Playlist * parent = 0, int pos = -1)
            : WebFile(uid, filePath, fileName, parent, pos) {
            //    setExtension("mp3");
            //    setBpm(itemBpm);
        }

        inline int itemType() const { return SOUNDCLOUD_FILE; }
    };
}

#endif // SOUNDCLOUD_ITEM_H
