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
    inline HtmlTag(QString tag, HtmlTag * parent_tag = 0) : _level(parent_tag ? parent_tag -> level() + 1 : 0), _name(tag), parent(parent_tag) {}
    inline ~HtmlTag() { qDeleteAll(tags); }

    inline QString name() const { return _name; }
    inline int level() const { return _level; }
    inline QHash<QString, QString> attributes() const { return attrs; }
    inline HtmlSet children() const { return tags; }

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

    bool validTo(HtmlSelector * selector);

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

    inline ~HtmlParser() { delete root; }

    HtmlSet search(QString predicate);

    inline void output() { qDebug() << (*root); }
private:
    inline bool isSolo(HtmlTag * tag) { return solo.contains(tag -> name()); }

    void initSoloTags();

    void parse(QIODevice * device);
    void parseTag(QIODevice * device);
    void parseAttr(QIODevice * device);
    void parseValue(QIODevice * device);

    QHash<QString, bool> solo;
    HtmlTag * root, * elem;
    QString curr, value;
    PState state;
    char * ch, last, initiator;
};

#endif // HTML_PARSER
