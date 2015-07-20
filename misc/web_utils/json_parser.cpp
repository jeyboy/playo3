#include "json_parser.h"

namespace Json {
    ObjectCell::~ObjectCell() {
        QList<QPair<CellType, void *> > attrs = cells.values();
        for(QList<QPair<CellType, void *> >::Iterator cell = attrs.begin(); cell != attrs.end(); cell++)
            switch((*cell).first) {
                case object: delete (ObjectCell *)(*cell).second; break;
                case array: delete (ArrayCell *)(*cell).second; break;
                case number: delete (float *)(*cell).second; break;
                default: delete (QString *)(*cell).second;
            }
    }

    ArrayCell::~ArrayCell() {
        for(QList<QPair<CellType, void *> >::Iterator cell = cells.begin(); cell != cells.end(); cell++)
            switch((*cell).first) {
                case object: delete (ObjectCell *)(*cell).second; break;
                case array: delete (ArrayCell *)(*cell).second; break;
                case number: delete (float *)(*cell).second; break;
                default: delete (QString *)(*cell).second;
            }
    }

    void Document::parse(QIODevice * device) {
        Cell::CellType state = Cell::object;
        char * ch = new char[2]();
        QString key, value; value.reserve(1024); key.reserve(256);
        Cell * elem = root = 0;

        while(!device -> atEnd()) {
            if (device -> getChar(ch)) {
                if (*ch > 0 && *ch < 31) continue; // skip not printable trash

                switch (state) {
                    case Cell::pre_value: {
                        switch(*ch) {
                            case breaker: state = Cell::value; break;
                            default: continue;
                        }
                    break;}

                    case Cell::key: {
                        switch(*ch) {
                            case string_token: state = Cell::pre_value; break;
                            default:
                                if (*ch > 0) key.append(*ch);
                                else scanUtf8Char(device, key, ch[0]);
                        }
                    break;}

                    case Cell::value: {
                        switch(*ch) {
                            case space: break; // skip -
                            case open_object: {
                                Cell * prev = elem;
                                elem = new ObjectCell(elem);
                                prev -> addVal(key, (state = Cell::object), elem);
                            break;}
                            case open_array: {
                                Cell * prev = elem;
                                elem = new ArrayCell(elem);
                                prev -> addVal(key, Cell::array, elem);
                                state = Cell::value;
                            break;}
                            case string_token: state = Cell::string; break;
                            default:
                                if (*ch > num_start && *ch < num_start) state = Cell::number_str;
                                else state = Cell::literal_str;
                                value.append(*ch);
                        }
                    break;}

                    case Cell::string: {
                        switch(*ch) {
                            case string_token: {
                                elem -> addVal(key, state, value);
                                state = Cell::object;
                            break;}
                            default:
                                if (*ch > 0) value.append(*ch);
                                else scanUtf8Char(device, value, ch[0]);
                        }
                    break;}

                    case Cell::number_str:
                    case Cell::literal_str: {
                        switch(*ch) {
                            case close_object:
                            case close_array: {
                                elem -> addVal(key, state, value); break;
                                elem = elem -> parent();
                                state = Cell::object;
                            break;}
                            case comma_token: {
                                elem -> addVal(key, state, value);
                                state = Cell::value;
                            break;}
                            default: value.append(*ch);
                        }
                    break;}

                    default: switch(*ch) {
                        case comma_token:
                            state = Cell::value; break;
                        case string_token:
                            state = Cell::key; break;
                        case close_object:
                        case close_array: elem = elem -> parent(); break;
                        case open_object: {
                            elem = root = new ObjectCell(elem);
                        break;}
                        default: break;
                    }
                }
            }
        }
        delete ch;
    }
}
