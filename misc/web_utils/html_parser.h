#ifndef HTML_PARSER
#define HTML_PARSER

#include <qiodevice.h>
#include <qstringbuilder.h>
#include <qhash.h>
#include <qbuffer.h>

#include <qdebug.h>

#define HTML_PARSER_TEXT_BLOCK "text"

class HtmlTag {
public:
    HtmlTag(QString tag, HtmlTag * parent_tag = 0) : _level(parent_tag ? parent_tag -> level() + 1 : 0), _name(tag), parent(parent_tag) {}
    ~HtmlTag() { qDeleteAll(tags); }

    inline QString name() const { return _name; }
    inline int level() const { return _level; }
    QHash<QString, QString> attributes() const { return attrs; }
    QList<HtmlTag *> childs() const { return tags; }

    inline HtmlTag * parentTag() { return parent; }

    inline void addAttr(QString & name, QString & val) { attrs.insert(name, val);  name.clear(); val.clear(); }
    inline HtmlTag * appendTag(QString & tname) {
        HtmlTag * newTag = new HtmlTag(tname, this); tname.clear();
        tags.append(newTag);
        return newTag;
    }
    inline void appendTag(QString & tname, QString & val) {
        HtmlTag * newTag = appendTag(tname);
        QString nm(HTML_PARSER_TEXT_BLOCK);
        newTag -> addAttr(nm, val); val.clear();
    }

    friend QDebug operator<< (QDebug debug, const HtmlTag & c) {
        QString attrStr;
        QHash<QString, QString> vals = c.attributes();

        for (QHash<QString, QString>::iterator it = vals.begin(); it != vals.end(); ++it)
            attrStr.append("(" + it.key() + " : " + it.value());

        debug.space() << QString(c.level() * 3, ' ') << c.name() << " ||| " << attrStr;

        foreach(HtmlTag * it, c.childs())
            qDebug() << (*it);

        return debug;
    }

private:
    int _level;
    QString _name;
    QHash<QString, QString> attrs;
    QList<HtmlTag *> tags;
    HtmlTag * parent;
};

class HtmlParser {
    enum PState {
        content,
        tag,
        attr,
        val
    };

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

    inline void output() { qDebug() << (*root); }
private:
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
                    if (!curr.isEmpty()) {
                        QString text(HTML_PARSER_TEXT_BLOCK);
                        elem -> appendTag(text, curr);
                    }

                    state = tag;
                    parseTag(device);
                } else if (*ch == '>') {
                    qDebug() << "MAIN " << (*ch);
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
