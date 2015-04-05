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
