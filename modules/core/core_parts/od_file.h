#ifndef OD_ITEM_H
#define OD_ITEM_H

#include "web_file.h"
//#include "modules/web/socials_api/od_api.h"

namespace Core {
    class OdFile : public WebFile {
    public:
        inline OdFile(QJsonObject * hash, Playlist * parent = 0) : WebFile(hash, parent) {}
        inline OdFile(QVariantMap & hash, Playlist * parent = 0, int pos = -1) : WebFile(hash, parent, pos) {}
        inline OdFile(const QVariant & uid, const QString & filePath, const QString & fileName, Playlist * parent = 0, int pos = -1)
            : WebFile(uid, filePath, fileName, parent, pos) {
            //    setExtension("mp3");
            //    setBpm(itemBpm);
        }

        inline QString toUid() { return uid().toString(); }
        inline int itemType() const { return OD_FILE; }
        inline bool isShareable() const { return true; }
    };
}

#endif // OD_ITEM_H
