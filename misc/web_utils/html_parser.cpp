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
    QString token;
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
    ch = new char[2](); initiator = '\0'; last = '\0';

    curr.reserve(4096);

    elem = root = new HtmlTag("*");

    while(!device -> atEnd()) {
        if (device -> getChar(ch)) {
            if (*ch == '<') {
                if (!curr.isEmpty())
                    elem -> appendText(curr);

                state = tag;
                parseTag(device);
            } else if (*ch == '>') {
                qDebug() << "ERROR " << elem << (*ch);
            } else if (*ch == ' ') {
                // skip spaces
            }
            else curr.append(ch);
        }
    }
    delete ch;
}

void HtmlParser::parseTag(QIODevice * device) {
    while(!device -> atEnd()) {
        if (device -> getChar(ch)) {
            if (*ch == ' ') {
                if (state == tag) {
                    state = attr;
                    elem = elem -> appendTag(curr);
                    parseAttr(device);
                    state = attr;
                }
                else parseAttr(device);

                if (state == content)
                    return;
            } else if (*ch == '>') {
                state = content;

                if (!curr.isEmpty()) {
                    bool close_tag = curr[0] == '/';

                    if (close_tag) {
                        // if (!isSolo(elem))
                        if (elem -> name() == curr.mid(1)) // add ignoring of the close tag for solo tags
                            elem = elem -> parentTag();

                        curr.clear();
                    } else {
                        if (last != '/') elem = elem -> appendTag(curr);
                        else elem -> appendTag(curr);
                    }
                }
                else if (isSolo(elem)) elem = elem -> parentTag();

                return;
            } else {
                curr.append(ch);
                last = *ch;
            }
        }
    }
}

void HtmlParser::parseAttr(QIODevice * device) {
    while(!device -> atEnd()) {
        if (device -> getChar(ch)) {
            if (*ch == '=') {
                state = val;
                parseValue(device);
                elem -> addAttr(curr, value);
                return;
            } else if (*ch == ' ') {
                if (!curr.isEmpty()) // proceed attrs without value
                    elem -> addAttr(curr, value);
                else return;
            } else if (*ch == '>') {
                if (!curr.isEmpty()) // proceed attrs without value
                    elem -> addAttr(curr, value);

                state = content;
                if (isSolo(elem)) elem = elem -> parentTag();
                return;
            } else {
                curr.append(ch);
            }
        }
    }
}

void HtmlParser::parseValue(QIODevice * device) {
    while(!device -> atEnd()) {
        if (device -> getChar(ch)) {
            if (*ch == '"' || *ch == '\'') {
                if (state == val) {
                    initiator = *ch;
                    state = content;
                } else if (state == content) {
                    if (initiator == *ch && last != '\\') {
                       state = attr;
                       return;
                    } else value.append(ch);
                } else {
                    qDebug() << "WRONG STATE";
                    return;
                }
            }
            else value.append(ch);
            last = *ch;
        }
    }
}
