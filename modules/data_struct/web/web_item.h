#ifndef WEB_ITEM_H
#define WEB_ITEM_H

#include "web_folder_item.h"

namespace Playo3 {
    class WebItem : public IItem {
    public:
        static inline QVariant toUid(QVariant owner, QVariant id) {
            return owner.isValid() && id.isValid() ? owner.toString() + "_" + id.toString() : QVariant();
        }

        inline WebItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline WebItem(QJsonObject * hash, FolderItem * parent = 0) : IItem(parent, hash) {}
        inline WebItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1) : IItem(parent, fileName, pos) {
            setUid(uid);
            setPath(filePath);
        }
        inline WebItem(QVariant song_uid, QVariant artist_uid, QString fileName, FolderItem * parent, WebSubType subtype = none, int pos = -1) : IItem(parent, fileName, pos) {
            setArtistUid(artist_uid);
            setSongUid(song_uid);
            setSubtype(subtype);
        }

        inline virtual ~WebItem() {}

        inline virtual int itemType() const { return WEB_ITEM; }

        inline QString fullPath() const { return path().toString(); }
        virtual inline QUrl toUrl() const { return QUrl(fullPath()); }
        virtual inline QVariant toUid() { return WebItem::toUid(owner(), uid()); }
        inline bool isRemote() const { return true; }

        inline virtual bool removePhysicalObject() { return false; }
        inline virtual bool isExist() const { return true; }
    };
}

#endif // WEB_ITEM_H
