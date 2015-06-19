#include "echo_item.h"

using namespace Playo3;

EchoItem::EchoItem(QJsonObject * hash, FolderItem * parent) : WebItem(hash, parent) {}
EchoItem::EchoItem(QVariantMap & hash, FolderItem * parent, int pos) : WebItem(hash, parent, pos) {}
EchoItem::EchoItem(QVariant uid, QVariant artist_uid, QString filePath, QString fileName, FolderItem * parent, int pos) : WebItem(uid, filePath, fileName, parent, pos) {
    setArtistUid(artist_uid);
}

EchoItem::~EchoItem() {}

bool EchoItem::removePhysicalObject() {
    //TODO: realization require
    return false;
}

bool EchoItem::isExist() const {
    //TODO: realization require
    return true;
}
