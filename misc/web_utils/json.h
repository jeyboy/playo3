#ifndef JSON_PARSER
#define JSON_PARSER

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qvariant.h>

//NOT USING // NOT FINISHED

#include <qstringbuilder.h>
#include <qhash.h>
#include <qbuffer.h>
#include <qpair.h>

#include "unicode_decoding.h"

#define DEBUG_LIMIT_OUTPUT 100

//QString js = QString(
//    "{"
//    "   \"firstName\": \"Иван\","
//    "   \"lastName\": \"Иванов\","
//    "   \"address\": {"
//    "       \"streetAddress\": \"Московское ш., 101, кв.101\","
//    "       \"city\": \"Ленинград\","
//    "       \"postalCode\": 101101"
//    "   },"
//    "   \"pido\": true,"
//    "   \"phoneNumbers\": ["
//    "       \"812 123-1234\","
//    "       \"916 123-4567\""
//    "   ],"
//    "   \"dialo\": ["
//    "       true,"
//    "       234567.34"
//    "   ]"
//    "}"
//);

namespace Json {
//    class ArrayCell;
//    class ObjectCell;

//    class Cell {
//        public:
//            enum CellType {
//                pre_value = -3, value = -2, key = -1,
//                object = 1, array = 2, string = 3, number = 4, number_str = 5, literal = 6, literal_str, vmap
//            };

//            inline Cell(Cell * parent = 0) : _parent(parent) {}
//            inline virtual ~Cell() {}
//            virtual inline CellType cell_type() const { return object; }

//            virtual void addVal(QString & key, CellType val_type, QString & val) = 0;
//            virtual void addVal(QString & key, CellType val_type, Cell * cell) = 0;
//            inline Cell * parent(CellType & cType) {
//                cType = cell_type() == Cell::object ? Cell::object : Cell::value;
//                return _parent;
//            }
////            virtual inline Cell * operator[](int index) = 0;
////            virtual Cell * operator[](QString name) = 0;

////            inline float toNum(QPair<CellType, void *> & val) {
////                switch(val.first) {
////                    case number: return val.second;
////                    case number_str: {
//////                        QString * str = val.second;
//////                        val = QPair<CellType, void *>
//////                        val.second = &str -> toFloat();

////                    break;}
////                }
////            }
////            inline bool toBool(QPair<CellType, void *> & val) {

////            }
////            inline bool toStr(QPair<CellType, void *> & val) {

////            }
////            inline bool toCell(QPair<CellType, void *> & val) {

////            }
//        private:
//            Cell * _parent;
//    };


//    class ObjectCell : public Cell {
//        public:
//            inline ObjectCell() : Cell(0) {}
//            inline ObjectCell(QString & key, Cell * parent) : Cell(parent) {
//                parent -> addVal(key, object, this); key.clear();
//            }
//            virtual ~ObjectCell();

//            inline void addVal(QString & key, CellType val_type, QString & val) {
//                cells.insert(key, QPair<CellType, void *>(val_type, new QString(val)));
//                key.clear(); val.clear();
//            }

//            inline void addVal(QString & key, CellType val_type, Cell * cell) {
//                cells.insert(key, QPair<CellType, void *>(val_type, cell));
//                key.clear();
//            }

//            inline int count() { return cells.size(); }

////            inline Cell * operator[](int index) {
////            }
////            inline Cell * operator[](QString name) = 0;
//        private:
//            QHash<QString, QPair<CellType, void *> > cells;
//    };

//    class ArrayCell : public Cell {
//        public:
//            inline ArrayCell(QString & key, Cell * parent) : Cell(parent) {
//                parent -> addVal(key, Cell::array, this);
//            }
//            virtual ~ArrayCell();
//            virtual inline CellType cell_type() const { return array; }

//            inline void addVal(QString & /*key*/, CellType val_type, QString & val) {
//                cells.append(QPair<CellType, void *>(val_type, new QString(val))); val.clear();
//            }

//            inline void addVal(QString & /*key*/, CellType val_type, Cell * cell) {
//                cells.append(QPair<CellType, void *>(val_type, cell));
//            }

//        private:
//            QList<QPair<CellType, void *> > cells;
//    };

//    class Document : public UnicodeDecoding {
//        enum Token {
//            comma_token = 44,
//            space = 32,
//            breaker = 58,
//            open_object = 123,
//            close_object = 125,
//            open_array = 91,
//            close_array = 93,
//            string_token = 34,
//            num_start = 47,
//            num_end = 58
//        };

//    public:
//        inline Document(QIODevice * device) { parse(device); }
//        inline Document(QByteArray & data) { parse(data); }

//        inline ~Document() { delete root; }

////        inline void output() { qDebug() << (*root); }
//    private:
//        void parse(QIODevice * device);
//        void parse(QByteArray & data);

//        Cell * root;
//    };

    class Obj;
    class Arr;
    class Val : public QJsonValue {
        public:
            static inline Val fromQVal(QJsonValue obj) { return *(Val *)(&obj); }
            inline Val(const Obj & o) : QJsonValue((QJsonObject &)o) {}

            Arr arr();
            Obj obj();
    };

    class Obj : public QJsonObject {
        public:
            static inline Obj fromData(const QByteArray & data) {
                QJsonObject obj = QJsonDocument::fromJson(data).object();
                return *(Json::Obj *)(&obj);
            }
            static inline Obj fromQJsonObj(QJsonObject obj) { return *(Obj *)(&obj); }

            Obj takeObj(const QString & key);
            Arr takeArr(const QString & key);
            inline int takeInt(const QString & key)         { return take(key).toInt(); }

            Obj obj(const QString & key);
            Arr arr(const QString & key);
            inline QVariant var(const QString & key)        { return value(key).toVariant(); }
            Val val(const QString & key);

            inline bool boolVal(const QString & key) const  { return value(key).toBool(); }
            inline QString str(const QString & key) const   { return value(key).toString(); }
            inline QString numStr(const QString & key)      { return QString::number((qint64)value(key).toDouble()); }
            inline int intVal(const QString & key) const    { return value(key).toInt(); }
            inline double doubleVal(const QString & key)    { return value(key).toDouble(); }
    };

    class Arr : public QJsonArray {
        public:
            inline Val operator[](int j) const { return Val::fromQVal(at(j)); }
    };
}

#endif // JSON_PARSER
