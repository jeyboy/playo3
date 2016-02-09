#include "cue.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/misc/file_utils/filesystem_watcher.h"
#include <qdiriterator.h>
#include <qdebug.h>

using namespace Core::Media;

Cue::Cue(const QString & filePath, QIODevice & obj) : level(0) {
    qDebug() << "---------   CUETA   ------------------ " << filePath;
    path = FilenameConversions::takePath(filePath);

    QTextStream in(&obj);
//    FileSystemWatcher::setEncoding(in);

    while(!in.atEnd()) {
        QString str = in.readLine();
        proceedLine(str);
    }
}

Cue * Cue::fromPath(const QString & path) {
    QFile f(path);
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        Cue * cue = new Cue(path, f);
        f.close();
        return cue;
    }

    return 0;
}

void Cue::identifyFile(QString & file_path, QString & file_extension, bool isShareable) {
    if (!QFile::exists(file_path)) {
        QString tPath = path % '/' % file_path;
        if (!QFile::exists(tPath)) {
            tPath = path;
            QString tExt, tName = file_path.split('/', QString::SkipEmptyParts).last();
            Extensions::obj().extractExtension(tName, tExt);

            bool findCompatible = false;
            QDirIterator dir_it(
                tPath,
                QStringList() << (tName.replace(QRegularExpression("[\\W]+"), QStringLiteral("*")) % QStringLiteral(".*")/* % tExt*/), // there is not possible to use extension from filename
                QDir::NoDotAndDotDot | QDir::Files,
                QDirIterator::Subdirectories
            );

            while(dir_it.hasNext()) {
                QString eq_str = dir_it.next();
                // in some cases extension is not eq to cue type and there we should to reinit real extension :(
                if (!eq_str.endsWith(QStringLiteral(".cue"))) {
                    findCompatible = true;
                    file_path = eq_str;
                    Extensions::obj().extractExtension(file_path, file_extension, false);
                    break;
                }
            }

            // try to find media file - if cue part contains more then 1 part
            if (!findCompatible && isShareable) {
                QDirIterator dir_it(path, Extensions::obj().filterList(MUSIC_PRESET), QDir::NoDotAndDotDot | QDir::Files);

                while(dir_it.hasNext()) {
                    file_path = dir_it.next();
                    Extensions::obj().extractExtension(file_path, file_extension, false);
                    break;
                }
            }
        } else file_path = tPath;
    }

    #ifdef Q_OS_WIN // windows allow to use case insensitive extensions - but we required on sharp name
    {
        QString tPath = file_path, tName, tExt;
        FilenameConversions::splitPath(tPath, tName);
        Extensions::obj().extractExtension(tName, tExt);

        QDir dir(tPath, (tName.replace(QRegularExpression("[\\W]+"), QStringLiteral("*")) % QStringLiteral(".") % tExt.toLower()));
        QStringList files = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        if (!files.isEmpty()) file_path = tPath % '/' % files.first();
    }
    #endif
}

QList<CueSong> Cue::songs() { // last element always missed at duration
    QList<CueSong> res;
    int group = 0;

    for(QList<CueFile *>::Iterator file = _files.begin(); file != _files.end(); file++, group++) {
        bool isShareable = (*file) -> tracks.length() > 1;
        QString file_path = QDir::fromNativeSeparators((*file) -> path);
        QString real_extension = (*file) -> extension;
        QHash<qint64, bool> time_marks;

        identifyFile(file_path, real_extension, isShareable);
//        if (!QFile::exists(file_path)) {
//            QString tPath = path % '/' % file_path;
//            if (!QFile::exists(tPath)) {
//                tPath = path;
//                QString tExt, tName = file_path.split('/', QString::SkipEmptyParts).last();
//                Extensions::obj().extractExtension(tName, tExt);

//                bool findCompatible = false;
//                QDirIterator dir_it(
//                    tPath,
//                    QStringList() << (tName.replace(QRegularExpression("[\\W]+"), QStringLiteral("*")) % QStringLiteral(".*")/* % tExt*/), // there is not possible to use extension from filename
//                    QDir::NoDotAndDotDot | QDir::Files,
//                    QDirIterator::Subdirectories
//                );

//                while(dir_it.hasNext()) {
//                    QString eq_str = dir_it.next();
//                    // in some cases extension is not eq to cue type and there we should to reinit real extension :(
//                    if (!eq_str.endsWith(QStringLiteral(".cue"))) {
//                        findCompatible = true;
//                        file_path = eq_str;
//                        Extensions::obj().extractExtension(file_path, real_extension, false);
//                        break;
//                    }
//                }

//                // try to find media file - if cue part contains more then 1 part
//                if (!findCompatible && isShareable) {
//                    QDirIterator dir_it(path, Extensions::obj().filterList(MUSIC_PRESET), QDir::NoDotAndDotDot | QDir::Files);

//                    while(dir_it.hasNext()) {
//                        file_path = dir_it.next();
//                        Extensions::obj().extractExtension(file_path, real_extension, false);
//                        break;
//                    }
//                }
//            } else file_path = tPath;
//        }

//        #ifdef Q_OS_WIN // windows allow to use case insensitive extensions - but we required on sharp name
//        {
//            QString tPath = file_path, tName, tExt;
//            FilenameConversions::splitPath(tPath, tName);
//            Extensions::obj().extractExtension(tName, tExt);

//            QDir dir(tPath, (tName.replace(QRegularExpression("[\\W]+"), QStringLiteral("*")) % QStringLiteral(".") % tExt.toLower()));
//            QStringList files = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
//            if (!files.isEmpty()) file_path = tPath % '/' % files.first();
//        }
//        #endif

        for(QList<CueTrack *>::Iterator track = (*file) -> tracks.begin(); track != (*file) -> tracks.end(); track++)
            for(QList<CueTrackIndex *>::Iterator index = (*track) -> indexes.begin(); index != (*track) -> indexes.end(); index++) {
                if ((*index) -> number == 0) continue;

                QString error;
                qint64 time_mark = (*index) -> toMillis();
                if (time_marks.contains(time_mark)) {
                    error = QStringLiteral("Wrong time mark %1").arg(QString::number(time_mark));
                    qDebug() << error;
                    time_mark = 0;
                }


                QString title = (*track) -> toStr();

                if (title.isEmpty()) {
                    QString tPath = file_path, tExt;
                    FilenameConversions::splitPath(tPath, title);
                    Extensions::obj().extractExtension(title, tExt);
                }

                res.append(CueSong(
                    time_mark, // need to add correction on pregap for current item and postgap for next
                    title,
                    file_path,
                    real_extension,
                    isShareable,
                    group,
                    error
                ));

                time_marks.insert(time_mark, true);
            }

        for(QList<CueTrackIndex *>::Iterator index = (*file) -> indexes.begin(); index != (*file) -> indexes.end(); index++) {
            if ((*index) -> number == 0) continue;

            res.append(CueSong(
                (*index) -> toMillis(),
                file_path,
                file_path,
                real_extension,
                true,
                group
            ));
        }
    }

    return res;
}

