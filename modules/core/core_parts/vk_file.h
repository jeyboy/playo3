#ifndef VK_ITEM_H
#define VK_ITEM_H

#include "../web_item.h"

namespace Playo3 {
    class VkItem : public WebItem {
    public:        
        inline VkItem(QJsonObject * hash, FolderItem * parent = 0) : WebItem(hash, parent) {}
        inline VkItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1) : WebItem(hash, parent, pos) {}
        inline VkItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1)
            : WebItem(uid, filePath, fileName, parent, pos) { setExtension("mp3"); }

        inline int itemType() const { return VK_ITEM; }
    };
}

#endif // VK_ITEM_H
