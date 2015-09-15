#include "json.h"

namespace Json {
//    ObjectCell::~ObjectCell() {
//        QList<QPair<CellType, void *> > attrs = cells.values();
//        for(QList<QPair<CellType, void *> >::Iterator cell = attrs.begin(); cell != attrs.end(); cell++)
//            switch((*cell).first) {
//                case object: delete (ObjectCell *)(*cell).second; break;
//                case array: delete (ArrayCell *)(*cell).second; break;
//                case number: delete (float *)(*cell).second; break;
//                default: delete (QString *)(*cell).second;
//            }
//    }

//    ArrayCell::~ArrayCell() {
//        for(QList<QPair<CellType, void *> >::Iterator cell = cells.begin(); cell != cells.end(); cell++)
//            switch((*cell).first) {
//                case object: delete (ObjectCell *)(*cell).second; break;
//                case array: delete (ArrayCell *)(*cell).second; break;
//                case number: delete (float *)(*cell).second; break;
//                default: delete (QString *)(*cell).second;
//            }
//    }

//    void Document::parse(QIODevice * device) {
//        Cell::CellType state = Cell::object, comma_type = Cell::object;
//        char * ch = new char[2]();
//        QString key, value;
//        Cell * elem = root = 0;

//        while(true) {
//            if (device -> getChar(ch)) {
//                if (*ch == 0) break;
//                if (*ch > 0 && *ch < 31) continue; // skip not printable trash

//                switch (state) {
//                    case Cell::pre_value: {
//                        switch(*ch) {
//                            case breaker: state = Cell::value; break;
//                            default: continue;
//                        }
//                    break;}

//                    case Cell::key: {
//                        switch(*ch) {
//                            case string_token: state = Cell::pre_value; break;
//                            default:
//                                if (*ch > 0) key.append(*ch);
//                                else scanUtf8Char(device, key, ch[0]);
//                        }
//                    break;}

//                    case Cell::value: {
//                        switch(*ch) {
//                            case space: break; // skip -
//                            case open_object: {
//                                elem = new ObjectCell(key, elem);
//                                comma_type = Cell::key; state = Cell::object;
//                            break;}
//                            case open_array: {
//                                elem = new ArrayCell(key, elem);
//                                comma_type = Cell::value;
//                            break;}
//                            case string_token: state = Cell::string; break;
//                            default:
//                                if (*ch > num_start && *ch < num_start) state = Cell::number_str;
//                                else state = Cell::literal_str;
//                                value.append(*ch);
//                        }
//                    break;}

//                    case Cell::string: {
//                        switch(*ch) {
//                            case string_token: {
//                                elem -> addVal(key, state, value);
//                                state = Cell::object;
//                            break;}
//                            default:
//                                if (*ch > 0) value.append(*ch);
//                                else scanUtf8Char(device, value, ch[0]);
//                        }
//                    break;}

//                    case Cell::number_str:
//                    case Cell::literal_str: {
//                        switch(*ch) {
//                            case close_object:
//                            case close_array: {
//                                elem -> addVal(key, state, value); break;
//                                elem = elem -> parent(comma_type);
//                                state = Cell::object;
//                            break;}
//                            case comma_token: {
//                                elem -> addVal(key, state, value);
//                                state = Cell::value;
//                            break;}
//                            default: value.append(*ch);
//                        }
//                    break;}

//                    default: switch(*ch) {
//                        case comma_token: state = comma_type; break;
//                        case string_token: state = Cell::key; break;
//                        case close_object:
//                        case close_array: elem = elem -> parent(comma_type); break;
//                        case open_object: elem = root = new ObjectCell(); break;
//                        default: break;
//                    }
//                }
//            }
//        }
//        delete ch;
//    }

//    void Document::parse(QByteArray & data) {
//        Cell::CellType state = Cell::object, comma_type = Cell::object;
//        char * ch = data.data();
//        QString key, value;
//        Cell * elem = root = 0;

//        while(*ch) {
//            if (*ch > 0 && *ch < 31) { ch++; continue; } // skip not printable trash

//            switch (state) {
//                case Cell::pre_value: {
//                    switch(*ch) {
//                        case breaker: state = Cell::value; break;
//                        default: ch++; continue;
//                    }
//                break;}

//                case Cell::key: {
//                    switch(*ch) {
//                        case string_token: state = Cell::pre_value; break;
//                        default: key.append(*ch);
////                            if (*ch > 0) key.append(*ch);
////                            else scanUtf8Char(ch, key);
//                    }
//                break;}

//                case Cell::value: {
//                    switch(*ch) {
//                        case space: break; // skip -
//                        case open_object: {
//                            elem = new ObjectCell(key, elem);
//                            comma_type = Cell::key; state = Cell::object;
//                        break;}
//                        case open_array: {
//                            elem = new ArrayCell(key, elem);
//                            comma_type = Cell::value;
//                        break;}
//                        case string_token: state = Cell::string; break;
//                        default:
//                            if (*ch > num_start && *ch < num_start) state = Cell::number_str;
//                            else state = Cell::literal_str;
//                            value.append(*ch);
//                    }
//                break;}

//                case Cell::string: {
//                    switch(*ch) {
//                        case string_token: {
//                            elem -> addVal(key, state, value);
//                            state = Cell::object;
//                        break;}
//                        default: value.append(*ch);
////                            if (*ch > 0) value.append(*ch);
////                            else scanUtf8Char(ch, value);
//                    }
//                break;}

//                case Cell::number_str:
//                case Cell::literal_str: {
//                    switch(*ch) {
//                        case close_object:
//                        case close_array: {
//                            elem -> addVal(key, state, value); break;
//                            elem = elem -> parent(comma_type);
//                            state = Cell::object;
//                        break;}
//                        case comma_token: {
//                            elem -> addVal(key, state, value);
//                            state = Cell::value;
//                        break;}
//                        default: value.append(*ch);
//                    }
//                break;}

//                default: switch(*ch) {
//                    case comma_token: state = comma_type; break;
//                    case string_token: state = Cell::key; break;
//                    case close_object:
//                    case close_array: elem = elem -> parent(comma_type); break;
//                    case open_object: elem = root = new ObjectCell(); break;
//                    default: break;
//                }
//            }
//            ch++;
//        }
//    }

    Arr Val::arr() {
        return *(Arr *)(this);
    }

    Obj Obj::takeObj(const QString & key) { return take(key).toObject(); }
    Arr Obj::takeArr(const QString & key) { return take(key).toArray(); }

    Obj Obj::toObj(const QString & key) {
        QJsonObject obj = value(key).toObject();
        return *(Obj *)(&obj);
    }
    Arr Obj::toArr(const QString & key) {
        QJsonArray obj = value(key).toArray();
        return *(Arr *)(&obj);
    }

    Val Obj::val(const QString & key) {
        QJsonValue obj = value(key);
        return *(Val *)(&obj);
    }
}
