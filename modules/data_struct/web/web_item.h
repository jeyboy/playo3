#ifndef WEB_ITEM_H
#define WEB_ITEM_H

#include "../item_interface.h"
#include "web_folder_item.h"

namespace Playo3 {
    class WebItem : public IItem {
    public:
        WebItem(QVariantMap & hash, WebFolderItem * parent = 0, int pos = -1);
        WebItem(QJsonObject * hash, WebFolderItem * parent = 0);
//        WebItem(QString fileName, FolderItem * parent = 0, int pos = -1);
//        WebItem(QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);

//        VkFile(const QString filepath, QString filename, QString ownerID, QString fileID, ModelItem *parent = 0, int genre_id = -1, QString itemDuration = "", int itemSize = -1, QString itemInfo = "", int init_state = STATE_DEFAULT | STATE_CHECKED);

        ~WebItem();
    };
}

#endif // WEB_ITEM_H
