#include "cue_playlist.h"
#include "cue_file.h"
#include "modules/core/media/cue/cue.h"

using namespace Core;

CuePlaylist::CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent, int pos, int initState)
    : Playlist(filePath, fileTitle, parent, pos, initState) {}

int CuePlaylist::initFiles(QHash<QString, bool> & filePathes, QStringList & unprocable) {
    Media::Cue * cue = Media::Cue::fromPath(path().toString());
    QList<Media::CueSong> songs = cue -> songs();
    QHash<QString, QString> ignore;

    for(QList<Media::CueSong>::Iterator song = songs.begin(); song != songs.end(); song++) {
        QString songPath = (*song).filePath;
        bool res = true;

        if (ignore.contains((*song).filePath))
            res = false;
        else if (!filePathes.contains(songPath)) {
            unprocable << songPath;
            res = QFile::exists(songPath);

            if (!res) {
              //TODO: ask user about manual choosing of media source for cue
            }
        }

        //TODO: maybe add error field to the item ?
        CueFile * f = new CueFile((*song).startPos, (*song).duration, songPath, (*song).trackName, (*song).extension, (*song).isPartial, this);
        if (res) {
            filePathes.insert(songPath, true);
            if (!(*song).error.isEmpty())
                f -> set(not_exist);
        } else {
            filePathes.insert(songPath, false);
            ignore.insert(songPath, QString());
            f -> set(not_exist);
        }
    }

    delete cue;
    updateItemsCountInBranch(childCount());
    return childCount();
}
