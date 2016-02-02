#include "cue.h"
#include <qdir.h>
#include <qdebug.h>

using namespace Core::Media;

Cue::Cue(const QString & filePath, QIODevice & obj) : level(0) {
    QStringList parts = filePath.split('/', QString::SkipEmptyParts);
    parts.removeLast();
    path = parts.join('/');

    QTextStream in(&obj);

    while(!in.atEnd()) {
        QString str = in.readLine();
        proceedLine(str);
    }
}

QMap<qint64, QPair<QString, QString> > Cue::songs() {
    QMap<qint64, QPair<QString, QString> > res;

    for(QList<CueFile *>::Iterator file = _files.begin(); file != _files.end(); file++) {
        QString file_path = QDir::fromNativeSeparators((*file) -> path);

        if (!file_path.contains('/'))
            file_path = path % '/' % file_path;

        for(QList<CueTrack *>::Iterator track = (*file) -> tracks.begin(); track != (*file) -> tracks.end(); track++)
            for(QList<CueTrackIndex *>::Iterator index = (*track) -> indexes.begin(); index != (*track) -> indexes.end(); index++)
                res.insert((*index) -> toMillis(), QPair<QString, QString>(file_path, (*track) -> toStr()));

        for(QList<CueTrackIndex *>::Iterator index = (*file) -> indexes.begin(); index != (*file) -> indexes.end(); index++)
            res.insert((*index) -> toMillis(), QPair<QString, QString>(file_path, file_path));
    }

    return res;
}

void Cue::splitLine(QString & line, QList<QString> & res) {
    bool locked = false;
    int pos = 0, offset = 0;
    for(QString::iterator ch = line.begin(); ch != line.end(); ch++, offset++)
        switch((*ch).unicode()) {
            case 32: {
                if (!locked) {
                    QStringRef str = line.midRef(pos, offset - pos);
                    if (!str.isEmpty()) {
                        CHOP_STR(str);
                        res.append(str.toString());
                    }
                    pos = offset + 1;
                }
            break;}
            case 34: { locked = !locked; break;}
        }

    QStringRef str = line.midRef(pos, offset);
    if (!str.isEmpty()) {
        CHOP_STR(str);
        res.append(str.toString());
    }
}

void Cue::proceedLine(QString & line) {
    QList<QString> parts;
    splitLine(line, parts);
    if (!parts.isEmpty()) {
        QString token = parts.takeFirst();

        while(level > -1) {
            switch(level) {
                case 0: {
                    if (token == QStringLiteral("REM")) {
                        _infos.insert(parts[0], parts[1]); return;
                    } else if (token == QStringLiteral("TITLE")) {
                        title = parts[0]; return;
                    } else if (token == QStringLiteral("SONGWRITER")) {
                        songwriter = parts[0]; return;
                    } else if (token == QStringLiteral("CATALOG")) {
                        catalog = parts[0]; return;
                    } else if (token == QStringLiteral("CDTEXTFILE")) {
                        text_file = parts[0]; return;
                    } else if (token == QStringLiteral("FILE")) {
                        level++;
                        addFile(parts[0], parts[1]); return;
                    } else if (token == QStringLiteral("PERFORMER")) {
                        performer = parts[0]; return;
                    }
                break;}

                case 1: {
                    if (token == QStringLiteral("TRACK")) {
                        level++;
                        activeFile -> addTrack(parts[0], parts[1]); return;
                    } else if (token == QStringLiteral("INDEX")) {
                        activeFile -> addIndex(parts[0], parts[1]); return;
                    }
                break;}

                case 2: {
                    if (token == QStringLiteral("TITLE")) {
                        activeFile -> activeTrack -> title = parts[0]; return;
                    } else if (token == QStringLiteral("SONGWRITER")) {
                        activeFile -> activeTrack -> songwriter = parts[0]; return;
                    } else if (token == QStringLiteral("ISRC")) {
                        activeFile -> activeTrack -> isrc = parts[0]; return;
                    } else if (token == QStringLiteral("PERFORMER")) {
                        activeFile -> activeTrack -> performer = parts[0]; return;
                    } else if (token == QStringLiteral("FLAGS")) {
                        activeFile -> activeTrack -> parseFlags(parts); return;
                    } else if (token == QStringLiteral("INDEX")) {
                        activeFile -> activeTrack -> addIndex(parts[0], parts[1]); return;
                    } else if (token == QStringLiteral("PREGAP")) {
                        activeFile -> activeTrack -> setPregap(parts[0]); return;
                    } else if (token == QStringLiteral("POSTGAP")) {
                        activeFile -> activeTrack -> setPostgap(parts[0]); return;
                    }
                break;}

                default: qDebug() << QStringLiteral("UNSUPPORTED PREDICATE") << parts;
            }

            level--;
        }
    }

    if (level == -1)
        qDebug() << QStringLiteral("ERROR LEVEL") << line;
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


