#ifndef SOUNDCLOUD_ITEM_H
#define SOUNDCLOUD_ITEM_H

#include "web_file.h"
#include "modules/core/web/apis/social/soundcloud_api.h"

namespace Core {
    class SoundcloudFile : public WebFile {
    public:
        inline SoundcloudFile(QJsonObject * hash, Playlist * parent = 0) : WebFile(hash, parent) {}
        inline SoundcloudFile(QVariantMap & hash, Playlist * parent = 0, int pos = -1) : WebFile(hash, parent, pos) {}
        inline SoundcloudFile(const QVariant & uid, const QString & filePath, const QString & fileName, Playlist * parent = 0, int pos = -1)
            : WebFile(uid, filePath, fileName, parent, pos) {
            //    setExtension("mp3");
            //    setBpm(itemBpm);
        }

        inline QUrl toUrl() const {
            QUrl url = QUrl(fullPath());
            url.setQuery(Web::Soundcloud::Api::instance() -> genDefaultParams());
            return url;
        }

        inline QVariant toUid() { return uid(); }
        inline int itemType() const { return SOUNDCLOUD_FILE; }
    };
}

#endif // SOUNDCLOUD_ITEM_H
