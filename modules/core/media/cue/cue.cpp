#include "cue.h"
#include "modules/core/misc/logger.h"
#include "modules/core/misc/file_utils/filename_conversions.h"

#include <qdiriterator.h>

using namespace Core::Media;

QHash<QString, Cue::CueTokens> Cue::tokens{
    {QLatin1String("REM"), Cue::rem},
    {QLatin1String("TITLE"), Cue::title},
    {QLatin1String("SONGWRITER"), Cue::songwriter},
    {QLatin1String("CATALOG"), Cue::catalog},
    {QLatin1String("CDTEXTFILE"), Cue::cdtextfile},
    {QLatin1String("FILE"), Cue::file},
    {QLatin1String("PERFORMER"), Cue::performer},

    {QLatin1String("TRACK"), Cue::track},
    {QLatin1String("INDEX"), Cue::index},

    {QLatin1String("ISRC"), Cue::isrc},
    {QLatin1String("FLAGS"), Cue::flags},
    {QLatin1String("PREGAP"), Cue::pregap},
    {QLatin1String("POSTGAP"), Cue::postgap}
};


Cue::Cue(const QString & filePath, QIODevice & obj) : level(0), path(filePath) {
    Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("INIT"), QStringList() << filePath);
    QString ext;
    FilenameConversions::splitPath(path, filename);
    Extensions::obj().extractExtension(filename, ext);

    QTextStream in(&obj);
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

