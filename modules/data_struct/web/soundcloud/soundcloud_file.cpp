#include "soundcloud_file.h"
#include "web/socials/soundcloud_api.h"
#include <QDebug>


SoundcloudFile::SoundcloudFile(QJsonObject *hash, ModelItem *parent) : ModelItem(hash, parent) {
    uid = hash -> value("u").toString();
    owner_uid = hash -> value("o").toString();
}

SoundcloudFile::SoundcloudFile(QString filePath, QString fileName, QString fileExtension, QString ownerID, QString fileID, ModelItem *parent, int genre_id, QString itemDuration, int itemSize, int itemBpm, QString itemInfo, int initState)
    : ModelItem(filePath, fileName, parent, genre_id, itemDuration, itemSize, itemInfo, initState) {

    bpm = itemBpm;
    uid = fileID;
    owner_uid = ownerID;
    extension = fileExtension;
}

SoundcloudFile::~SoundcloudFile() {}

bool SoundcloudFile::removePhysicalObject() {
    //TODO: realization require
    return false;
//    return QFile::remove(fullPath());
}

bool SoundcloudFile::isExist() const {
    //TODO: realization require
    return true;
//    return QFile::exists(fullPath());
}

bool SoundcloudFile::isRemote() const { return true; }

QString SoundcloudFile::toUID() {
    return buildUid(owner_uid, uid);
}

QUrl SoundcloudFile::toUrl() {
    QUrl url(path);
    url.setQuery(SoundcloudApi::instance() -> commonMethodParams());

    return url;
}

QJsonObject SoundcloudFile::toJSON() {
    QJsonObject root = ModelItem::toJSON();

    root["i"] = SOUNDCLOUD_FILE;
    root["u"] = uid;
    root["o"] = owner_uid;

    return root;
}
