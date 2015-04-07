#ifndef VK_ITEM_H
#define VK_ITEM_H

#include "../web_item.h"

namespace Playo3 {
    class VkItem : public WebItem {
    public:
        VkItem(QJsonObject * hash, FolderItem * parent = 0);
        VkItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1);
        VkItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);
        ~VkItem();

        inline int itemType() const { return VK_ITEM; }
        inline QVariant toUid() { return WebItem::toUid(owner(), uid()); }
        bool removePhysicalObject();

        bool isExist() const;
    };
}


#endif // VK_ITEM_H
