#include "cue.h"

Cue::Cue(QIODevice & obj) {
    initRules();

    QTextStream in(&obj);
//    QString str_Line = in.readLine();
}

void Cue::initRules() {

}

//inline void parseCatalog(QString & line) {
//    QRegularExpression regex("<ca>(.*)</ca>", QRegularExpression::MultilineOption);
//    QRegularExpressionMatch match = regex.match(content);
//    QString ca = match.captured(1);
//}


//REM GENRE Rock
//REM DATE 2003
//REM DISCID 24059504
//REM COMMENT "ExactAudioCopy v0.95b3"

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


