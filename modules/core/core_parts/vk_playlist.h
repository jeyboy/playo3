#ifndef VK_FOLDER_H
#define VK_FOLDER_H

#include "web_playlist.h"

namespace Core {
    class VkPlaylist : public WebPlaylist {
    public:
        inline VkPlaylist(int initState) : WebPlaylist(initState) {}
        inline VkPlaylist(QJsonObject * hash, Playlist * parent = 0) : WebPlaylist(hash, parent) {}
        inline VkPlaylist(const QString & uid, const QString & folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_ITEM_STATE)
            : WebPlaylist(uid, folderTitle, parent, pos, initState) {}
    };
}

#endif // VK_FOLDER_H
