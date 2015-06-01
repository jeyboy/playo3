#ifndef CUE_STRUCTS
#define CUE_STRUCTS

#include <QList>

#define MAXTRACK	99	/* Red Book track limit */
#define MAXINDEX	99	/* Red Book index limit */

enum AudioFormat {
    BINARY,		/* Intel binary file (least significant byte first) */
    MOTOROLA,	/* Motorola binary file (most significant byte first) */
    AIFF,		/* Audio AIFF file */
    WAVE,       /* Audio WAVE file */
    MP3         /* Audio MP3 file */
};

enum AudioFlag {
    FLAG_DCP,   /*Digital copy permitted*/
    FLAG_4CH,   /*Four channel audio*/
    FLAG_PRE,   /*Pre-emphasis enabled (audio tracks only)*/
    FLAG_SCMS   /*Serial copy management system (not supported by all recorders)*/
};

enum TrackDataType {
    AUDIO,      /*Audio/Music (2352)*/
    CDG,        /*Karaoke CD+G (2448)*/
    MODE1_2048, /*CDROM Mode1 Data (cooked)*/
    MODE1_2352, /*CDROM Mode1 Data (raw)*/
    MODE2_2336, /*CDROM-XA Mode2 Data*/
    MODE2_2352, /*CDROM-XA Mode2 Data*/
    CDI_2336,   /*CDI Mode2 Data*/
    CDI_2352    /*CDI Mode2 Data*/
};

///* disc modes
// * DATA FORM OF MAIN DATA (5.29.2.8)
// */
//enum DiscMode {
//    MODE_CD_DA,		/* CD-DA */
//    MODE_CD_ROM,		/* CD-ROM mode 1 */
//    MODE_CD_ROM_XA		/* CD-ROM XA and CD-I */
//};

///* track modes
// * 5.29.2.8 DATA FORM OF MAIN DATA
// * Table 350 - Data Block Type Codes
// */
//enum TrackMode {
//    MODE_AUDIO,		/* 2352 byte block length */
//    MODE_MODE1,		/* 2048 byte block length */
//    MODE_MODE1_RAW,		/* 2352 byte block length */
//    MODE_MODE2,		/* 2336 byte block length */
//    MODE_MODE2_FORM1,	/* 2048 byte block length */
//    MODE_MODE2_FORM2,	/* 2324 byte block length */
//    MODE_MODE2_FORM_MIX,	/* 2332 byte block length */
//    MODE_MODE2_RAW		/* 2352 byte block length */
//};

///* sub-channel mode
// * 5.29.2.13 Data Form of Sub-channel
// * NOTE: not sure if this applies to cue files
// */
//enum TrackSubMode {
//    SUB_MODE_RW,		/* RAW Data */
//    SUB_MODE_RW_RAW		/* PACK DATA (written R-W */
//};

///* track flags
// * Q Sub-channel Control Field (4.2.3.3, 5.29.2.2)
// */
//enum TrackFlag {
//    FLAG_NONE		= 0x00,	/* no flags set */
//    FLAG_PRE_EMPHASIS	= 0x01,	/* audio recorded with pre-emphasis */
//    FLAG_COPY_PERMITTED	= 0x02,	/* digital copy permitted */
//    FLAG_DATA		= 0x04,	/* data track */
//    FLAG_FOUR_CHANNEL	= 0x08,	/* 4 audio channels */
//    FLAG_SCMS		= 0x10,	/* SCMS (not Q Sub-ch.) (5.29.2.7) */
//    FLAG_ANY		= 0xff	/* any flags set */
//};

//enum DataType {
//    DATA_AUDIO,
//    DATA_DATA,
//    DATA_FIFO,
//    DATA_ZERO
//};

void parseCueTimeStr(QString str, int & min, int & sec, int & milli) {
    QList<QString> attrs = str.split(':');
    min = attrs.value(0).toInt();
    sec = attrs.value(1).toInt();
    milli = (attrs.value(2).toFloat() / 75.0) * 1000;
}

struct CueTrackIndex { //      INDEX [number] [mm:ss:ff] // MAXINDEX // mm:ss:ff – Starting time in minutes, seconds, and frames (75 frames/second). All times are relative to the beginning of the current file.
    CueTrackIndex(int numb, QString pos) : number(numb) {
        parseCueTimeStr(pos, minutes, seconds, millis);
    }

    int number;
    int minutes;
    int seconds;
    int millis;

    QString title;
    QString writer;
};

struct CueTrack {
    CueTrack(int numb, QString dType) : number(numb) {
        if (dType == "AUDIO") data_type = AUDIO;
        if (dType == "CDG") data_type = CDG;

        if (dType == "MODE1/2048") data_type = MODE1_2048;
        if (dType == "MODE1/2352") data_type = MODE1_2352;
        if (dType == "MODE2/2336") data_type = MODE2_2336;
        if (dType == "MODE2/2352") data_type = MODE2_2352;
        if (dType == "CDI/2336") data_type = CDI_2336;
        if (dType == "CDI/2352") data_type = CDI_2352;
    }

    //      PREGAP [mm:ss:ff] // mm:ss:ff – Specifies the pregap length in minutes, seconds, and frames. // must appear after a TRACK command, but before any INDEX commands. Only one PREGAP command is allowed per track
    //      POSTGAP [mm:ss:ff] // mm:ss:ff – Specifies the postgap length in minutes, seconds, and frames. // must appear after all INDEX commands for the current track. Only one POSTGAP command is allowed per track.

    int number;
    TrackDataType data_type;

    QString title;
    QString writer;
    AudioFlag flags; //      FLAGS [flags] // one or more // AudioFlag
    QString isrc;
    QString performer;
};

struct CueFile {
    CueFile(QString fpath, QString fType) : path(fpath), format(fType) {}

    QList<CueTrack> tracks;
    QString path;
    QString format;
};

#endif // CUE_STRUCTS
