#include "html_parser.h"

////////  HtmlSet //////////

HtmlSet & HtmlSet::find(HtmlSelector * selector, HtmlSet & set) {
    for(HtmlSet::Iterator tag = begin(); tag != end(); tag++) {
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

////////  HtmlSelector //////////

void HtmlSelector::addToken(SState & tType, QString & token, char & rel) {
    switch(tType) {
        case attr: {
            QStringList parts = token.split(rel, QString::SkipEmptyParts);
            QPair<char, QString> newAttr(rel, parts.length() > 1 ? parts.last() : "*");
            _attrs.insert(parts.first(), newAttr); rel = attr_rel_eq;
            break;
        }
        case klass: {
            klasses.append(token.split(" ", QString::SkipEmptyParts));
            break;
        }
    }

    _tokens.insert(tType, token);
    token.clear();
}

HtmlSelector::HtmlSelector(char * predicate) : _direct(false), prev(0), next(0) {
    HtmlSelector::SState state = HtmlSelector::tag;
    HtmlSelector * selector = this;
    QString token; token.reserve(128);
    char rel, * it = predicate;

    while(*it) {
        switch(*it) {
            case id_token: {
                selector -> addToken(state, token, rel);
                state = HtmlSelector::id;
            break;}

            case class_token: {
                selector -> addToken(state, token, rel);
                state = HtmlSelector::klass;
            break;}

            case attr_token:
            case attr_separator: {
                selector -> addToken(state, token, rel);
                state = HtmlSelector::attr;
            break;}

            case attr_rel_eq:
            case attr_rel_begin:
            case attr_rel_end:
            case attr_rel_match: {
                token.append((rel = *it));
            break;}

            case attr_token_end: {
                selector -> addToken(state, token, rel);
                state = HtmlSelector::none;
            break;}

            case type_token: {
                selector -> addToken(state, token, rel);
                state = HtmlSelector::type;
            break;}

            case direct_token: {
                if (!token.isEmpty())
                    selector -> addToken(state, token, rel);
                selector = new HtmlSelector(true, selector);
                state = HtmlSelector::tag;
            break;}

            case space_token: {
                if (state != attr && !token.isEmpty()) {
                    selector -> addToken(state, token, rel);
                    selector = new HtmlSelector(false, selector);
                    state = HtmlSelector::tag;
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
}

////////  HtmlTag //////////

bool HtmlTag::validTo(HtmlSelector * selector) {
    for(QHash<HtmlSelector::SState, QString>::Iterator it = selector -> _tokens.begin(); it != selector -> _tokens.end(); it++) {
        switch(it.key()) {
            case HtmlSelector::tag: { if (!(it.value() == "*" || _name == it.value())) return false; break; }
            case HtmlSelector::attr: {
                for(QHash<QString, QPair<char, QString> >::Iterator it = selector -> _attrs.begin(); it != selector -> _attrs.end(); it++)
                    switch(it.value().first) {
                        case HtmlSelector::attr_rel_eq: { if (!(it.value().second == "*" || attrs.value(it.key()) == it.value().second)) return false;  break;}
                        case HtmlSelector::attr_rel_begin: { if (!attrs.value(it.key()).startsWith(it.value().second)) return false;  break;}
                        case HtmlSelector::attr_rel_end: { if (!attrs.value(it.key()).endsWith(it.value().second)) return false;  break;}
                        case HtmlSelector::attr_rel_match: { if (attrs.value(it.key()).indexOf(it.value().second) == -1) return false;  break;}
                        default: qDebug() << "UNSUPPORTED PREDICATE " << it.value().first;
                    };
                break;
            }
            case HtmlSelector::id:  { if (attrs["id"] != it.value()) return false; break; }
            case HtmlSelector::klass: { //TODO: optimisation needed
                QStringList node_klasses = attrs["class"].split(" ", QString::SkipEmptyParts);
                if (node_klasses.isEmpty()) return false;

                for(QStringList::Iterator it = selector -> klasses.begin(); it != selector -> klasses.end(); it++) {
                    bool finded = false;
                    for(QStringList::Iterator xit = node_klasses.begin(); xit != node_klasses.end(); xit++) // TODO: if list generated each time - remove finded classes for speed up of the proccess of search
                        if ((finded = (*xit) == (*it))) break;

                    if (!finded) return false;
                }
                break;
            }
            case HtmlSelector::type: {
                if (!((_name == "input" || _name == "select") && attrs["type"] == it.value())) return false;
                break;
            }
            default: ;
        }
    }

    return true;
}

////////  HtmlParser //////////

void HtmlParser::initSoloTags() {
    solo.insert("br", true);
    solo.insert("meta", true);
    solo.insert("link", true);
}

void HtmlParser::parse(QIODevice * device) {
    initSoloTags();
    PState state = content;
    char * ch = new char[2](), initiator = 0, last = 0;
    QString curr, value; curr.reserve(4096); value.reserve(1024);
    HtmlTag * elem = (root = new HtmlTag("*"));
    bool is_closed = false;

    while(!device -> atEnd()) {
        if (device -> getChar(ch)) {
            switch (state) {
                case content: {
                    switch(*ch) {
                        case open_tag: {
                            if (!curr.isEmpty()) elem -> appendText(curr);
                            state = tag;
                        break;}
                        case space: if (curr.isEmpty()) continue;
                        default: { curr.append((last = *ch)); }
                    }
                break;}

                default: switch(*ch) {
                    case open_tag: {
                        if (!curr.isEmpty()) elem -> appendText(curr);
                        state = tag;
                    break;}

                    case space: {
                        switch(state) {
                            case attr:
                            case val: { if (!curr.isEmpty()) elem -> addAttr(curr, value); state = attr; break; } // proceed attrs without value
                            case tag: { elem = elem -> appendTag(curr); state = attr; break;}
                            default: /*continue*/; // else skip spaces
                        }
                    break;}

                    case attr_rel: {
                        parseValue(device, value, ch, initiator, last, state);
                        elem -> addAttr(curr, value);
                        state = attr;
                    break;}

                    case close_tag_predicate: { is_closed = state == tag; break; }

                    case close_tag: {
                        if (!curr.isEmpty()) {
                            if (state & attr_val)
                                elem -> addAttr(curr, value); // proceed attrs without value // if (isSolo(elem)) elem = elem -> parentTag();
                            else {
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

                    default: { curr.append((last = *ch)); }
                }
            }
        }
    }
    delete ch;
}

void HtmlParser::parseValue(QIODevice * device, QString & value, char * ch, char & initiator, char & last, PState & state) {
    state = val;
    while(!device -> atEnd()) {
        if (device -> getChar(ch)) {
            switch(*ch) {
                case content_del1:
                case content_del2: {
                    switch(state) {
                        case val: { initiator = *ch; state = content; break;}
                        case content: { if (initiator == *ch && last != mean_sym) return; value.append(ch); break;}
                        default: { qDebug() << "WRONG STATE" << state; return; }
                    }
                break;}
                default: value.append((last = *ch));
            }
        }
    }
}
