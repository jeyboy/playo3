#include "html_parser.h"

////////  Set //////////
namespace Html {
    QString Set::link() { return (isEmpty()) ? QString() : first() -> link(); }
    QString Set::text() { return (isEmpty()) ? QString() : first() -> text(); }
    QString Set::value(QString name) { return (isEmpty()) ? QString() : first() -> value(name); }

    Set & Set::find(const Selector * selector, Set & set) {
        for(Set::Iterator tag = begin(); tag != end(); tag++) {
            if ((*tag) -> validTo(selector)) {
                    if (selector -> next) {
                        if (selector -> next -> isBackward())
                            (*tag) -> backwardFind(selector -> next, set);
                        else if (!(*tag) -> children().isEmpty())
                            (*tag) -> children().find(selector -> next, set);
                    }
                    else set.append((*tag));
            }
            else if (!selector -> isDirect() && !(*tag) -> children().isEmpty())
                (*tag) -> children().find(selector, set);
        }

        return set;
    }

    QHash<QString, QString> & Set::findLinks(const Selector * selector, QHash<QString, QString> & links) {
        for(Set::Iterator tag = begin(); tag != end(); tag++) {
            if ((*tag) -> validTo(selector)) {
                if (selector -> next) {
                    if (selector -> next -> isBackward())
                        (*tag) -> backwardFindLinks(selector -> next, links);
                    else if (!(*tag) -> children().isEmpty())
                        (*tag) -> children().findLinks(selector -> next, links);
                } else if ((*tag) -> is_link())
                    links.insert((*tag) -> link(), (*tag) -> text());
            }
            else if (!selector -> isDirect() && !(*tag) -> children().isEmpty())
                (*tag) -> children().findLinks(selector, links);
        }

        return links;
    }

    ////////  Selector //////////

    void Selector::addToken(SState & tType, QString & token, char & rel) {
        switch(tType) {
            case attr: {
                QStringList parts = token.split(rel, QString::SkipEmptyParts);
                QPair<char, QString> newAttr(rel, parts.length() > 1 ? parts.last() : any_elem_token);
                _attrs.insert(parts.first(), newAttr); rel = attr_rel_eq;
            break;}
            case klass: {
                klasses.append(token.split(" ", QString::SkipEmptyParts));
            break;}
            default:;
        }

        _tokens.insert(tType, token);
        token.clear();
    }

