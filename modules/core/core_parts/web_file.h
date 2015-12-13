#ifndef WEB_ITEM_H
#define WEB_ITEM_H

#include "playlist.h"

namespace Core {
    class WebFile : public IItem {
    public:
        static inline QString toUid(QVariant owner, QVariant id) {
            return /*owner.isValid() &&*/ id.isValid() ? owner.toString() % QStringLiteral("_") % id.toString() : QString();
        }

        inline WebFile(QVariantMap & hash, Playlist * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline WebFile(QJsonObject * hash, Playlist * parent = 0) : IItem(parent, hash) {}
        inline WebFile(const QVariant & uid, const QString & filePath, const QString & fileName, Playlist * parent = 0, int pos = -1) : IItem(parent, fileName, pos) {
            setUid(uid);
            setPath(filePath);
        }
        inline virtual ~WebFile() {}

        inline virtual int itemType() const { return WEB_FILE; }

        inline QString fullPath() const { return path().toString(); }
        virtual inline QUrl toUrl() const { return QUrl(fullPath()); }
        virtual inline QString toUid() { return WebFile::toUid(owner(), uid()); }
        inline bool isRemote() const { return true; }

        inline virtual bool removePhysicalObject() { return false; }
        inline virtual bool isExist() const { return true; }
    };
}

#endif // WEB_ITEM_H
