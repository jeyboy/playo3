#include "vk_file.h"
#include "web/socials/vk_api.h"
#include <QDebug>


VkFile::VkFile(QJsonObject *hash, ModelItem *parent) : ModelItem(hash, parent) {
    uid = hash -> value("u").toString();
    owner_uid = hash -> value("o").toString();
}

VkFile::VkFile(QString filePath, QString fileName, QString ownerID, QString fileID, ModelItem *parent, int genre_id, QString itemDuration, int itemSize, QString itemInfo, int initState)
    : ModelItem(filePath, fileName, parent, genre_id, itemDuration, itemSize, itemInfo, initState) {

    uid = fileID;
    owner_uid = ownerID;
    extension = "mp3";
}

VkFile::~VkFile() {}

bool VkFile::removePhysicalObject() {
    //TODO: realization require
    return false;
//    return QFile::remove(fullPath());
}

bool VkFile::isExist() const {
    //TODO: realization require
    return true;
//    return QFile::exists(fullPath());
}

bool VkFile::isRemote() const { return true; }

QString VkFile::toUID() {
    return buildUid(owner_uid, uid);
}

QUrl VkFile::toUrl() {
    return QUrl(path);
}

QJsonObject VkFile::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = VK_FILE;
    root["u"] = uid;
    root["o"] = owner_uid;

    return root;
}
