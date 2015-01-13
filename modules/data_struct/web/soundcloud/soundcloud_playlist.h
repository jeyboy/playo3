#ifndef SOUNDCLOUD_PLAYLIST_H
#define SOUNDCLOUD_PLAYLIST_H

#include "model/folder_item.h"

class SoundcloudPlaylist : public FolderItem {
public:
    SoundcloudPlaylist();
    SoundcloudPlaylist(QJsonObject * hash, ModelItem *parent = 0);
    SoundcloudPlaylist(const QString filePath, QString folderID, QString folderTitle = "", ModelItem *parent = 0, int initState = STATE_UNPROCESSED | STATE_CHECKED);
    ~SoundcloudPlaylist();

    QUrl toUrl();
    void openLocation();
    bool removePhysicalObject();

    bool isExist() const;
    bool isRemote() const;

    QJsonObject toJSON();

protected:
    QString uid;
};


#endif // SOUNDCLOUD_PLAYLIST_H
