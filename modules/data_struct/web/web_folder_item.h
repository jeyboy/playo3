#ifndef WEB_FOLDER_ITEM_H
#define WEB_FOLDER_ITEM_H

#include "modules/data_struct/folder_item.h"

namespace Playo3 {
    class WebFolderItem : public FolderItem {
    public:
        WebFolderItem(int initState = DEFAULT_MODEL_CONTAINER_STATE);
        WebFolderItem(QJsonObject * hash, FolderItem * parent = 0);
        WebFolderItem(QString uid, QString folderTitle, FolderItem * parent = 0, int pos = -1, int initState = DEFAULT_MODEL_CONTAINER_STATE);
        ~WebFolderItem();

        FolderItem * createFolder(QString name, QString uid, int pos = -1);
        void accumulateUids(QHash<IItem *, QString> & store);
    };
}

#endif // WEB_FOLDER_ITEM_H
