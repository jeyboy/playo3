#ifndef SOUNDCLOUD_FOLDER_H
#define SOUNDCLOUD_FOLDER_H

#include "../web_folder_item.h"

namespace Playo3 {
    class SoundcloudFolder : public WebFolderItem {
    public:
        SoundcloudFolder(int initState);
        SoundcloudFolder(QJsonObject * hash, FolderItem * parent = 0);
        SoundcloudFolder(QString uid, QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);

        ~SoundcloudFolder();

        inline int itemType() const { return SOUNDCLOUD_PLAYLIST; }
        bool removePhysicalObject();

        bool isExist() const;
    };
}

#endif // SOUNDCLOUD_FOLDER_H
