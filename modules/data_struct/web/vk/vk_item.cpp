#include "vk_item.h"

using namespace Playo3;

VkItem::VkItem(QJsonObject * hash, FolderItem * parent) : WebItem(hash, parent) {}
VkItem::VkItem(QVariantMap & hash, FolderItem * parent, int pos) : WebItem(hash, parent, pos) {}
VkItem::VkItem(QString filePath, QString fileName, FolderItem * parent, int pos) : WebItem(filePath, fileName, parent, pos) {
    setExtension("mp3");
}

//VkFile::VkFile(QString filePath, QString fileName, QString ownerID, QString fileID, ModelItem *parent, int genre_id, QString itemDuration, int itemSize, QString itemInfo, int initState)
//    : ModelItem(filePath, fileName, parent, genre_id, itemDuration, itemSize, itemInfo, initState) {

//    uid = fileID;
//    owner_uid = ownerID;
//    extension = "mp3";
//}

VkItem::~VkItem() {}

bool VkItem::removePhysicalObject() {
    //TODO: realization require
    return false;
}

bool VkItem::isExist() const {
    //TODO: realization require
    return true;
}
