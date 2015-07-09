#ifndef HTML_PARSER
#define HTML_PARSER

#include <qiodevice.h>
#include <qstringbuilder.h>
#include <qhash.h>
#include <qbuffer.h>

#include <qdebug.h>

#define HTML_PARSER_TEXT_BLOCK "text"
#define DEBUG_LIMIT_OUTPUT 100

struct HtmlSelector {
    enum SState { none, tag, attr, id, klass, type };

    inline HtmlSelector(bool direct = false, HtmlSelector * prev_selector = 0) : _direct(direct), prev(prev_selector), next(0) {
        if (prev_selector) prev_selector -> next = this;
    }
    inline void addToken(SState tType, QString & token) {
        switch(tType) { // preparing on multy params
            case attr: {
                QStringList parts = token.split("=", QString::SkipEmptyParts);
                _attrs.insert(parts.first(), parts.length() > 1 ? parts.last() : "");
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

    QStringList klasses;
    QHash<SState, QString> _tokens;
    QHash<QString, QString> _attrs;
    bool _direct;

    HtmlSelector * prev;
    HtmlSelector * next;
};

class HtmlTag;
class HtmlSet : public QList<HtmlTag *> {
public:
    HtmlSet & find(HtmlSelector * selector, HtmlSet & set);
};

class HtmlTag {
public:
    HtmlTag(QString tag, HtmlTag * parent_tag = 0) : _level(parent_tag ? parent_tag -> level() + 1 : 0), _name(tag), parent(parent_tag) {}
    ~HtmlTag() { qDeleteAll(tags); }

    inline QString name() const { return _name; }
    inline int level() const { return _level; }
    QHash<QString, QString> attributes() const { return attrs; }
    HtmlSet children() const { return tags; }

    inline HtmlTag * parentTag() { return parent; }

    inline void addAttr(QString & name, QString & val) { attrs.insert(name, val);  name.clear(); val.clear(); }
    inline HtmlTag * appendTag(QString & tname) {
        HtmlTag * newTag = new HtmlTag(tname, this); tname.clear();
        tags.append(newTag);
        return newTag;
    }
    inline void appendText(QString & val) {
        QString tnm(HTML_PARSER_TEXT_BLOCK);
        HtmlTag * newTag = appendTag(tnm);
        QString nm(HTML_PARSER_TEXT_BLOCK);
        newTag -> addAttr(nm, val); val.clear();
    }

    bool validTo(HtmlSelector * selector) {
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

    friend QDebug operator<< (QDebug debug, const HtmlTag & c) {
        QString attrStr;
        QHash<QString, QString> vals = c.attributes();

        for (QHash<QString, QString>::iterator it = vals.begin(); it != vals.end(); ++it)
            attrStr.append("(" + it.key() + " : " + (it.value().size() > DEBUG_LIMIT_OUTPUT ? (it.value().mid(0, DEBUG_LIMIT_OUTPUT / 2) % "..." % it.value().mid(it.value().size() - DEBUG_LIMIT_OUTPUT / 2, DEBUG_LIMIT_OUTPUT / 2)) : it.value()) + ")");

        qDebug("%s%s%s%s%s", QString(c.level() * 3, ' ').toUtf8().constData(), c.name().toUtf8().constData(), " ||| [", attrStr.toUtf8().constData(), "]");

        foreach(HtmlTag * it, c.children())
            qDebug() << (*it);

        return debug;
    }

private:
    int _level;
    QString _name;
    QHash<QString, QString> attrs;
    HtmlSet tags;
    HtmlTag * parent;
};

class HtmlParser {
    enum PState { content, tag, attr, val };

public:
    inline HtmlParser(QIODevice * device) : state(content) { parse(device); }
    inline HtmlParser(QString & str) : state(content) {
        QByteArray ar = str.toUtf8();
        QBuffer stream(&ar);
        stream.open(QIODevice::ReadOnly);
        parse((QIODevice *)&stream);
        stream.close();
    }

    ~HtmlParser() { delete root; }

    QList<HtmlTag *> search(QString predicate) {
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

    inline void output() { qDebug() << (*root); }
private:
//    void proceedSearch(HtmlSelector * selector, HtmlTag * node, HtmlSet & res) {
//        HtmlSet nodes = node -> children();
//        for(HtmlSet::Iterator tag = nodes.begin(); tag != nodes.end(); tag++) {
//            if ((*tag) -> validTo(selector)) {
//                if (selector -> next && !(*tag) -> children().isEmpty())
//                    proceedSearch(selector -> next, (*tag), res);
//                else
//                    res.append((*tag));
//            }
//            else if (!selector -> _direct && !(*tag) -> children().isEmpty())
//                proceedSearch(selector, (*tag), res);
//        }
//    }

    inline bool isSolo(HtmlTag * tag) { return solo.contains(tag -> name()); }

    inline void initSoloTags() {
        solo.insert("br", true);
        solo.insert("meta", true);
        solo.insert("link", true);
    }

    void parse(QIODevice * device) {
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

    void parseTag(QIODevice * device) {
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

    void parseAttr(QIODevice * device) {
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

    void parseValue(QIODevice * device) {
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

    QHash<QString, bool> solo;
    HtmlTag * root, * elem;
    QString curr, value;
    PState state;
    char * ch, last, initiator;
};

#endif // HTML_PARSER
