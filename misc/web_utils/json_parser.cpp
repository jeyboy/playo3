#include "json_parser.h"

namespace Json {
    void Document::parse(QIODevice * device) {
        Cell::CellType state = Cell::object;
        char * ch = new char[2]();
        QString key, value; value.reserve(1024); key.reserve(256);
        Cell * elem = root = new ObjectCell();

        while(!device -> atEnd()) {
            if (device -> getChar(ch)) {
                if (*ch > 0 && *ch < 31) continue; // skip not printable trash

                switch (state) {
                    case Cell::key: {
                        switch(*ch) {
                            case string_token: state = Cell::value; break;
                            default:
                                if (*ch > 0) key.append(*ch);
                                else scanUtf8Char(device, key, ch[0]);
                        }
                    break;}

                    case Cell::value: {
                        switch(*ch) {
                            case breaker:
                            case space:
                                break; // skip -
                            case open_object: {
                                Cell * prev = elem;
                                elem = new ObjectCell(elem);
                                prev -> addVal(key, (state = Cell::object), elem);
                            break;}
                            case open_array: {
                                Cell * prev = elem;
                                elem = new ArrayCell(elem);
                                prev -> addVal(key, (state = Cell::array), elem);
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
                                state = Cell::key;
                            break;}
                            default: value.append(*ch);
                        }
                    break;}

                    default: switch(*ch) {
                        case breaker: state = Cell::value; break;
                        case comma_token:
                        case string_token:
                            state = Cell::key; break;
                        case close_object:
                        case close_array: elem = elem -> parent(); break;
                        case open_object: {
                            Cell * prev = elem;
                            elem = new ObjectCell(elem);
                            prev -> addVal(key, (state = Cell::object), elem);
                        break;}
                        default: break;
                    }
                }
            }
        }
        delete ch;
    }
}
