#ifndef VK_ITEM_H
#define VK_ITEM_H

#include "../web_item.h"

namespace Playo3 {
    class VkItem : public WebItem {
    public:
        VkItem(QJsonObject * hash, FolderItem * parent = 0);
        VkItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1);
        VkItem(QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);
//        VkFile(const QString filepath, QString filename, QString ownerID, QString fileID, ModelItem *parent = 0, int genre_id = -1, QString itemDuration = "", int itemSize = -1, QString itemInfo = "", int init_state = STATE_DEFAULT | STATE_CHECKED);
        ~VkItem();

        inline int itemType() const { return VK_ITEM; }
        inline QVariant toUID() { return QVariant(owner().toString() + "_" + uid().toString()); }
        bool removePhysicalObject();

        bool isExist() const;
    };
}


#endif // VK_ITEM_H
