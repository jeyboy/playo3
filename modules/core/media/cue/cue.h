#ifndef CUE
#define CUE

#include <qfile.h>
#include <qtextstream.h>
#include <qregularexpression.h>

#include "cue_structs.h"

#define CHOP_STR(str) \
    if (str.startsWith('"')) \
        str = str.mid(1, str.length() - 2);

#define INIT_DURATION(res) \
    int length = res.length(); \
    if (length > 1) { \
        CueSong & updated = res[length - 2]; \
        CueSong & last = res[length - 1]; \
        updated.initDuration(last); \
    }


namespace Core {
    namespace Media {
        struct CueSong {
            CueSong(qint64 startPos = 0, const QString & trackName = QString(), const QString & filePath = QString(), const QString & extension = QString(), bool isPartial = true, int group = 0, const QString & error = QString())
                : startPos(startPos), trackName(trackName), filePath(filePath), extension(extension), isPartial(isPartial), group(group), error(error), duration(0) { }

            void initDuration(const CueSong & last) {
                if (last.group == group) \
                    duration = last.startPos - startPos; \
            }

            qint64 startPos;
            QString trackName;
            QString filePath;
            QString extension;
            bool isPartial;
            int group;
            QString error;
            qint64 duration;
        };

        class Cue {
                int level;

                QList<CueFile *> _files;
                CueFile * activeFile;

                QHash<QString, QString> _infos;
                QString title;
                QString performer;
                QString songwriter;
                QString catalog; // 13 digits
                QString text_file;
                QString path;
            public:
                Cue(const QString & path, QIODevice & obj);
                static Cue * fromPath(const QString & path);
                ~Cue() { qDeleteAll(_files); }

                inline QList<CueFile *> files() const { return _files; }
                inline QHash<QString, QString> infos() const { return _infos; }
                QList<CueSong> songs();
            protected:
                void identifyFile(QString & file_path, QString & file_extension, bool isShareable);
                void splitLine(QString & line, QList<QString> & res);
                void proceedLine(QString & line);
                inline void addFile(const QString & fpath, const QString & fType) { _files << (activeFile = new CueFile(fpath, fType) ); }
        };
    }
}

#endif // CUE