    Selector::Selector(const char * predicate) : sType(forward), prev(0), next(0) {
        Selector::SState state = Selector::tag;
        Selector * selector = this;
        QString token; token.reserve(128);
        char rel;
        const char * it = predicate;

        while(*it) {
            switch(*it) {
                case attr_token_end:
                case type_token:
                case class_token:
                case id_token: {
                    if (!token.isEmpty()) selector -> addToken(state, token, rel);
                    state = (SState)*it;
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

                case back_direct_token:
                case direct_token: {
                    if (!token.isEmpty()) selector -> addToken(state, token, rel);
                    selector = new Selector((SType)*it, selector);
                    state = Selector::tag;
                break;}

                case space_token: {
                    if (state != attr && !token.isEmpty()) {
                        selector -> addToken(state, token, rel);
                        selector = new Selector(forward, selector);
                        state = Selector::tag;
                    }
                break;}

                case cont1_token:
                case cont2_token: break; // skipping

                default: token.append(*it);
            }

            it++;
        }

        if (!token.isEmpty()) selector -> addToken(state, token, rel);
        free((void *)predicate);
    }

    ////////  Tag //////////

    bool Tag::validTo(const Selector * selector) {
        for(QHash<Selector::SState, QString>::ConstIterator it = selector -> _tokens.cbegin(); it != selector -> _tokens.cend(); it++) {
            switch(it.key()) {
                case Selector::tag: { if (!(it.value() == any_elem_token || _name == it.value())) return false; break; }
                case Selector::attr: {
                    for(QHash<QString, QPair<char, QString> >::ConstIterator it = selector -> _attrs.cbegin(); it != selector -> _attrs.cend(); it++)
                        switch(it.value().first) {
                            case Selector::attr_rel_eq: { if (!(it.value().second == any_elem_token || attrs.value(it.key()) == it.value().second)) return false;  break;}
                            case Selector::attr_rel_begin: { if (!attrs.value(it.key()).startsWith(it.value().second)) return false;  break;}
                            case Selector::attr_rel_end: { if (!attrs.value(it.key()).endsWith(it.value().second)) return false;  break;}
                            case Selector::attr_rel_match: { if (attrs.value(it.key()).indexOf(it.value().second) == -1) return false;  break;}
                            default: qDebug() << "UNSUPPORTED PREDICATE " << it.value().first;
                        };
                    break;
                }
                case Selector::id:  { if (attrs[id_token] != it.value()) return false; break; }
                case Selector::klass: { //TODO: optimisation needed
                    QStringList node_klasses = attrs[class_token].split(split_token, QString::SkipEmptyParts);
                    if (node_klasses.isEmpty()) return false;

                    for(QStringList::ConstIterator it = selector -> klasses.cbegin(); it != selector -> klasses.cend(); it++) {
                        bool finded = false;
                        for(QStringList::Iterator xit = node_klasses.begin(); xit != node_klasses.end(); xit++) // TODO: if list generated each time - remove finded classes for speed up of the proccess of search
                            if ((finded = (*xit) == (*it))) break;

                        if (!finded) return false;
                    }
                    break;
                }
                case Selector::type: {
                    if (!((_name == input_token || _name == select_token) && attrs[type_token] == it.value())) return false;
                    break;
                }
                default: ;
            }
        }

        return true;
    }

    Set & Tag::backwardFind(Selector * selector, Set & set) {
        if (!parent) return set;

        if (parent -> validTo(selector))
            selector = selector -> next;

        if (!selector)
            set.append(parent);
        else if (selector -> isBackward() && parent -> parent)
            parent -> backwardFind(selector, set);

        return set;
    }

    QHash<QString, QString> & Tag::backwardFindLinks(Selector * selector, QHash<QString, QString> & links) {
        if (!parent) return links;

        if (parent -> validTo(selector))
            selector = selector -> next;

        if (!selector) {
            if (parent -> is_link())
                links.insert(parent -> link(), parent -> text());
        } else if (selector -> isBackward() && parent -> parent)
            parent -> backwardFindLinks(selector, links);

        return links;
    }

    ////////  Document //////////

    void Document::initSoloTags() {
        html_entities.insert("nbsp", ' ');
        html_entities.insert("amp", '&');
        html_entities.insert("lt", '<');
        html_entities.insert("gt", '>');

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
        Tag * elem = (root = new Tag(any_elem_token));
        bool is_closed = false;

        while(true) {
            if (device -> getChar(ch)) {
                if (*ch > 0 && *ch < 31) continue; // skip not printable trash

                switch (state) {
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
                            case code_start: { value.append(parseCode(device, ch)); break; }
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
                            case code_start: { curr.append(parseCode(device, ch)); break; }
                            case space: if (curr.isEmpty()) continue;
                            default:
                                if ((last = *ch) > 0)
                                    curr.append(*ch);
                                else
                                    scanUtf8Char(device, curr, ch[0]);
                        }
                    break;}

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

                    default: switch(*ch) {
                        case space: {
                            switch(state) {
                                case attr:
                                case val: { if (!curr.isEmpty()) elem -> addAttr(curr, value); state = attr; break; } // proceed attrs without value
                                case tag: { elem = elem -> appendTag(curr); state = attr; break;}
                                default: /*continue*/; // else skip spaces
                            }
                        break;}

                        case attr_rel: state = val; break;

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
            } else break;
        }
        delete ch;
    }
}
