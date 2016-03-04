#ifndef WEB_FOLDER_ITEM_H
#define WEB_FOLDER_ITEM_H

#include "playlist.h"

namespace Core {
    class WebPlaylist : public Playlist {
    public:
        inline WebPlaylist(int initState = DEFAULT_ITEM_STATE) : Playlist(initState) {}
        inline WebPlaylist(QJsonObject * hash, Playlist * parent = 0) : Playlist(hash, parent) {}
        inline WebPlaylist(const QString & uid, const QString & folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_ITEM_STATE)
            : Playlist(folderTitle, parent, uid, pos, initState) {}

        inline virtual ~WebPlaylist() {}
    };
}

#endif // WEB_FOLDER_ITEM_H
