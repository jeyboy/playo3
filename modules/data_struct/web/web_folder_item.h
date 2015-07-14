#ifndef WEB_FOLDER_ITEM_H
#define WEB_FOLDER_ITEM_H

#include "modules/data_struct/folder_item.h"

namespace Playo3 {
    class WebFolderItem : public FolderItem {
    public:
        inline WebFolderItem(int initState = DEFAULT_MODEL_CONTAINER_STATE) : FolderItem(initState) {}
        inline WebFolderItem(QJsonObject * hash, FolderItem * parent = 0) : FolderItem(hash, parent) {}
        inline WebFolderItem(QString uid, QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE)
            : FolderItem(folderTitle, parent, uid, pos, initState) {}

        inline virtual ~WebFolderItem() {}

        inline QUrl toUrl() { return QUrl(path().toString()); }
        inline bool isRemote() const { return true; }
    };
}

#endif // WEB_FOLDER_ITEM_H
