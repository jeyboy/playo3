//#include "soundcloud_playlist.h"
//#include <QDebug>

/////////////////////////////////////////////////////////////
//SoundcloudPlaylist::SoundcloudPlaylist() : FolderItem(STATE_UNPROCESSED) {
//    folders = new QHash<QString, ModelItem *>();
//}

//SoundcloudPlaylist::SoundcloudPlaylist(QJsonObject * hash, ModelItem *parent) : FolderItem(hash, parent) {
//    uid = hash -> value("u").toString();
//}

//SoundcloudPlaylist::SoundcloudPlaylist(QString filePath, QString folderID, QString folderTitle, ModelItem *parent, int initState)
//    : FolderItem(filePath, folderTitle, parent, initState) {
//    uid = folderID;
//}

//SoundcloudPlaylist::~SoundcloudPlaylist() {}

//QUrl SoundcloudPlaylist::toUrl() {
//    return QUrl(path);
//}

//void SoundcloudPlaylist::openLocation() {
//    QDesktopServices::openUrl(toUrl());
//}

//bool SoundcloudPlaylist::removePhysicalObject() {
//    //TODO: add realization

//    return false;
//}

//bool SoundcloudPlaylist::isExist() const {
//    //TODO: add realization

//    return true;
//}

//bool SoundcloudPlaylist::isRemote() const { return true; }

//QJsonObject SoundcloudPlaylist::toJSON() {
//    QJsonObject root = FolderItem::toJSON();

//    root["i"] = SOUNDCLOUD_PLAYLIST;
//    root["u"] = uid;

//    return root;
//}
