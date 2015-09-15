#ifndef VK_FOLDER_H
#define VK_FOLDER_H

#include "../web_folder_item.h"

namespace Playo3 {
    class VkFolder : public WebFolderItem {
    public:
        inline VkFolder(int initState) : WebFolderItem(initState) {}
        inline VkFolder(Json::Obj & hash, FolderItem * parent = 0) : WebFolderItem(hash, parent) {}
        inline VkFolder(const QString & uid, const QString & folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE)
            : WebFolderItem(uid, folderTitle, parent, pos, initState) {}

        inline int itemType() const { return VK_PLAYLIST; }
        inline bool removePhysicalObject() {
            //TODO: add realization
            return false;
        }

        inline bool isExist() const {
            //TODO: add realization
            return true;
        }
    };
}

#endif // VK_FOLDER_H
