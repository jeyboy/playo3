#ifndef SOUNDCLOUD_FOLDER_H
#define SOUNDCLOUD_FOLDER_H

#include "web_playlist.h"

namespace Core {
    class SoundcloudPlaylist : public WebPlaylist {
    public:
        inline SoundcloudPlaylist(int initState) : WebPlaylist(initState) {}
        inline SoundcloudPlaylist(QJsonObject * hash, Playlist * parent = 0) : WebPlaylist(hash, parent) {}
        inline SoundcloudPlaylist(QString uid, QString folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_ITEM_STATE)
            : WebPlaylist(uid, folderTitle, parent, pos, initState) {}
    };
}

#endif // SOUNDCLOUD_FOLDER_H
