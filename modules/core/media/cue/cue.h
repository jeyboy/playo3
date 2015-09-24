#ifndef CUE
#define CUE

#include <qfile.h>
#include <qtextstream.h>
#include <qregularexpression.h>

#include "cue_structs.h"

namespace Core {
    namespace Media {
        class Cue {
            public:
                Cue(QIODevice & obj);
        //        Cue(QString & obj);

                inline QList<CueFile *> files() const { return _files; }
                inline QHash<QString, QString> infos() const { return _infos; }
                QMap<qint64, QString> songs();
            protected:
                QList<QString> splitLine(QString & line);
                void proceedLine(QString & line);
                inline void addFile(QString fpath, QString fType) { _files << (activeFile = new CueFile(fpath, fType) ); }
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
        };
    }
}

#endif // CUE
