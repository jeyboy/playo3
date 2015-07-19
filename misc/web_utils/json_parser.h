#ifndef JSON_PARSER
#define JSON_PARSER

#include <qstringbuilder.h>
#include <qhash.h>
#include <qbuffer.h>
//#include <qpair.h>

#include "unicode_decoding.h"

#define DEBUG_LIMIT_OUTPUT 100

namespace Json {
    class Cell {
        public:
            enum CellType {object = 1, array, string, number, literal};

        private:
            CellType cell_type;
            Cell * parent;
    };

    class ObjectCell : public Cell {
        public:
            inline ObjectCell() : Cell(), cell_type(object) {}
            inline ~ObjectCell() { qDeleteAll(cells.values()); }

        private:
            QHash<QString, Cell *> cells;
    };

    class Document : public UnicodeDecoding {
        enum PToken {
            open_tag = 60,
            close_tag_predicate = 47,
            close_tag = 62,
            space = 32,
            comment_token = 33,
            comment_post_token = 45,
            attr_rel = 61,
            content_del1 = 34,
            content_del2 = 39,
            mean_sym = 92
        };

    public:
        inline Document(QIODevice * device, Flags parse_flags = skip_comment) : flags(parse_flags) { parse(device); }
        inline Document(QString & str, Flags parse_flags = skip_comment) : flags(parse_flags) {
            QByteArray ar = str.toUtf8();
            QBuffer stream(&ar);
            stream.open(QIODevice::ReadOnly);
            parse((QIODevice *)&stream);
            stream.close();
        }

        inline ~Document() { delete root; }

        inline void output() { qDebug() << (*root); }
    private:
        inline bool isSolo(HtmlTag * tag) { return solo.contains(tag -> name()); }

        void initSoloTags();

        void parse(QIODevice * device);

        QHash<QString, bool> solo;
        HtmlTag * root;
        Flags flags;
    };
}

#endif // JSON_PARSER
