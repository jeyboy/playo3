#include "cue_playlist.h"
#include "cue_file.h"
#include "modules/core/media/cue/cue.h"

using namespace Core;

CuePlaylist::CuePlaylist(const QString & filePath, const QString & fileTitle, Playlist * parent, int pos, int initState)
    : Playlist(filePath, fileTitle, parent, pos, initState) {
    Media::Cue * cue = Media::Cue::fromPath(filePath);
    QMap<qint64, QPair<QString, QString> > songs = cue -> songs();

    for(QMap<qint64, QPair<QString, QString> >::Iterator song = songs.begin(); song != songs.end(); song++) {
        qDebug() << "CUETA" << song.key() << song.value();
        new CueFile(song.key(), song.value().first, song.value().second, this);
    }

    delete cue;
}
