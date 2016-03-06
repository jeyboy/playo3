#include "cue_playlist.h"
#include "cue_file.h"
#include "modules/core/media/cue/cue.h"

using namespace Core;

CuePlaylist::CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent, int pos, int initState)
    : Playlist(filePath, fileTitle, parent, pos, initState) {}

int CuePlaylist::initFiles(QHash<QString, bool> & filePathes, QHash<QString, IItem *> & existed) {
    Media::Cue * cue = Media::Cue::fromPath(path().toString());
    QList<Media::CueSong> songs = cue -> songs();
    QHash<QString, QString> ignore;

    for(QList<Media::CueSong>::Iterator song = songs.begin(); song != songs.end(); song++) {
        QString songPath = (*song).filePath;
        bool res = true;

        if (ignore.contains((*song).filePath))
            res = false;
        else if (!filePathes.contains(songPath)) {
            //TODO: temp solution for removing from list already added cue parts
            if (!existed.isEmpty()) {
                IItem * itm = existed.take(songPath);
                if (itm) {
                    if (itm -> parent() -> childCount() == 1)
                        itm -> parent() -> removeYouself();
                    else
                        itm -> removeYouself();
                }
            }


            res = QFile::exists(songPath);

            if (!res) {
              //TODO: ask user about manual choosing of media source for cue
            }
        }

        //TODO: maybe add error field to the item ?
        Core::CueFile * f = new Core::CueFile((*song).startPos, (*song).duration, songPath, (*song).trackName, (*song).extension, (*song).isPartial, this);
        if (res) {
            filePathes.insert(songPath, true);
            if (!(*song).error.isEmpty()) // there should be other error status ?
                f -> setError(ItemErrors::err_not_existed);
        } else {
            filePathes.insert(songPath, false);
            ignore.insert(songPath, QString());
            f -> setError(ItemErrors::err_not_existed);
        }
    }

    delete cue;
    updateItemsCountInBranch(childCount());
    return childCount();
}
