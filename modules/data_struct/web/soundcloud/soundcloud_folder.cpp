#include "soundcloud_folder.h"

using namespace Playo3;

SoundcloudFolder::SoundcloudFolder(int initState) : WebFolderItem(initState) {}
SoundcloudFolder::SoundcloudFolder(QJsonObject * hash, FolderItem * parent) : WebFolderItem(hash, parent) {}
SoundcloudFolder::SoundcloudFolder(QString uid, QString folderTitle, FolderItem * parent, int pos, int initState)
    : WebFolderItem(uid, folderTitle, parent, pos, initState) {}

SoundcloudFolder::~SoundcloudFolder() {}

bool SoundcloudFolder::removePhysicalObject() {
    //TODO: add realization
    return false;
}

bool SoundcloudFolder::isExist() const {
    //TODO: add realization
    return true;
}
