#include "cue_playlist.h"
#include "cue_file.h"
#include "modules/core/media/cue/cue.h"

#include <qdiriterator.h>

using namespace Core;

CuePlaylist::CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent, int pos, int initState)
    : Playlist(filePath, fileTitle, parent, pos, initState) {}

int CuePlaylist::initFiles(QHash<QString, bool> & filePathes) {
    Media::Cue * cue = Media::Cue::fromPath(path().toString());
    QList<Media::CueSong> songs = cue -> songs();
    QHash<QString, QString> redirects, ignore;

    for(QList<Media::CueSong>::Iterator song = songs.begin(); song != songs.end(); song++) {
        QString songPath = redirects.value((*song).filePath, (*song).filePath);
        bool res = true;

        if (ignore.contains((*song).filePath))
            res = false;
        else if (!filePathes.contains(songPath)) {
            res = QFile::exists(songPath);

            if (!res) {
                QString tPath = path().toString(), tExt, tName;
                FilenameConversions::splitPath(tPath, tName);

                if (Extensions::obj().extractExtension(tName, tExt)) { // maybe check of result is not needed?
                    QDirIterator dir_it(
                        tPath,
                        QStringList() << (tName % QStringLiteral("*.") % (*song).extension),
                        QDir::NoDotAndDotDot | QDir::Files,
                        QDirIterator::Subdirectories
                    );

                    while(dir_it.hasNext()) {
                        songPath = dir_it.next();
                        redirects.insert(path().toString(), songPath);
                        res = true;
                        break;
                    }
                }
            }

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
