#ifndef HTML_PARSER
#define HTML_PARSER

#include <qiodevice.h>
#include <qstringbuilder.h>
#include <qhash.h>

#include <qtextstream.h>

#define HTML_PARSER_TEXT_BLOCK QString("text")

class HtmlTag {
public:
    HtmlTag(QString tag, HtmlTag * parent_tag = 0) : _name(tag), next(0), parent(parent_tag) {}
    ~HtmlTag() { qDeleteAll(tags); delete next; }

    inline QString name() const { return _name; }

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

private:
    QString _name;
    QHash<QString, QString> attrs;
    QList<HtmlTag *> tags;
    HtmlTag * next, * parent;
};

class HtmlParser {
    enum PState : int {
        content,
        tag,
        attr,
        val
    };

public:
    inline HtmlParser(QIODevice * device) : state(content) { parse(device); }
    inline HtmlParser(QString & str) : state(content) {
        QTextStream stream(&str, QIODevice::ReadOnly);
        parse((QIODevice *)&stream);
    }

    ~HtmlParser() { delete root; }

private:
    inline bool isSolo(HtmlTag * tag) { return solo.contains(tag -> name()); }

    inline void initSoloTags() {
        solo.insert("br", true);
        solo.insert("meta", true);
        solo.insert("link", true);
    }

    void parse(QIODevice * device) {
        curr.reserve(1024);

        char * ch = 0;
        elem = root = new HtmlTag("*");

        while(device -> atEnd()) {
            if (device -> getChar(ch)) {
                qDebug() << "MAIN " << (*ch);

                if (*ch == '<') {
                    if (!curr.isEmpty()) {
                        QString text(HTML_PARSER_TEXT_BLOCK);
                        elem -> appendTag(text, curr);
                    }

                    state = tag;
                    parseTag(device);
                } else if (*ch ==  '>') {
                    qDebug() << "MAIN " << (*ch);
                }
                else curr.append(ch);
            }
        }
    }

    void parseTag(QIODevice * device) {
        curr.reserve(64);
        char * ch = 0, * last = 0;

        while(device -> atEnd()) {
            if (device -> getChar(ch)) {
                if (*ch == ' ') {
                    if (state == tag) {
                        state = attr;
                        elem = elem -> appendTag(curr);
                        parseAttr(device);
                        state = attr;
                    } else parseAttr(device);
                } else if (*ch == '>') {
                    state = content;
                    if (*last != '/') elem = elem -> appendTag(curr);
                    else elem -> appendTag(curr);
                    return;
                } else {
                    curr.append(ch);
                    last = ch;
                }
            }
        }
    }

    void parseAttr(QIODevice * device) {
        curr.reserve(256);
        char * ch = 0;

        while(device -> atEnd()) {
            if (device -> getChar(ch)) {
                if (*ch == '=') {
                    state = val;
                    parseValue(device);
                    elem -> addAttr(curr, value);
                    return;
                } else if (*ch == ' ') {
                    return; // skip attrs without value
                } else if (*ch == '>') {
                    state = content;
                    if (isSolo(elem))
                        elem = elem -> parentTag();
                    return;
                } else {
                    curr.append(ch);
                }
            }
        }
    }

    void parseValue(QIODevice * device) {
        value.reserve(512);
        char * ch = 0, * last = 0, * initiator;

        while(device -> atEnd()) {
            if (device -> getChar(ch)) {
                if (*ch == '"') {
                    if (state == val) {
                        initiator = ch;
                        state = content;
                    } else if (state == content) {
                        if (initiator == ch && *last != '\\') {
                           return;
                        } else curr.append(ch);
                    } else {
                        qDebug() << "WRONG STATE";
                        return;
                    }
                } if (*ch == '\'') {
                    if (state == val) {
                        initiator = ch;
                        state = content;
                    } else if (state == content) {
                        if (initiator == ch && *last != '\\') {
                           return;
                        } else curr.append(ch);
                    } else {
                        qDebug() << "WRONG STATE";
                        return;
                    }
                } else  curr.append(ch);
                last = ch;
            }
        }
    }

    HtmlTag * root, * elem;
    QString curr, value;
    PState state;
    QHash<QString, bool> solo;
};

#endif // HTML_PARSER
