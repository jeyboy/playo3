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
