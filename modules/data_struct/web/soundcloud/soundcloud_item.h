#ifndef SOUNDCLOUD_ITEM_H
#define SOUNDCLOUD_ITEM_H

#include "../web_item.h"
#include "modules/web/socials_api/soundcloud_api.h"

namespace Playo3 {
    class SoundcloudItem : public WebItem {
    public:
        inline SoundcloudItem(QJsonObject * hash, FolderItem * parent = 0) : WebItem(hash, parent) {}
        inline SoundcloudItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1) : WebItem(hash, parent, pos) {}
        inline SoundcloudItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1)
            : WebItem(uid, filePath, fileName, parent, pos) {
            //    setExtension("mp3");
            //    setBpm(itemBpm);
        }

        inline QUrl toUrl() const {
            QUrl url = QUrl(fullPath());
            url.setQuery(SoundcloudApi::instance() -> genDefaultParams());
            return url;
        }

        inline QVariant toUid() { return uid(); }
        inline int itemType() const { return SOUNDCLOUD_ITEM; }
    };
}

#endif // SOUNDCLOUD_ITEM_H
