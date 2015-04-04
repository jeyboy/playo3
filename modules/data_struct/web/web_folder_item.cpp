#include "web_folder_item.h"

using namespace Playo3;

///////////////////////////////////////////////////////////
WebFolderItem::WebFolderItem(int initState) : FolderItem(initState) {
}

WebFolderItem::WebFolderItem(QJsonObject * hash, FolderItem * parent) : FolderItem(hash, parent) {
}

WebFolderItem::WebFolderItem(QString uid, QString folderTitle, FolderItem * parent, int pos, int initState)
    : FolderItem(uid, parent, folderTitle, pos, initState) {
}

WebFolderItem::~WebFolderItem() {
}

FolderItem * WebFolderItem::createFolder(QString uid, QString name, int pos) {
    FolderItem * curr = folders.value(folderUid(name, uid), 0);

    if (!curr)
        curr = new WebFolderItem(uid, name, this, pos);
    return curr;
}

void WebFolderItem::accumulateUids(QHash<IItem *, QString> & store) {
    QList<IItem *>::Iterator it = children.begin();

    for( ;it != children.end(); it++) {
        if ((*it) -> isRemote()) {
            if ((*it) -> isContainer())
                (reinterpret_cast<WebFolderItem *>(*it)) -> accumulateUids(store);
            else {
                QVariant item_uid = (*it) -> uid();
                if (item_uid.isValid())
                    store.insert((*it), item_uid.toString());
            }
        }
    }
}
