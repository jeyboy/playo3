#ifndef SOUNDCLOUD_FOLDER_H
#define SOUNDCLOUD_FOLDER_H

#include "../web_folder_item.h"

namespace Playo3 {
    class SoundcloudFolder : public WebFolderItem {
    public:
        inline SoundcloudFolder(int initState) : WebFolderItem(initState) {}
        inline SoundcloudFolder(Json::Obj * hash, FolderItem * parent = 0) : WebFolderItem(hash, parent) {}
        inline SoundcloudFolder(const QString & uid, const QString & folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE)
            : WebFolderItem(uid, folderTitle, parent, pos, initState) {}

        inline int itemType() const { return SOUNDCLOUD_PLAYLIST; }

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

#endif // SOUNDCLOUD_FOLDER_H
