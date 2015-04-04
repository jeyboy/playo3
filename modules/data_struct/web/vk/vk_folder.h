#ifndef VK_FOLDER_H
#define VK_FOLDER_H

#include "../web_folder_item.h"

namespace Playo3 {
    class VkFolder : public WebFolderItem {
    public:
        VkFolder(int initState);
        VkFolder(QJsonObject * hash, FolderItem * parent = 0);
        VkFolder(QString uid, QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);

        ~VkFolder();

        inline int itemType() const { return VK_PLAYLIST; }
        bool removePhysicalObject();

        bool isExist() const;
    };
}


#endif // VK_FOLDER_H
