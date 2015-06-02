#include "cue.h"
#include <qdebug.h>

Cue::Cue(QIODevice & obj) : level(0) {
    QTextStream in(&obj);

    while(!in.atEnd()) {
        QString str = in.readLine();
        proceedLine(str);
    }
}

QMap<qint64, QString> Cue::songs() {
    QMap<qint64, QString> res;

    for(QList<CueFile *>::Iterator file = _files.begin(); file != _files.end(); file++) {
        for(QList<CueTrack *>::Iterator track = (*file) -> tracks.begin(); track != (*file) -> tracks.end(); track++) {
            for(QList<CueTrackIndex *>::Iterator index = (*track) -> indexes.begin(); index != (*track) -> indexes.end(); index++) {
                res.insert((*index) -> toMillis(), (*track) -> toStr());
            }
        }

        for(QList<CueTrackIndex *>::Iterator index = (*file) -> indexes.begin(); index != (*file) -> indexes.end(); index++) {
            res.insert((*index) -> toMillis(), (*file) -> path);
        }
    }

    return res;
}

QList<QString> Cue::splitLine(QString & line) {
    QList<QString> res = line.split(' ', QString::SkipEmptyParts);

    QMutableListIterator<QString> i(res);
    bool has_token = false;
    QString token;

    while (i.hasNext()) {
        QString val = i.next();

        if (val.startsWith('"')) {
            has_token = true;
            token = val.mid(1);
        }
        else if (has_token)
            token += ' ' + val;

        if (has_token) {
            if (!(has_token = !token.endsWith('"'))) {
                token.chop(1);
                i.setValue(token);
            } else
                i.remove();
        }
    }

    return res;
}

void Cue::proceedLine(QString & line) {
    QList<QString> parts = splitLine(line);
    if (!parts.isEmpty()) {
        QString token = parts.takeFirst();

        while(level > -1) {
            switch(level) {
                case 0: {
                    if (token == "REM") {
                        _infos.insert(parts[0], parts[1]); return;
                    } else if (token == "TITLE") {
                        title = parts[0]; return;
                    } else if (token == "SONGWRITER") {
                        songwriter = parts[0]; return;
                    } else if (token == "CATALOG") {
                        catalog = parts[0]; return;
                    } else if (token == "CDTEXTFILE") {
                        text_file = parts[0]; return;
                    } else if (token == "FILE") {
                        level++;
                        addFile(parts[0], parts[1]); return;
                    } else if (token == "PERFORMER") {
                        performer = parts[0]; return;
                    }
                break;}

                case 1: {
                    if (token == "TRACK") {
                        level++;
                        activeFile -> addTrack(parts[0], parts[1]); return;
                    } else if (token == "INDEX") {
                        activeFile -> addIndex(parts[0], parts[1]); return;
                    }
                break;}

                case 2: {
                    if (token == "TITLE") {
                        activeFile -> activeTrack -> title = parts[0]; return;
                    } else if (token == "SONGWRITER") {
                        activeFile -> activeTrack -> songwriter = parts[0]; return;
                    } else if (token == "ISRC") {
                        activeFile -> activeTrack -> isrc = parts[0]; return;
                    } else if (token == "PERFORMER") {
                        activeFile -> activeTrack -> performer = parts[0]; return;
                    } else if (token == "FLAGS") {
                        activeFile -> activeTrack -> parseFlags(parts); return;
                    } else if (token == "INDEX") {
                        activeFile -> activeTrack -> addIndex(parts[0], parts[1]); return;
                    } else if (token == "PREGAP") {
                        activeFile -> activeTrack -> setPregap(parts[0]); return;
                    } else if (token == "POSTGAP") {
                        activeFile -> activeTrack -> setPostgap(parts[0]); return;
                    }
                break;}

                default: qDebug() << "UNSUPPORTED PREDICATE" << parts;
            }

            level--;
        }
    }

    if (level == -1) {
        qDebug() << "ERROR LEVEL" << line;
    }
}

//REM (comment)
//TITLE [title-string] // not mandatory quotation marks // Strings should be limited to 80 character or less.
//SONGWRITER [songwriter-string] // not mandatory quotation marks // Strings should be limited to 80 character or less.
//CATALOG [media-catalog-number] // 13 digits
//CDTEXTFILE [filename] // not mandatory quotation marks
//FILE [filename] [filetype] // not mandatory quotation marks // AudioFormat
//  TRACK [number] [datatype] // 1 - 99 // TrackDataType
//      TITLE [title-string] // not mandatory quotation marks // Strings should be limited to 80 character or less.
//      SONGWRITER [songwriter-string] // not mandatory quotation marks // Strings should be limited to 80 character or less.
//      INDEX [number] [mm:ss:ff] // MAXINDEX // mm:ss:ff – Starting time in minutes, seconds, and frames (75 frames/second). All times are relative to the beginning of the current file.
//      FLAGS [flags] // one or more // AudioFlag
//      ISRC (code) // http://isrc.ifpi.org/en/ // The ISRC must be 12 characters in length. The first five characters are alphanumeric, but the last seven are numeric only.
//      PERFORMER [performer-string] // not mandatory quotation marks // Strings should be limited to 80 character or less.
//      PREGAP [mm:ss:ff] // mm:ss:ff – Specifies the pregap length in minutes, seconds, and frames. // must appear after a TRACK command, but before any INDEX commands. Only one PREGAP command is allowed per track
//      POSTGAP [mm:ss:ff] // mm:ss:ff – Specifies the postgap length in minutes, seconds, and frames. // must appear after all INDEX commands for the current track. Only one POSTGAP command is allowed per track.
//


