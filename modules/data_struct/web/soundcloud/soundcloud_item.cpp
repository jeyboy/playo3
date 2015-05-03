#include "soundcloud_item.h"

using namespace Playo3;

SoundcloudItem::SoundcloudItem(QJsonObject * hash, FolderItem * parent) : WebItem(hash, parent) {}
SoundcloudItem::SoundcloudItem(QVariantMap & hash, FolderItem * parent, int pos) : WebItem(hash, parent, pos) {}
SoundcloudItem::SoundcloudItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent, int pos) : WebItem(uid, filePath, fileName, parent, pos) {
//    setExtension("mp3");
//    setBpm(itemBpm);
}

SoundcloudItem::~SoundcloudItem() {}

bool SoundcloudItem::removePhysicalObject() {
    //TODO: realization require
    return false;
}

bool SoundcloudItem::isExist() const {
    //TODO: realization require
    return true;
}
