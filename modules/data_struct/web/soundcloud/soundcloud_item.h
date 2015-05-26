#ifndef SOUNDCLOUD_ITEM_H
#define SOUNDCLOUD_ITEM_H

#include "../web_item.h"
#include "modules/web/socials_api/soundcloud_api_private.h"

namespace Playo3 {
    class SoundcloudItem : public WebItem {
    public:
        SoundcloudItem(QJsonObject * hash, FolderItem * parent = 0);
        SoundcloudItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1);
        SoundcloudItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);
        ~SoundcloudItem();

        inline QUrl toUrl() const {
            QUrl url = QUrl(fullPath());
            url.setQuery(SoundcloudApiPrivate::commonMethodParams());
            return url;
        }

        inline QVariant toUid() { return uid(); }
        inline int itemType() const { return SOUNDCLOUD_ITEM; }
        bool removePhysicalObject();

        bool isExist() const;
    };
}

#endif // SOUNDCLOUD_ITEM_H
