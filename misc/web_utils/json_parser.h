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
            enum CellType {
                pre_value = -3, value = -2, key = -1,
                object = 1, array = 2, string = 3, number = 4, number_str = 5, literal = 6, literal_str, vmap
            };

            inline Cell(Cell * parent = 0) : _parent(parent) {}
            inline virtual ~Cell() {}

            virtual void addVal(QString & key, CellType val_type, QString & val) = 0;
            virtual void addVal(QString & key, CellType val_type, Cell * cell) = 0;
            inline Cell * parent() { return _parent; }
        protected:
            CellType cell_type;
        private:
            Cell * _parent;
    };

    class ObjectCell : public Cell {
        public:
            inline ObjectCell(Cell * parent = 0) : Cell(parent) { cell_type = object; }
            virtual ~ObjectCell();

            inline void addVal(QString & key, CellType val_type, QString & val) {
                cells.insert(key, QPair<CellType, void *>(val_type, new QString(val)));
                key.clear(); val.clear();
            }

            inline void addVal(QString & key, CellType val_type, Cell * cell) {
                cells.insert(key, QPair<CellType, void *>(val_type, cell));
                key.clear();
            }

        private:
            QHash<QString, QPair<CellType, void *> > cells;
    };

    class ArrayCell : public Cell {
        public:
            inline ArrayCell(Cell * parent = 0) : Cell(parent) { cell_type = array; }
            virtual ~ArrayCell();

            inline void addVal(QString & /*key*/, CellType val_type, QString & val) {
                cells.append(QPair<CellType, void *>(val_type, new QString(val))); val.clear();
            }

            inline void addVal(QString & /*key*/, CellType val_type, Cell * cell) {
                cells.append(QPair<CellType, void *>(val_type, cell));
            }

        private:
            QList<QPair<CellType, void *> > cells;
    };

    class Document : public UnicodeDecoding {
        enum Token {
            comma_token = 44,
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

//        inline void output() { qDebug() << (*root); }
    private:
        void parse(QIODevice * device);

        Cell * root;
    };
}

#endif // JSON_PARSER