void Cue::splitLine(QString & line, QList<QString> & res) {
    bool locked = false;
    int pos = 0, offset = 0;
    line.remove('\t'); // better do this in loop - but string is not supported removing through iterator :(
    for(QString::iterator ch = line.begin(); ch != line.end(); ch++, offset++) {
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
        QString token = parts.takeFirst().toUpper();

        if (token.startsWith(';')) {
            qDebug() << "CUE COMMENT" << line;
            return;
        }

        if (parts.isEmpty()) {
            qDebug() << "WRONG CUE TAG" << line;
            return;
        }

        if (level == 0 && token == QStringLiteral("TRACK")) // fix for non standart builded cue
            level = 1;

        while(level > -1) {
            switch(level) {
                case 0: {
                    if (token == QStringLiteral("REM")) {
                        if (parts.length() != 2)
                            qDebug() << "WRONG CUE TAG" << line;
                        else
                            _infos.insert(parts[0], parts[1]);
                        return;
                    } else if (token == QStringLiteral("TITLE")) {
                        title = parts[0]; return;
                    } else if (token == QStringLiteral("SONGWRITER")) {
                        songwriter = parts[0]; return;
                    } else if (token == QStringLiteral("CATALOG")) {
                        catalog = parts[0]; return;
                    } else if (token == QStringLiteral("CDTEXTFILE")) {
                        text_file = parts[0]; return;
                    } else if (token == QStringLiteral("FILE")) {
                        if (parts.length() != 2)
                            qDebug() << "WRONG CUE TAG" << line;
                        else {
                            level++;
                            addFile(parts[0], parts[1]); return;
                        }
                    } else if (token == QStringLiteral("PERFORMER")) {
                        performer = parts[0]; return;
                    }
                break;}

                case 1: {
                    if (token == QStringLiteral("TRACK")) {
                        if (parts.length() != 2)
                            qDebug() << "WRONG CUE TAG" << line;
                        else {
                            level++;
                            activeFile -> addTrack(parts[0], parts[1]); return;
                        }
                    } else if (token == QStringLiteral("INDEX")) {
                        if (parts.length() != 2)
                            qDebug() << "WRONG CUE TAG" << line;
                        else
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
                        if (parts.length() != 2)
                            qDebug() << "WRONG CUE TAG" << line;
                        else
                            activeFile -> activeTrack -> addIndex(parts[0], parts[1]); return;
                    } else if (token == QStringLiteral("PREGAP")) {
                        activeFile -> activeTrack -> setPregap(parts[0]); return;
                    } else if (token == QStringLiteral("POSTGAP")) {
                        activeFile -> activeTrack -> setPostgap(parts[0]); return;
                    } else if (token == QStringLiteral("REM")) { // specification did not contain this case, but some cue generators inserted REM into track
                        if (parts.length() != 2)
                            qDebug() << "WRONG CUE TAG" << line;
                        else
                            activeFile -> activeTrack -> addInfo(parts[0], parts[1]); return;
                    }
                break;}

                default: qDebug() << QStringLiteral("UNSUPPORTED PREDICATE") << parts;
            }

            level--;
        }
    }

    if (level == -1) {
        qDebug() << path << QStringLiteral("ERROR LEVEL") << line;
        level = 0;
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


