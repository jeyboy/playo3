#ifndef CUE_FOLDER_H
#define CUE_FOLDER_H

#include "playlist.h"

namespace Core {
    class CuePlaylist : public Playlist {
    public:
        inline CuePlaylist(int initState) : Playlist(initState) {}
        inline CuePlaylist(QJsonObject * hash, Playlist * parent = 0) : Playlist(hash, parent) {}
        CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_PLAYLIST_STATE);

        int initFiles(QHash<QString, bool> & filePathes);

        inline int itemType() const { return CUE_PLAYLIST; }

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

#endif // CUE_FOLDER_H
