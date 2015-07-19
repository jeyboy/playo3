#include "html_parser.h"

////////  Set //////////
namespace Html {
    Set & Set::find(Selector * selector, Set & set) {
        for(Set::Iterator tag = begin(); tag != end(); tag++) {
            if ((*tag) -> validTo(selector)) {
                if (selector -> next && !(*tag) -> children().isEmpty())
                    (*tag)-> children().find(selector -> next, set);
                else
                    set.append((*tag));
            }
            else if (!selector -> _direct && !(*tag) -> children().isEmpty())
                (*tag)-> children().find(selector, set);
        }

        return set;
    }

    ////////  Selector //////////

    void Selector::addToken(SState & tType, QString & token, char & rel) {
        switch(tType) {
            case attr: {
                QStringList parts = token.split(rel, QString::SkipEmptyParts);
                QPair<char, QString> newAttr(rel, parts.length() > 1 ? parts.last() : HTML_PARSER_ANY_ELEMENT);
                _attrs.insert(parts.first(), newAttr); rel = attr_rel_eq;
                break;
            }
            case klass: {
                klasses.append(token.split(" ", QString::SkipEmptyParts));
                break;
            }
            default:;
        }

        _tokens.insert(tType, token);
        token.clear();
    }

    Selector::Selector(const char * predicate) : _direct(false), prev(0), next(0) {
        Selector::SState state = Selector::tag;
        Selector * selector = this;
        QString token; token.reserve(128);
        char rel;
        const char * it = predicate;

        while(*it) {
            switch(*it) {
                case id_token: {
                    selector -> addToken(state, token, rel);
                    state = Selector::id;
                break;}

                case class_token: {
                    selector -> addToken(state, token, rel);
                    state = Selector::klass;
                break;}

                case attr_token:
                case attr_separator: {
                    selector -> addToken(state, token, rel);
                    state = Selector::attr;
                break;}

                case attr_rel_eq:
                case attr_rel_begin:
                case attr_rel_end:
                case attr_rel_match: {
                    token.append((rel = *it));
                break;}

                case attr_token_end: {
                    selector -> addToken(state, token, rel);
                    state = Selector::none;
                break;}

                case type_token: {
                    selector -> addToken(state, token, rel);
                    state = Selector::type;
                break;}

                case direct_token: {
                    if (!token.isEmpty())
                        selector -> addToken(state, token, rel);
                    selector = new Selector(true, selector);
                    state = Selector::tag;
                break;}

                case space_token: {
                    if (state != attr && !token.isEmpty()) {
                        selector -> addToken(state, token, rel);
                        selector = new Selector(false, selector);
                        state = Selector::tag;
                    }
                break;}

                case cont1_token:
                case cont2_token: {
                    // skipping
                break;}

                default: token.append(*it);
            }

            it++;
        }

        free((void *)predicate);
    }

    ////////  Tag //////////

    bool Tag::validTo(Selector * selector) {
        for(QHash<Selector::SState, QString>::Iterator it = selector -> _tokens.begin(); it != selector -> _tokens.end(); it++) {
            switch(it.key()) {
                case Selector::tag: { if (!(it.value() == HTML_PARSER_ANY_ELEMENT || _name == it.value())) return false; break; }
                case Selector::attr: {
                    for(QHash<QString, QPair<char, QString> >::Iterator it = selector -> _attrs.begin(); it != selector -> _attrs.end(); it++)
                        switch(it.value().first) {
                            case Selector::attr_rel_eq: { if (!(it.value().second == HTML_PARSER_ANY_ELEMENT || attrs.value(it.key()) == it.value().second)) return false;  break;}
                            case Selector::attr_rel_begin: { if (!attrs.value(it.key()).startsWith(it.value().second)) return false;  break;}
                            case Selector::attr_rel_end: { if (!attrs.value(it.key()).endsWith(it.value().second)) return false;  break;}
                            case Selector::attr_rel_match: { if (attrs.value(it.key()).indexOf(it.value().second) == -1) return false;  break;}
                            default: qDebug() << "UNSUPPORTED PREDICATE " << it.value().first;
                        };
                    break;
                }
                case Selector::id:  { if (attrs[HTML_PARSER_ID_ATTR] != it.value()) return false; break; }
                case Selector::klass: { //TODO: optimisation needed
                    QStringList node_klasses = attrs[HTML_PARSER_CLASS_ATTR].split(QStringLiteral(" "), QString::SkipEmptyParts);
                    if (node_klasses.isEmpty()) return false;

                    for(QStringList::Iterator it = selector -> klasses.begin(); it != selector -> klasses.end(); it++) {
                        bool finded = false;
                        for(QStringList::Iterator xit = node_klasses.begin(); xit != node_klasses.end(); xit++) // TODO: if list generated each time - remove finded classes for speed up of the proccess of search
                            if ((finded = (*xit) == (*it))) break;

                        if (!finded) return false;
                    }
                    break;
                }
                case Selector::type: {
                    if (!((_name == HTML_PARSER_INPUT_ATTR || _name == HTML_PARSER_SELECT_ATTR) && attrs[HTML_PARSER_TYPE_ATTR] == it.value())) return false;
                    break;
                }
                default: ;
            }
        }

        return true;
    }

