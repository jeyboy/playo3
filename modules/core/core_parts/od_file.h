#ifndef OD_ITEM_H
#define OD_ITEM_H

#include "web_file.h"
//#include "modules/web/socials_api/od_api.h"

namespace Core {
    class OdItem : public WebFile {
    public:
        inline OdItem(QJsonObject * hash, Playlist * parent = 0) : WebFile(hash, parent) {}
        inline OdItem(QVariantMap & hash, Playlist * parent = 0, int pos = -1) : WebFile(hash, parent, pos) {}
        inline OdItem(QVariant uid, QString filePath, QString fileName, Playlist * parent = 0, int pos = -1)
            : WebFile(uid, filePath, fileName, parent, pos) {
            //    setExtension("mp3");
            //    setBpm(itemBpm);
        }

        inline QVariant toUid() { return uid(); }
        inline int itemType() const { return OD_FILE; }
    };
}

#endif // OD_ITEM_H
