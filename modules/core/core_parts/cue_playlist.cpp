#include "cue_playlist.h"
#include "cue_file.h"
#include "modules/core/media/cue/cue.h"

using namespace Core;

CuePlaylist::CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent, int pos, int initState)
    : Playlist(filePath, fileTitle, parent, pos, initState) {
    Media::Cue * cue = Media::Cue::fromPath(filePath);
    QList<Media::CueSong> songs = cue -> songs();

    for(QList<Media::CueSong>::Iterator song = songs.begin(); song != songs.end(); song++) {
        qDebug() << "CUETA" << (*song).startPos << (*song).trackName << (*song).extension << (*song).filePath;
        new CueFile((*song).startPos, (*song).filePath, (*song).trackName, (*song).extension, this);
    }

    delete cue;
}
