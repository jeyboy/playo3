#include "json_parser.h"

namespace Json {
    void Document::parse(QIODevice * device) {
        State state = none;
        char * ch = new char[2]();
        QString key, value; value.reserve(1024); key.reserve(256);
        ObjectCell * elem = root = new ObjectCell();

        while(!device -> atEnd()) {
            if (device -> getChar(ch)) {
                if (*ch > 0 && *ch < 31) continue; // skip not printable trash

                switch (state) {
                    case val: {
                        switch(*ch) {
                            case space: break; // skip -
                            case open_object: {
                                elem = new ObjectCell(elem);
                            break;}
                            case open_array: {

                            break;}
                            case string_token: state = str; break;
                            default:
                                if (*ch > num_start && *ch < num_start)
                                    state = num;
                                else
                                    state = lit;
                                curr.append(*ch);
                        }

                    break;}

                    case str: {
                        switch(*ch) {
                            case string_token: {
                                elem -> addVal(key, );
                                state = none;
                            break;}
                            default:
                                if (*ch > 0)
                                    curr.append(*ch);
                                else
                                    scanUtf8Char(device, curr, ch[0]);
                        }
                    break;}

                    case num: {
                        switch(*ch) {
                            case content_del1:
                            case content_del2: {
                                switch(state) {
                                    case val: { state = in_val; break;}
                                    case in_val: {
                                        elem -> addAttr(curr, value);
                                        state = attr;
                                    break;}
                                    default: { qDebug() << "WRONG STATE" << state; return; }
                                }
                            break;}
                            default:
                                if (*ch > 0)
                                    value.append(*ch);
                                else
                                    scanUtf8Char(device, value, ch[0]);
                        }
                    break;}

                    case lit: {
                        switch(*ch) {
                            case content_del1:
                            case content_del2: {
                                switch(state) {
                                    case val: { state = in_val; break;}
                                    case in_val: {
                                        elem -> addAttr(curr, value);
                                        state = attr;
                                    break;}
                                    default: { qDebug() << "WRONG STATE" << state; return; }
                                }
                            break;}
                            default:
                                if (*ch > 0)
                                    value.append(*ch);
                                else
                                    scanUtf8Char(device, value, ch[0]);
                        }
                    break;}

                    default: switch(*ch) {
                        case space: {
                            switch(state) {
                                case attr:
                                case val: { if (!curr.isEmpty()) elem -> addAttr(curr, value); state = attr; break; } // proceed attrs without value
                                case tag: { elem = elem -> appendTag(curr); state = attr; break;}
                                default: /*continue*/; // else skip spaces
                            }
                        break;}

                        default: { curr.append(*ch); }
                    }
                }
            }
        }
        delete ch;
    }
}
