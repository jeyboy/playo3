#include "html_parser.h"

namespace Core {
    namespace Web {
        namespace Html {
            ////////  Set //////////
                QString Set::link() { return (isEmpty()) ? QString() : first() -> link(); }
                QString Set::text() { return (isEmpty()) ? QString() : first() -> text(); }
                QString Set::value(const QString & name) { return (isEmpty()) ? QString() : first() -> value(name); }

                Set & Set::find(const Selector * selector, Set & set, bool findFirst) const {
                    for(Set::ConstIterator tag = cbegin(); tag != cend(); tag++) {
                        if ((*tag) -> validTo(selector)) {
                                if (selector -> next) {
                                    if (selector -> next -> isBackward()) {
                                        (*tag) -> backwardFind(selector -> next, set);
                                        if (findFirst) break;
                                    } else if (!(*tag) -> children().isEmpty())
                                        (*tag) -> children().find(selector -> next, set);
                                }
                                else {
                                    set.append((*tag));
                                    if (findFirst) break;
                                }
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
                            if (parts.length() > 1)
                                _attrs.insert(parts.first(), QPair<char, QString>(rel, parts.last()));
                            else
                                _attrs.insert(parts.first(), QPair<char, QString>(attr_rel_eq, tkn_any_elem));
                            rel = attr_rel_eq;
                        break;}
                        case klass: {
                            klasses.append(token.split(' ', QString::SkipEmptyParts));
                        break;}
                        default:;
                    }

                    if (tType == tag && token.isEmpty())
                        token = tkn_any_elem;

                    _tokens.insert(tType, token);
                    token.clear();
                }

