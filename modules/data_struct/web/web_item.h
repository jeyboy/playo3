#ifndef WEB_ITEM_H
#define WEB_ITEM_H

#include "web_folder_item.h"

namespace Playo3 {
    class WebItem : public IItem {
    public:
        WebItem(QVariantMap & hash, FolderItem * parent = 0, int pos = -1);
        WebItem(QJsonObject * hash, FolderItem * parent = 0);
        WebItem(QString filePath, QString fileName, FolderItem * parent = 0, int pos = -1);

        ~WebItem();

        inline QUrl toUrl() { return QUrl(path().toString()); }
        inline bool isRemote() const { return true; }
    };
}

#endif // WEB_ITEM_H
