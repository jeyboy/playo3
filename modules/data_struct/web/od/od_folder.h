#ifndef OD_FOLDER_H
#define OD_FOLDER_H

#include "../web_folder_item.h"

namespace Playo3 {
    class OdFolder : public WebFolderItem {
    public:
        inline OdFolder(int initState) : WebFolderItem(initState) {}
        inline OdFolder(const Json::Obj & hash, FolderItem * parent = 0) : WebFolderItem(hash, parent) {}
        inline OdFolder(const QString & uid, const QString & folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE)
            : WebFolderItem(uid, folderTitle, parent, pos, initState) {}

        inline int itemType() const { return OD_PLAYLIST; }

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

#endif // OD_FOLDER_H
