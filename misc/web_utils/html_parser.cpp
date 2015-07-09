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

////////  HtmlTag //////////

bool HtmlTag::validTo(HtmlSelector * selector) {
    bool res = true;
    for(QHash<HtmlSelector::SState, QString>::Iterator it = selector -> _tokens.begin(); it != selector -> _tokens.end(); it++) {
        if (!res) break;

        switch(it.key()) {
            case HtmlSelector::tag: { res |= (it.value() == "*" || _name == it.value()); break; }
            case HtmlSelector::attr: {
                for(QHash<QString, QString>::Iterator it = selector -> _attrs.begin(); it != selector -> _attrs.end(); it++)
                    if (attrs.value(it.key()) != it.value()) return false;
                break;
            }
            case HtmlSelector::id:  { res |= attrs["id"] == it.value(); break; }
            case HtmlSelector::klass: { //TODO: optimisation needed
                QStringList node_klasses = attrs["class"].split(" ", QString::SkipEmptyParts);

                if (res |= !node_klasses.isEmpty()) {
                    for(QStringList::Iterator it = selector -> klasses.begin(); it != selector -> klasses.end(); it++) {
                        bool finded = false;
                        for(QStringList::Iterator xit = node_klasses.begin(); xit != node_klasses.end(); xit++) // TODO: if list generated each time - remove finded classes for speed up of the proccess of search
                            if ((finded = (*xit) == (*it))) break;

                        if (!finded) return false;
                    }
                }
                break;
            }
            case HtmlSelector::type: {
                res |= ((_name == "input" || _name == "select") && attrs["type"] == it.value());
                break;
            }
            default: ;
        }
    }

    return res;
}

////////  HtmlParser //////////

HtmlSet HtmlParser::search(QString predicate) {
    HtmlSelector::SState state = HtmlSelector::tag;
    HtmlSelector * selector = new HtmlSelector(), * head = selector;
    QString token;

    for(QString::Iterator it = predicate.begin(); it != predicate.end(); it++) {
        if ((*it) == '#') {
            selector -> addToken(state, token);
            state = HtmlSelector::id;
        } else if ((*it) == '.') {
            selector -> addToken(state, token);
            state = HtmlSelector::klass;
        } else if ((*it) == '[' || (*it) == ',') {
            selector -> addToken(state, token);
            state = HtmlSelector::attr;
        } else if ((*it) == ']') {
            selector -> addToken(state, token);
            state = HtmlSelector::none;
        } else if ((*it) == ':') {
            selector -> addToken(state, token);
            state = HtmlSelector::type;
        } else if ((*it) == '>') {
            selector -> addToken(state, token);
            selector = new HtmlSelector(true, selector);
        } else if ((*it) == ' ') {
            if (state != attr) {
                selector -> addToken(state, token);
                selector = new HtmlSelector(false, selector);
            }
        } else if ((*it) == '\'' || (*it) == '"') {
            // skipping
        } else token.append((*it));
    }

    HtmlSet res;
    return root -> children().find(head, res);
}

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
