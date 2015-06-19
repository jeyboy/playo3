#ifndef ECHO_ITEM_H
#define ECHO_ITEM_H

#include "../web_item.h"

namespace Playo3 {
    class EchoItem : public WebItem {
    public:
        EchoItem(QJsonObject * hash, FolderItem * parent = 0);
        EchoItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1);
        EchoItem(QVariant uid, QVariant artist_uid, QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);
        ~EchoItem();

        inline int itemType() const { return ECHO_ITEM; }
        bool removePhysicalObject();

        bool isExist() const;
    };
}

#endif // ECHO_ITEM_H
