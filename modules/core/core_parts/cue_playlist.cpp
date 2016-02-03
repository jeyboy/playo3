#include "cue_playlist.h"
#include "cue_file.h"
#include "modules/core/media/cue/cue.h"

using namespace Core;

CuePlaylist::CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent, int pos, int initState)
    : Playlist(filePath, fileTitle, parent, pos, initState) {
    Media::Cue * cue = Media::Cue::fromPath(filePath);
    QList<Media::CueSong> songs = cue -> songs();

    for(QList<Media::CueSong>::Iterator song = songs.begin(); song != songs.end(); song++) {
        qint64 duration = ((song + 1) != songs.end()) ? (*(song + 1)).startPos - (*song).startPos : 0;
        qDebug() << "CUETA" << (*song).startPos << duration << (*song).trackName << (*song).extension << (*song).filePath;
        new CueFile((*song).startPos, duration, (*song).filePath, (*song).trackName, (*song).extension, this);
    }

    delete cue;
}