bool Cue::identifyFile(QString & file_path, QString & file_extension, bool isShareable) { // this method is very slow :(
    bool res = QFile::exists(file_path);

    if (!res) {
        QString tPath = path % '/' % file_path;
        res = QFile::exists(tPath);

        if (!res) {
            tPath = path;
            QString tExt, tName = file_path.split('/', QString::SkipEmptyParts).last();
            Extensions::obj().extractExtension(tName, tExt);

            QStringList music_filters = Extensions::obj().filterList(MUSIC_PRESET);
            // try to find media file with name of cue or with name from cue file
            QDirIterator dir_it(tPath, QStringList() << PREPARE_SEARCH_PREDICATE(tName, QLatin1String("*")) << PREPARE_SEARCH_PREDICATE(filename, QLatin1String("*")),
                QDir::NoDotAndDotDot | QDir::Files, QDirIterator::Subdirectories
            );

            while(dir_it.hasNext()) {
                // in some cases extension is not eq to cue type and there we should to reinit real extension :(
                if (QDir::match(music_filters, dir_it.next())) {
                    res = true;
                    file_path = dir_it.filePath();
                    Extensions::obj().extractExtension(file_path, file_extension, false);
                    break;
                }
            }

            // if cue part contains more then 1 part (related to one big file) try to find any media file
            if (!res && isShareable) {
                QDirIterator dir_it(path, music_filters, QDir::NoDotAndDotDot | QDir::Files);

                while(dir_it.hasNext()) {
                    res = true;
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

        QDirIterator dir_it(tPath, QStringList() << PREPARE_SEARCH_PREDICATE(tName, tExt.toLower()), QDir::NoDotAndDotDot | QDir::Files);
        while(dir_it.hasNext()) {
            file_path = dir_it.next();
            break;
        }
    }
    #endif

    return res;
}

int Cue::buildItems(Playlist * cuePlaylist, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items) { // last element always missed at duration
    QVariantMap cue_map;
    int amount = 0;

    for(QList<CueFile *>::Iterator file = _files.begin(); file != _files.end(); file++) {
        bool isShareable = (*file) -> tracks.length() > 1;
        QString file_path = QDir::fromNativeSeparators((*file) -> path);
        QString real_extension = (*file) -> extension;
        QHash<qint64, bool> time_marks;
        IItem * prev_ditem = 0;
        int prev_mark = 0;

        if (cue_map.contains(file_path)) continue;

        bool exists = identifyFile(file_path, real_extension, isShareable);
        if (!exists) {
          //TODO: ask user about manual choosing of media source for cue
        }


        //////// TODO: temp solution for removing from list already added cue parts
        if (!unproc_files.contains(file_path)) {
            if (!items.isEmpty()) {
                IItem * itm = items.take(file_path);
                if (itm) {
                    if (itm -> parent() -> childCount() == 1)
                        itm -> parent() -> removeYouself();
                    else
                        itm -> removeYouself();
                }
            }
        }
        ////////

        for(QList<CueTrack *>::Iterator track = (*file) -> tracks.begin(); track != (*file) -> tracks.end(); track++)
            for(QList<CueTrackIndex *>::Iterator index = (*track) -> indexes.begin(); index != (*track) -> indexes.end(); index++) {
                if ((*index) -> number == 0) continue;

                QString error;
                qint64 time_mark = (*index) -> toMillis();
                if (time_marks.contains(time_mark)) {
                    error = QLatin1String("Wrong time mark ") % QString::number(time_mark);
                    Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("PARSING"), QStringList() << error, Logger::log_error);
                    time_mark = 0;
                }

                QString title = (*track) -> toStr();

                if (title.isEmpty()) {
                    QString tPath = file_path, tExt;
                    FilenameConversions::splitPath(tPath, title);
                    Extensions::obj().extractExtension(title, tExt);
                }

                //////////////////////////////////////////////////////////////

                IItem * ditem = new IItem(cuePlaylist, LOCAL_CUE_ITEM_ATTRS(
                    file_path, title, real_extension, time_mark, isShareable
                ));

                if (prev_ditem)
                    prev_ditem -> setDuration(Duration::fromMillis(time_mark - prev_mark));


                if (exists) {
                    if (!error.isEmpty()) // there should be other error status ?
                        ditem -> setError(ItemErrors::err_not_existed);
                }
                else ditem -> setError(ItemErrors::err_not_existed);

                prev_ditem = ditem;
                prev_mark = time_mark;
                amount++;
                cue_map[file_path] = cue_map.value(file_path, 0).toInt() + 1;

                //////////////////////////////////////////////////////////////
                time_marks.insert(time_mark, true);
            }       

        // this part is not tested on practice
        if (!(*file) -> indexes.isEmpty()) {
            prev_ditem = 0;
            prev_mark = 0;

            for(QList<CueTrackIndex *>::Iterator index = (*file) -> indexes.begin(); index != (*file) -> indexes.end(); index++) {
                if ((*index) -> number == 0) continue;
                QString error;
                qint64 time_mark = (*index) -> toMillis();
                if (time_marks.contains(time_mark)) {
                    error = QLatin1String("Wrong time mark ") % QString::number(time_mark);
                    Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("PARSING"), QStringList() << error, Logger::log_error);
                    time_mark = 0;
                }

                //////////////////////////////////////////////////////////////

                IItem * ditem = new IItem(cuePlaylist, LOCAL_CUE_ITEM_ATTRS(
                    file_path, title, real_extension, time_mark, isShareable
                ));

                if (prev_ditem)
                    prev_ditem -> setDuration(Duration::fromMillis(time_mark - prev_mark));

                if (exists) {
                    if (!error.isEmpty()) // there should be other error status ?
                        ditem -> setError(ItemErrors::err_not_existed);
                } else
                    ditem -> setError(ItemErrors::err_not_existed);

                prev_ditem = ditem;
                prev_mark = time_mark;
                amount++;
                cue_map[file_path] = cue_map.value(file_path, 0).toInt() + 1;

                //////////////////////////////////////////////////////////////
            }
        }

        unproc_files.insert(file_path, exists);
        cuePlaylist -> setCueMap(cue_map);
    }

    return amount;
}

void Cue::splitLine(QString & line, QList<QString> & res) {
    bool locked = false;
    int pos = 0, offset = 0;
//    line.remove('\t'); // better do this in loop - but string is not supported removing through iterator :(
    for(QString::iterator ch = line.begin(); ch != line.end(); ch++, offset++) {
        switch((*ch).unicode()) {
            case 9:
            case 32: {
                if (!locked) {
                    ADD_LINE_PART(res, line, pos, offset)
                    pos = offset + 1;
                }
            break;}
            case 34: { locked = !locked; break;}
        }
    }

    ADD_LINE_PART(res, line, pos, offset)
}

void Cue::proceedLine(QString & line) {
    if (line.startsWith(';')) {
        Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("COMMENT"), QStringList() << line);
        return;
    }

    QList<QString> parts;
    splitLine(line, parts);
    if (!parts.isEmpty()) {
        Cue::CueTokens token = Cue::tokens.value(parts.takeFirst().toUpper(), Cue::unknow);

        if (token == -1 || parts.isEmpty()) {
            Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("WRONG TAG"), QStringList() << line, Logger::log_error);
            return;
        }

        if (level == 0 && token == Cue::track) // fix for non standart builded cue
            level = 1;

        while(level > -1) {
            switch(level) {
                case 0: {
                    switch(token) {
                        case Cue::rem: {
                            if (parts.length() < 2)
                                Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("WRONG TAG"),
                                    QStringList() << line, Logger::log_error);
                            else _infos.insert(parts.takeFirst(), parts.join(' '));
                        return;}

                        case Cue::file: {
                            if (parts.length() != 2)
                                Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("WRONG TAG"),
                                    QStringList() << line, Logger::log_error);
                            else {
                                level++;
                                addFile(parts[0], parts[1]); return;
                            }
                        return;}

                        case Cue::catalog:
                        case Cue::cdtextfile: { return; }

                        case Cue::title:
                        case Cue::songwriter:
                        case Cue::performer: { _attrs.insert(token, parts[0]); return; }
                        default: ;
                    }
                break;}

                case 1: {
                    switch(token) {
                        case Cue::track: {
                            if (parts.length() != 2)
                                Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("WRONG TAG"),
                                    QStringList() << line, Logger::log_error);
                            else {
                                level++;
                                activeFile -> addTrack(parts[0], parts[1]); return;
                            }
                        return;}

                        case Cue::index: {
                            if (parts.length() < 2)
                                Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("WRONG TAG"),
                                    QStringList() << line, Logger::log_error);
                            else activeFile -> addIndex(parts[0], parts[1]); return;
                        return;}
                        default: ;
                    }
                break;}

                case 2: {
                    switch(token) {
                        case Cue::index: {
                            if (parts.length() < 2)
                                Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("WRONG TAG"),
                                    QStringList() << line, Logger::log_error);
                            else activeFile -> activeTrack -> addIndex(parts[0], parts[1]); return;
                        return;}

                        case Cue::title: { activeFile -> activeTrack -> title = parts[0]; return; }
                        case Cue::songwriter: { activeFile -> activeTrack -> songwriter = parts[0]; return; }
                        case Cue::isrc: { /*activeFile -> activeTrack -> isrc = parts[0];*/ return; }
                        case Cue::performer: { activeFile -> activeTrack -> performer = parts[0]; return; }
                        case Cue::flags: { /*activeFile -> activeTrack -> parseFlags(parts);*/ return; }

                        case Cue::pregap: { activeFile -> activeTrack -> setPregap(parts[0]); return; }
                        case Cue::postgap: { activeFile -> activeTrack -> setPostgap(parts[0]); return; }
                        // specification did not contain this case, but some cue generators inserted REM into track
                        // item store only ONE value
                        case Cue::rem: { /*activeFile -> activeTrack -> addInfo(parts.join(' '));*/ return; }
                        default: ;
                    }
                break;}

                default: Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("UNSUPPORTED PREDICATE"),
                            QStringList() << line, Logger::log_error);
            }

            level--;
        }
    }

    if (level == -1) {
        Logger::obj().write(QLatin1String("CUE PARSER"), QLatin1String("ERROR LEVEL"), QStringList() << line, Logger::log_error);
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
