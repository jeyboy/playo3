#ifndef WEB_ITEM_H
#define WEB_ITEM_H

#include "playlist.h"

namespace Core {
    class WebFile : public IItem {
    public:
        inline WebFile(QVariantHash & hash, Playlist * parent = 0, int pos = -1) : IItem(parent, hash, pos) {}
        inline WebFile(QJsonObject * hash, Playlist * parent = 0) : IItem(parent, hash) {}
        inline WebFile(const QVariant & uid, const QString & filePath, const QString & fileName, Playlist * parent = 0, int pos = -1) : IItem(parent, fileName, pos) {
            setId(uid);
            setPath(filePath);
        }
        inline virtual ~WebFile() {}

//        inline QString fullPath() const { return path().toString(); }
//        virtual inline QUrl toUrl() const { return QUrl(fullPath()); }
//        virtual inline QString toUid() { return WebFile::toUid(owner(), id()); }
    };
}

#endif // WEB_ITEM_H
