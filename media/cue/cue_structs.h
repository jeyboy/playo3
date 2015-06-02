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
    FLAG_NONE = 0,
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

static void parseCueTimeStr(QString str, int & min, int & sec, int & milli) {
    QList<QString> attrs = str.split(':');
    min = attrs.value(0).toInt();
    sec = attrs.value(1).toInt();
    milli = (attrs.value(2).toFloat() / 75.0) * 1000;
}

struct CueTrackIndex { //      All times are relative to the beginning of the current file.
    CueTrackIndex(int numb, QString pos) : number(numb) {
        parseCueTimeStr(pos, minutes, seconds, millis);
    }

    qint64 toMillis() {
        return millis + seconds * 1000 + minutes * 60000;
    }

    QString toStr() {
        return title;
    }

    int number;
    int minutes, seconds, millis;

    QString title;
    QString writer;
};

struct IndexContainer {
    void addIndex(QString numb, QString pos) {
        indexes << (activeIndex = new CueTrackIndex(numb.toInt(), pos));
    }

    QList<CueTrackIndex *> indexes;
    CueTrackIndex * activeIndex;
};

struct CueTrack : IndexContainer {
    CueTrack(int numb, QString dType) : number(numb),
        pregap_minutes(0), pregap_seconds(0), pregap_millis(0),
        postgap_minutes(0), postgap_seconds(0), postgap_millis(0),
        flags(FLAG_NONE)
    {
        if (dType == "AUDIO") data_type = AUDIO;
        if (dType == "CDG") data_type = CDG;

        if (dType == "MODE1/2048") data_type = MODE1_2048;
        if (dType == "MODE1/2352") data_type = MODE1_2352;
        if (dType == "MODE2/2336") data_type = MODE2_2336;
        if (dType == "MODE2/2352") data_type = MODE2_2352;
        if (dType == "CDI/2336") data_type = CDI_2336;
        if (dType == "CDI/2352") data_type = CDI_2352;
    }

    ~CueTrack() {
        qDeleteAll(indexes);
    }

    void parseFlags(QList<QString> flgs) {
        for(QList<QString>::Iterator flag = flgs.begin(); flag != flgs.end(); flag++) {
            if ((*flag) == "DCP") flags = (AudioFlag)(flags|FLAG_DCP);
            if ((*flag) == "4CH") flags = (AudioFlag)(flags|FLAG_4CH);
            if ((*flag) == "PRE") flags = (AudioFlag)(flags|FLAG_PRE);
            if ((*flag) == "SCMS") flags = (AudioFlag)(flags|FLAG_SCMS);
        }
    }

    void setPregap(QString pregap) { parseCueTimeStr(pregap, pregap_minutes, pregap_seconds, pregap_millis); }
    void setPostgap(QString postgap) { parseCueTimeStr(postgap, postgap_minutes, postgap_seconds, postgap_millis); }

    int number;
    TrackDataType data_type;

    int pregap_minutes, pregap_seconds, pregap_millis;
    int postgap_minutes, postgap_seconds, postgap_millis;

    QString title;
    QString songwriter;
    AudioFlag flags; //      FLAGS [flags] // one or more // AudioFlag
    QString isrc;
    QString performer;
};

struct CueFile : IndexContainer {
    CueFile(QString fpath, QString fType) : path(fpath) {
        if (fType == "BINARY") format = BINARY;
        if (fType == "MOTOROLA") format = MOTOROLA;
        if (fType == "AIFF") format = AIFF;
        if (fType == "WAVE") format = WAVE;
        if (fType == "MP3") format = MP3;
    }

    ~CueFile() {
        qDeleteAll(tracks);
    }

    void addTrack(QString numb, QString dType) {
        tracks << (activeTrack = new CueTrack(numb.toInt(), dType));
    }

    QString path;
    AudioFormat format;

    QList<CueTrack *> tracks;
    CueTrack * activeTrack;
};

#endif // CUE_STRUCTS
