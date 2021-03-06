#ifndef CUE
#define CUE

#include <qfile.h>
#include <qtextstream.h>
#include <qregularexpression.h>

#include "modules/core/core_parts/playlist.h"
#include "cue_structs.h"

#define PREPARE_SEARCH_PREDICATE(tName, ext) \
    tName.replace(QRegularExpression("[\\W]+"), QLatin1String("*")) % QLatin1String(".") % ext

#define CHOP_STR(str) \
    if (str.startsWith('"')) \
        str = str.mid(1, str.length() - 2);

#define ADD_LINE_PART(res, line, pos, offset) \
    QStringRef str = line.midRef(pos, offset - pos); \
    if (!str.isEmpty()) { \
        CHOP_STR(str); \
        res.append(str.toString()); \
    }


namespace Core {
    namespace Media {
        class Cue {
                enum CueTokens {
                    unknow = -1,
                    rem = 1,
                    title,
                    songwriter,
                    catalog,
                    cdtextfile,
                    file,
                    performer,

                    track,
                    index,

                    isrc,
                    flags,
                    pregap,
                    postgap
                };

                static QHash<QString, CueTokens> tokens;

                int level;

                QList<CueFile *> _files;
                CueFile * activeFile;

                QHash<CueTokens, QString> _attrs;
                QHash<QString, QString> _infos;

                QString path;
                QString filename;
            public:
                Cue(const QString & path, QIODevice & obj);
                static Cue * fromPath(const QString & path);
                ~Cue() { qDeleteAll(_files); }

                inline QList<CueFile *> files() const { return _files; }
                inline QHash<QString, QString> infos() const { return _infos; }
                int buildItems(Playlist * cuePlaylist, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);
            protected:
                bool identifyFile(QString & file_path, QString & file_extension, bool isShareable);
                void splitLine(QString & line, QList<QString> & res);
                void proceedLine(QString & line);
                inline void addFile(const QString & fpath, const QString & fType) { _files << (activeFile = new CueFile(fpath, fType) ); }
        };
    }
}

#endif // CUE