    ////////  Document //////////

    void Document::initSoloTags() {
        solo.insert(QStringLiteral("br"), true);
        solo.insert(QStringLiteral("meta"), true);
        solo.insert(QStringLiteral("link"), true);
        solo.insert(QStringLiteral("img"), true);
        solo.insert(QStringLiteral("!DOCTYPE"), true);
    }


    void Document::parse(QIODevice * device) {
        initSoloTags();
        PState state = content;
        char * ch = new char[2](), last = 0;
        QString curr, value; curr.reserve(1024); value.reserve(1024);
        Tag * elem = (root = new Tag(HTML_PARSER_ANY_ELEMENT));
        bool is_closed = false;

        while(!device -> atEnd()) {
            if (device -> getChar(ch)) {
                if (*ch > 0 && *ch < 31) continue; // skip not printable trash

                switch (state) {
                    case comment: {
                        switch(*ch) {
                            case comment_post_token: break; // skip -
                            case close_tag: {
                                if (flags & skip_comment) curr.clear();
                                else  elem -> appendComment(curr);
                                state = content;
                            break;}
                            default:
                                if (*ch > 0)
                                    curr.append(*ch);
                                else
                                    scanUtf8Char(device, curr, ch[0]);
                        }
                    break;}

                    case val:
                    case in_val: {
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

                    case content: {
                        switch(*ch) {
                            case open_tag: {
                                if (!(flags & skip_text) && !curr.isEmpty()) elem -> appendText(curr);
                                state = tag;
                            break;}
                            case space: if (curr.isEmpty()) continue;
                            default: if ((last = *ch) > 0) curr.append(*ch); else scanUtf8Char(device, curr, ch[0]);
                        }
                    break;}

                    default: switch(*ch) {
    //                    case open_tag: {
    //                        qDebug() << "MAYBE NOT USABLE";
    //                        if (!(flags & skip_text) && !curr.isEmpty()) elem -> appendText(curr);
    //                        state = tag;
    //                    break;}

                        case space: {
                            switch(state) {
                                case attr:
                                case val: { if (!curr.isEmpty()) elem -> addAttr(curr, value); state = attr; break; } // proceed attrs without value
                                case tag: { elem = elem -> appendTag(curr); state = attr; break;}
                                default: /*continue*/; // else skip spaces
                            }
                        break;}

                        case attr_rel: {
                            state = val;
                        break;}

                        case close_tag_predicate: {
                            last = *ch;
                            switch (state) {
                                case tag: is_closed = true; break;
                                case attr: state = tag;
                                default: ;
                            }
                        break; }

                        case close_tag: {
                            if (!curr.isEmpty()) {
                                if (state & attr_val) {
                                    elem -> addAttr(curr, value); // proceed attrs without value // if (isSolo(elem)) elem = elem -> parentTag();
                                    if (isSolo(elem)) elem = elem -> parentTag();
                                } else {
                                    if (is_closed) {
                                        if (elem -> name() == curr) elem = elem -> parentTag();// add ignoring of the close tag for solo tags
                                        curr.clear(); is_closed = false;
                                    } else {
                                        if (last != close_tag_predicate) elem = elem -> appendTag(curr);
                                        else elem -> appendTag(curr);
                                    }
                                }
                            } else if (isSolo(elem)) elem = elem -> parentTag();

                            state = content;
                        break;}

                        case comment_post_token: {
                            switch(state) {
                                case tag: if (last == comment_token) { state = comment; curr.clear(); continue; }
                                default: ;
                            }
                        }
                        default: { curr.append((last = *ch)); }
                    }
                }
            }
        }
        delete ch;
    }
}
