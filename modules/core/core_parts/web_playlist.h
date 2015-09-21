#ifndef WEB_FOLDER_ITEM_H
#define WEB_FOLDER_ITEM_H

#include "playlist.h"

namespace Core {
    class WebPlaylist : public Playlist {
    public:
        inline WebFolderItem(int initState = DEFAULT_PLAYLIST_STATE) : Playlist(initState) {}
        inline WebFolderItem(QJsonObject * hash, Playlist * parent = 0) : Playlist(hash, parent) {}
        inline WebFolderItem(QString uid, QString folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_PLAYLIST_STATE)
            : Playlist(folderTitle, parent, uid, pos, initState) {}

        inline virtual ~WebPlaylist() {}

        inline QUrl toUrl() { return QUrl(path().toString()); }
        inline bool isRemote() const { return true; }
    };
}

#endif // WEB_FOLDER_ITEM_H
