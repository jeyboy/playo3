#include "vk_folder.h"

using namespace Playo3;

VkFolder::VkFolder(int initState) : WebFolderItem(initState) {}
VkFolder::VkFolder(QJsonObject * hash, FolderItem * parent) : WebFolderItem(hash, parent) {}
VkFolder::VkFolder(QString uid, QString folderTitle, FolderItem * parent, int pos, int initState)
    : WebFolderItem(uid, folderTitle, parent, pos, initState) {}

VkFolder::~VkFolder() {}

bool VkFolder::removePhysicalObject() {
    //TODO: add realization
    return false;
}

bool VkFolder::isExist() const {
    //TODO: add realization
    return true;
}
