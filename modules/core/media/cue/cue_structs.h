#ifndef CUE_STRUCTS
#define CUE_STRUCTS

#include <qlist.h>
#include <qstringbuilder.h>

#include "modules/core/misc/file_utils/extensions.h"
#include "modules/core/misc/file_utils/filename_conversions.h"

#define MAXTRACK	99	/* Red Book track limit */
#define MAXINDEX	99	/* Red Book index limit */

namespace Core {
    namespace Media {
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

        static void parseCueTimeStr(const QString & str, qint64 & min, qint64 & sec, qint64 & milli) {
            QList<QString> attrs = str.split(':');
            min = attrs.value(0).toLongLong();
            sec = attrs.value(1).toLongLong();
            milli = (attrs.value(2).toFloat() / 75.0) * 1000;
        }

        struct CueTrackIndex { //      All times are relative to the beginning of the current file.
            inline CueTrackIndex(int numb, QString pos) : number(numb) { parseCueTimeStr(pos, minutes, seconds, millis); }

            inline qint64 toMillis() { return millis + seconds * 1000 + minutes * 60000; }

            int number;
            qint64 minutes, seconds, millis;
        };

        struct IndexContainer {
            inline void addIndex(QString numb, QString pos) {
                indexes << (activeIndex = new CueTrackIndex(numb.toInt(), pos));
            }

            QList<CueTrackIndex *> indexes;
            CueTrackIndex * activeIndex;
        };

        struct CueTrack : IndexContainer {
            CueTrack(int numb, const QString & dType) : number(numb),
                pregap_minutes(0), pregap_seconds(0), pregap_millis(0),
                postgap_minutes(0), postgap_seconds(0), postgap_millis(0),
                flags(FLAG_NONE)
            {
                if (dType == QStringLiteral("AUDIO")) data_type = AUDIO;
                if (dType == QStringLiteral("CDG")) data_type = CDG;

                if (dType == QStringLiteral("MODE1/2048")) data_type = MODE1_2048;
                if (dType == QStringLiteral("MODE1/2352")) data_type = MODE1_2352;
                if (dType == QStringLiteral("MODE2/2336")) data_type = MODE2_2336;
                if (dType == QStringLiteral("MODE2/2352")) data_type = MODE2_2352;
                if (dType == QStringLiteral("CDI/2336")) data_type = CDI_2336;
                if (dType == QStringLiteral("CDI/2352")) data_type = CDI_2352;
            }

            ~CueTrack() {
                qDeleteAll(indexes);
            }

            void parseFlags(const QList<QString> & flgs) {
                for(QList<QString>::ConstIterator flag = flgs.cbegin(); flag != flgs.cend(); flag++) {
                    if ((*flag) == QStringLiteral("DCP")) flags = (AudioFlag)(flags|FLAG_DCP);
                    if ((*flag) == QStringLiteral("4CH")) flags = (AudioFlag)(flags|FLAG_4CH);
                    if ((*flag) == QStringLiteral("PRE")) flags = (AudioFlag)(flags|FLAG_PRE);
                    if ((*flag) == QStringLiteral("SCMS")) flags = (AudioFlag)(flags|FLAG_SCMS);
                }
            }

            void setPregap(const QString & pregap) { parseCueTimeStr(pregap, pregap_minutes, pregap_seconds, pregap_millis); }
            void setPostgap(const QString & postgap) { parseCueTimeStr(postgap, postgap_minutes, postgap_seconds, postgap_millis); }
            void addInfo(const QString & value) { info = value; }

            QString toStr() {
                if (!performer.isEmpty())
                    return performer % QStringLiteral(" - ") % title;
                else return title;
            }

            int number;
            TrackDataType data_type;

            qint64 pregap_minutes, pregap_seconds, pregap_millis;
            qint64 postgap_minutes, postgap_seconds, postgap_millis;

            QString title;
            QString songwriter;
            AudioFlag flags; //      FLAGS [flags] // one or more // AudioFlag
            QString isrc;
            QString performer;
            QString info;
        };

        struct CueFile : IndexContainer {
            CueFile(const QString & fpath, const QString & fType) : path(fpath) {
                if (fType == QStringLiteral("BINARY")) format = BINARY;
                if (fType == QStringLiteral("MOTOROLA")) format = MOTOROLA;
                if (fType == QStringLiteral("AIFF")) format = AIFF;
                if (fType == QStringLiteral("WAVE")) {
                    extension = QStringLiteral("wav");
                    format = WAVE;
                }
                if (fType == QStringLiteral("MP3")) {
                    extension = QStringLiteral("mp3");
                    format = MP3;
                }

                if (extension.isEmpty()) {
                    QString tpath = QString(fpath);
                    Extensions::obj().extractExtension(tpath, extension, false);
                }
            }

            virtual ~CueFile() { qDeleteAll(tracks); }

            inline void addTrack(const QString & numb, const QString & dType) {
                tracks << (activeTrack = new CueTrack(numb.toInt(), dType));
            }

            QString path;
            AudioFormat format;
            QString extension;

            QList<CueTrack *> tracks;
            CueTrack * activeTrack;
        };
    }
}

#endif // CUE_STRUCTS
