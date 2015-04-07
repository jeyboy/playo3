#include "vk_item.h"

using namespace Playo3;

VkItem::VkItem(QJsonObject * hash, FolderItem * parent) : WebItem(hash, parent) {}
VkItem::VkItem(QVariantMap & hash, FolderItem * parent, int pos) : WebItem(hash, parent, pos) {}
VkItem::VkItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent, int pos) : WebItem(uid, filePath, fileName, parent, pos) {
    setExtension("mp3");
}

VkItem::~VkItem() {}

bool VkItem::removePhysicalObject() {
    //TODO: realization require
    return false;
}

bool VkItem::isExist() const {
    //TODO: realization require
    return true;
}
