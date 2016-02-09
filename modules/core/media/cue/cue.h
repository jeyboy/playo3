#ifndef CUE
#define CUE

#include <qfile.h>
#include <qtextstream.h>
#include <qregularexpression.h>

#include "cue_structs.h"

#define CHOP_STR(str) \
    if (str.startsWith('"')) \
        str = str.mid(1, str.length() - 2);

namespace Core {
    namespace Media {
        struct CueSong {
            CueSong(qint64 startPos = 0, const QString & trackName = QString(), const QString & filePath = QString(), const QString & extension = QString(), bool isPartial = true, int group = 0)
                : startPos(startPos), trackName(trackName), filePath(filePath), extension(extension), isPartial(isPartial), group(group) { }
            qint64 startPos;
            QString trackName;
            QString filePath;
            QString extension;
            bool isPartial;
            int group;
        };

        class Cue {
            public:
                Cue(const QString & path, QIODevice & obj);
                static Cue * fromPath(const QString & path) {
                    QFile f(path);
                    if (f.open(QFile::ReadOnly | QFile::Text)) {
                        Cue * cue = new Cue(path, f);
                        f.close();
                        return cue;
                    }

                    return 0;
                }

                inline QList<CueFile *> files() const { return _files; }
                inline QHash<QString, QString> infos() const { return _infos; }
                QList<CueSong> songs();
            protected:
                void splitLine(QString & line, QList<QString> & res);
                void proceedLine(QString & line);
                inline void addFile(const QString & fpath, const QString & fType) { _files << (activeFile = new CueFile(fpath, fType) ); }
            private:
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
        };
    }
}

#endif // CUE