                //TODO: add :3 - position limitation
                Selector::Selector(const char * predicate) : sType(forward), prev(0), next(0) {
                    Selector::SState state = Selector::tag;
                    Selector * selector = this;
                    QString token; token.reserve(128);
                    char rel;
                    const char * it = predicate;
                    bool in_attr = false;

                    while(*it) {
                        switch(*it) {
                            case attr_token_end:
                            case type_token:
                            case class_token:
                            case id_token: {
                                in_attr &= attr_token_end != *it;

                                if (!in_attr) {
                                    if (!token.isEmpty()) selector -> addToken(state, token, rel);
                                    state = (SState)*it;
                                } else token.append(*it);
                            break;}

                            case attr_token:
                            case attr_separator: {
                                in_attr |= attr_token == *it;
                                selector -> addToken(state, token, rel);
                                state = Selector::attr;
                            break;}

                            case attr_rel_eq:
                            case attr_rel_begin:
                            case attr_rel_end:
                            case attr_rel_match:
                            case attr_rel_not: {
                                token.append((rel = *it));
                            break;}

                            case back_direct_token:
                            case direct_token: {
                                if (!token.isEmpty()) selector -> addToken(state, token, rel);
                                selector = new Selector((SType)*it, selector);
                                state = Selector::tag;
                            break;}

                            case space_token: {
                                if (state != attr) {
                                    if (!token.isEmpty())
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
                }

                ////////  Tag //////////
                //INFO: some servers very sensitive to params part and payload part separation ...
                // appendable - appends inputs from vals, which not finded in form
                // default_url - if form did not contains action, we should use source url
                void Tag::serializeForm(QUrl & url, QByteArray & payload, const QHash<QString, QString> & vals, const FormSerializationFlags & flags, const QString & default_url) {
                    QString action = value(attr_action);
                    url = QUrl(action.isEmpty() ? default_url : action);

                    Set inputs = find("input") << find("select");
                    QHash<QString, QString> url_vals(vals);

                    if (!inputs.isEmpty()) {
                        QUrlQuery query = QUrlQuery();

                        for(Set::Iterator input = inputs.begin(); input != inputs.end(); input++) {
                            QString inp_name = (*input) -> value(attr_name);
                            QString inp_val = url_vals.take(inp_name);
                            if (inp_val.isEmpty()) inp_val = (*input) -> value();

                            query.addQueryItem(
                                inp_name,
                                flags & fsf_percent_encoding ? QUrl::toPercentEncoding(inp_val) : inp_val
                            );
                        }

                        if (flags & fsf_append_vals_from_hash && !url_vals.isEmpty())
                            for(QHash<QString, QString>::Iterator it = url_vals.begin(); it != url_vals.end(); it++)
                                query.addQueryItem(
                                    it.key(),
                                    flags & fsf_percent_encoding ? QUrl::toPercentEncoding(it.value()) : it.value()
                                );

                        payload = query.toString().toUtf8();
                    }
                }
//                QUrl Tag::serializeFormToUrl(const QHash<QString, QString> & vals, const FormSerializationFlags & flags, const QString & default_url) { // not full support of inputs
//                    QString action = value(attr_action);
//                    QUrl url = QUrl(action.isEmpty() ? default_url : action);

//                    Set inputs = find("input") << find("select");
//                    QHash<QString, QString> url_vals(vals);

//                    if (!inputs.isEmpty()) {
//                        QUrlQuery query = QUrlQuery(url.query());

//                        for(Set::Iterator input = inputs.begin(); input != inputs.end(); input++) {
//                            QString inp_name = (*input) -> value(attr_name);
//                            QString inp_val = url_vals.take(inp_name);
//                            if (inp_val.isEmpty()) inp_val = (*input) -> value();

//                            query.addQueryItem(inp_name, flags & fsf_percent_encoding ? QUrl::toPercentEncoding(inp_val) : inp_val);
//                        }

//                        if (flags & fsf_append_vals_from_hash && !url_vals.isEmpty())
//                            for(QHash<QString, QString>::Iterator it = url_vals.begin(); it != url_vals.end(); it++)
//                                query.addQueryItem(it.key(), flags & fsf_percent_encoding ? QUrl::toPercentEncoding(it.value()) : it.value());

//                        url.setQuery(query);
//                    }

//                    return url;
//                }
                QUrl Tag::serializeFormToUrl(const QHash<QString, QString> & vals, const FormSerializationFlags & flags, const QString & default_url) { // not full support of inputs
                    QUrl url;
                    QByteArray payload;

                    serializeForm(url, payload, vals, flags, default_url);
                    url.setQuery(QUrlQuery(payload + '&' + url.query()));
                    return url;
                }
                QString Tag::toText() const {
                    if (_name == tkn_text_block)
                        return attrs.value(tkn_text_block);
                    else {
                        QString result;

                        for(Set::ConstIterator tag = tags.cbegin(); tag != tags.cend(); tag++)
                            result += (*tag) -> toText();

                        return result;
                    }
//                    const Tag * text = (_name == tkn_text_block ? this : childTag(tkn_text_block));
//                    return text ? text -> attrs.value(tkn_text_block) : QString();
                }
                QString Tag::toHtml() const {
                    if (_name == tkn_text_block)
                        return attrs.value(tkn_text_block);
                    else {
                        QString result = QString(_level * 2, ' ') % '<' % _name;

                        for(QHash<QString, QString>::ConstIterator attr = attrs.constBegin(); attr != attrs.constEnd(); attr++)
                            result = result % ' ' % attr.key() % QLatin1String("=\"") % attr.value() % '"';

                        result = result % '>';

                        for(Set::ConstIterator tag = tags.cbegin(); tag != tags.cend(); tag++)
                            result += (*tag) -> toHtml();

                        return Document::solo.contains(_name) && tags.isEmpty() ? result : QString(result % QLatin1String("</") % _name % '>');
                    }
                }

                bool Tag::validTo(const Selector * selector) {
                    for(QHash<Selector::SState, QString>::ConstIterator it = selector -> _tokens.cbegin(); it != selector -> _tokens.cend(); it++) {
                        switch(it.key()) {
                            case Selector::tag: { if (!(it.value() == tkn_any_elem || _name == it.value())) return false; break; }
                            case Selector::attr: {
                                for(QHash<QString, QPair<char, QString> >::ConstIterator it = selector -> _attrs.cbegin(); it != selector -> _attrs.cend(); it++) {
                                    QString tag_value = it.key() == QLatin1String("text") ? text() : attrs.value(it.key());
                                    QString selector_value = it.value().second;

                                    switch(it.value().first) {
                                        case Selector::attr_rel_eq: {
                                            if (!(attrs.contains(it.key()) && (selector_value == tkn_any_elem || tag_value == selector_value)))
                                                return false;
                                            break;}
                                        case Selector::attr_rel_begin: {
                                            if (!tag_value.startsWith(selector_value))
                                                return false;
                                            break;}
                                        case Selector::attr_rel_end: {
                                            if (!tag_value.endsWith(selector_value))
                                                return false;
                                            break;}
                                        case Selector::attr_rel_match: {
                                            if (tag_value.indexOf(selector_value) == -1)
                                                return false;
                                            break;}
                                        case Selector::attr_rel_not: {
                                            if (tag_value.indexOf(selector_value) != -1)
                                                return false;
                                            break;}

                                        default: qDebug() << "UNSUPPORTED PREDICATE " << it.value().first;
                                    };
                                }
                                break;
                            }
                            case Selector::id:  { if (attrs[attr_id] != it.value()) return false; break; }
                            case Selector::klass: { //TODO: optimisation needed
                                QStringList node_klasses = attrs[attr_class].split(tkn_split, QString::SkipEmptyParts);
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
                                if (!((_name == tag_input || _name == tag_select) && attrs[attr_type] == it.value())) return false;
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
                    else
                        if (selector -> isBackward()) {
                            if (parent -> parent)
                                parent -> backwardFind(selector, set);
                        }
                        else parent -> children().find(selector, set);

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

                QHash<QString, bool> Document::solo = {
                    {tag_br, true}, {tag_meta, true}, {tag_link, true}, {tag_img, true}, {tag_doctype, true}, {tag_xml, true}, {tag_input, true}
                };

                void Document::parse(QIODevice * device) {
                    PState state = content;
                    char * ch = new char[2](), last = 0, del = 0;
                    QString name, value; name.reserve(1024); value.reserve(1024);
                    Tag * elem = (root = new Tag(tkn_any_elem));
                    bool is_closed = false;

                    while(true) {
                        if (device -> getChar(ch)) {
                            if (*ch > 0 && *ch < 32) continue; // skip not printable trash
                            text.push_back(*ch);

                            switch (state) {
                                case val:
                                case in_val: {
                                    switch(*ch) {
                                        case content_del1:
                                        case content_del2: {
                                            switch(state) {
                                                case val: { state = in_val; del = *ch; break;}
                                                case in_val: {
                                                    if (del == *ch) {
                                                        elem -> addAttr(name, value);
                                                        state = attr;
                                                    }
                                                    else value.append(*ch);
                                                break;}
                                                default: { qDebug() << "WRONG STATE" << state; return; }
                                            }
                                        break;}
                                        case code_start: { value.append(parseCode(device, ch)); break; }
                                        default:
                                            if (*ch > 0) value.append(*ch);
                                            else toUtf8(charset, device, value, ch[0]);
                                    }
                                break;}

                                case content: {
                                    switch(*ch) {
                                        case open_tag: {
                                            if (last == close_tag_predicate && elem -> is_script())
                                                name.append(*ch); // javascript comments
                                            else {
                                                if (!(flags & skip_text) && !name.isEmpty()) elem -> appendText(name);
                                                state = tag;
                                            }
                                        break;}
                                        case code_start: { name.append(parseCode(device, ch)); break; } // &quot; and etc
                                        case space: if (name.isEmpty()) continue;
                                        default:
                                            if ((last = *ch) > 0) name.append(*ch);
                                            else toUtf8(charset, device, name, ch[0]);
                                    }
                                break;}

                                case service: {
                                    switch(*ch) {
                                        case close_tag: {
                                            // block closing on > in middle of text
                                            bool closed_correctly = last == ']';
                                            bool started_correctly = name[0] == '[';

                                            if (started_correctly == closed_correctly) { // <![CDATA[ */ @import url(/css/al/ie6.css?26); /* ]]>
                                                if (flags & skip_service) name.clear();
                                                else elem -> appendService(name);
                                                state = content; // need to check
                                                continue;
                                            }
                                        }
                                        default:
                                            if (*ch > 0) name.append(*ch);
                                            else toUtf8(charset, device, name, ch[0]);
                                    }
                                    last = *ch;
                                break;}

                                case comment: {
                                    switch(*ch) {
//                                        case comment_post_token: break; // skip -
                                        case close_tag: {
                                            if (last == comment_post_token) {
                                                if (flags & skip_comment) name.clear();
                                                else elem -> appendComment(name);
                                                state = content;
                                                break;
                                            }
                                        }
                                        default:
                                            if ((last = *ch) > 0) name.append(*ch);
                                            else toUtf8(charset, device, name, ch[0]);
                                    }
                                break;}

                                default: switch(*ch) {
                                    case space: {
                                        switch(state) {
                                            case attr:
                                            case val: { if (!name.isEmpty()) elem -> addAttr(name, value); state = attr; break; } // proceed attrs without value
                                            case tag: {
                                                if (last != close_tag_predicate) {
                                                    elem = elem -> appendTag(name);
                                                    state = attr;
                                                }
                                            break;}
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
                                        if (!(charset_finded || using_default_charset))
                                            checkCharset(elem);

                                        if (!name.isEmpty()) {
                                            if (state & attr_val) {
                                                if (name[0] != '?') // ignore ?>
                                                    elem -> addAttr(name, value); // proceed attrs without value // if (isSolo(elem)) elem = elem -> parentTag();

                                                if (isSolo(elem)) elem = elem -> parentTag();
                                            } else {
                                                if (is_closed) {
                                                    if (elem -> name() == name) elem = elem -> parentTag();// add ignoring of the close tag for solo tags
                                                    name.clear(); is_closed = false;
                                                } else {
                                                    if (last != close_tag_predicate && !isSolo(name)) elem = elem -> appendTag(name);
                                                    else elem -> appendTag(name);
                                                }
                                            }
                                        } else {
                                            if (isSolo(elem) || last == close_tag_predicate) elem = elem -> parentTag();
                                        }

                                        state = content;

                                        break;}

                                    default: {
                                        if (state == tag && last == service_token) {
                                            name.clear();
                                            state = (*ch == comment_post_token) ? comment : service;
                                            name.append((last = *ch));
                                        continue;}

                                        name.append((last = *ch));
                                    }
                                }
                            }
                        } else break;
                    }
                    delete ch;
                }
        }
    }
}
