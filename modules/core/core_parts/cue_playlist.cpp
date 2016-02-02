#include "cue_playlist.h"
#include "cue_file.h"
#include "modules/core/media/cue/cue.h"

using namespace Core;

CuePlaylist::CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent, int pos, int initState)
    : Playlist(filePath, fileTitle, parent, pos, initState) {
    Media::Cue * cue = Media::Cue::fromPath(filePath);
    QMap<qint64, QString> songs = cue -> songs();

    for(QMap<qint64, QString>::Iterator song = songs.begin(); song != songs.end(); song++)
        new CueFile(song.key(), song.value(), this);

    delete cue;
}
