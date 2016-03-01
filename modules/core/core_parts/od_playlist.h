#ifndef OD_FOLDER_H
#define OD_FOLDER_H

#include "web_playlist.h"

namespace Core {
    class OdPlaylist : public WebPlaylist {
    public:
        inline OdPlaylist(int initState) : WebPlaylist(initState) {}
        inline OdPlaylist(QJsonObject * hash, Playlist * parent = 0) : WebPlaylist(hash, parent) {}
        inline OdPlaylist(const QString & uid, const QString & folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_ITEM_STATE)
            : WebPlaylist(uid, folderTitle, parent, pos, initState) {}

        inline int itemType() const { return OD_PLAYLIST; }

        inline bool removePhysicalObject() {
            //TODO: add realization
            return false;
        }
        inline bool isExist() const {
            //TODO: add realization
            return true;
        }
    };
}

#endif // OD_FOLDER_H
