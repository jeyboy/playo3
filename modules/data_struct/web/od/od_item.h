#ifndef OD_ITEM_H
#define OD_ITEM_H

#include "../web_item.h"
//#include "modules/web/socials_api/od_api.h"

namespace Playo3 {
    class OdItem : public WebItem {
    public:
        inline OdItem(QJsonObject * hash, FolderItem * parent = 0) : WebItem(hash, parent) {}
        inline OdItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1) : WebItem(hash, parent, pos) {}
        inline OdItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1)
            : WebItem(uid, filePath, fileName, parent, pos) {
            //    setExtension("mp3");
            //    setBpm(itemBpm);
        }

//        inline QUrl toUrl() const {
//            QUrl url = QUrl(fullPath());
//            url.setQuery(Soundcloud::Api::instance() -> genDefaultParams());
//            return url;
//        }

        inline QVariant toUid() { return uid(); }
        inline int itemType() const { return OD_ITEM; }
    };
}

#endif // OD_ITEM_H
