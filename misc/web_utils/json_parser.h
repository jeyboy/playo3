#ifndef JSON_PARSER
#define JSON_PARSER

#include <qstringbuilder.h>
#include <qhash.h>
#include <qbuffer.h>
#include <qpair.h>

#include "unicode_decoding.h"

#define DEBUG_LIMIT_OUTPUT 100

namespace Json {
    class Cell {
        public:
            enum CellType {object = 1, array, string, number, number_str, literal, literal_str, vmap};

            Cell(Cell * parent = 0) : parent(parent) {}
        private:
            CellType cell_type;
            Cell * parent;
    };

    class ObjectCell : public Cell {
        public:
            inline ObjectCell() : Cell(), cell_type(object) {}
            inline ~ObjectCell() {
                QList<QPair<CellType, void *> > attrs = cells.values();
                for(QList<QPair<CellType, void *> >::Iterator cell = attrs.begin(); cell != attrs.end(); cell++)
                    switch((*cell).first) {
                        case object:
                        case array: delete (*cell).second;
                    }
            }

        private:
            QHash<QString, QPair<CellType, void *> > cells;
    };

    class Document : public UnicodeDecoding {
        enum Token {
            space = 32,
            breaker = 58,
            open_object = 123,
            close_object = 125,
            open_array = 91,
            close_array = 93,
            string_token = 34,
            num_start = 47,
            num_end = 58
        };

        enum State {none, val, str, num, lit};

    public:
        inline Document(QIODevice * device) { parse(device); }
        inline Document(QString & str) {
            QByteArray ar = str.toUtf8();
            QBuffer stream(&ar);
            stream.open(QIODevice::ReadOnly);
            parse((QIODevice *)&stream);
            stream.close();
        }

        inline ~Document() { delete root; }

        inline void output() { qDebug() << (*root); }
    private:
        void parse(QIODevice * device);

        Cell * root;
    };
}

#endif // JSON_PARSER
