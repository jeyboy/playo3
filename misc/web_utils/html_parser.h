#ifndef HTML_PARSER
#define HTML_PARSER

#include <qiodevice.h>
#include <qstringbuilder.h>
#include <qhash.h>
#include <qbuffer.h>
#include <qpair.h>

#include <qdebug.h>

#define HTML_PARSER_TEXT_BLOCK "text"
#define DEBUG_LIMIT_OUTPUT 100

struct HtmlSelector {
    enum SState { none, tag, attr, id, klass, type };

    enum SToken {
        id_token = 35,
        class_token = 46,
        attr_token = 91,
        attr_separator = 44,
        attr_rel_eq = 61,
        attr_rel_begin = 94,
        attr_rel_end = 38,
        attr_rel_match = 126,
        attr_token_end = 93,
        type_token = 58,
        direct_token = 62,
        space_token = 32,
        cont1_token = 34,
        cont2_token = 39
    };

    HtmlSelector(char * predicate);

    inline HtmlSelector(bool direct = false, HtmlSelector * prev_selector = 0) : _direct(direct), prev(prev_selector), next(0) {
        if (prev_selector) prev_selector -> next = this;
    }
    inline ~HtmlSelector() { delete next; }

    void addToken(SState & tType, QString & token, char & rel);

    QStringList klasses;
    QHash<SState, QString> _tokens;
    QHash<QString, QPair<char, QString> > _attrs;
    bool _direct;

    HtmlSelector * prev;
    HtmlSelector * next;
};

class HtmlTag;
class HtmlSet : public QList<HtmlTag *> {
public:
    inline HtmlSet find(HtmlSelector * selector) {
        HtmlSet set;
        return find(selector, set);
    }

    inline HtmlSet find(QString predicate) {
        HtmlSelector selector(predicate.toUtf8().data());
        return find(&selector);
    }
private:
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
    inline QString value(QString name) { return attrs.value(name); }
    inline QString text() const { return attrs.value(HTML_PARSER_TEXT_BLOCK); }

    inline HtmlTag * parentTag() { return parent; }

    inline HtmlSet find(HtmlSelector * selector) { return tags.find(selector); }
    inline HtmlSet find(QString predicate) {
        HtmlSelector selector(predicate.toUtf8().data());
        return tags.find(&selector);
    }

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
    enum PState { content = 1, tag = 2, attr = 4, val = 8, attr_val = attr | val };

    enum PToken {
        open_tag = 60,
        close_tag_predicate = 47,
        close_tag = 62,
        space = 32,
        attr_rel = 61,
        content_del1 = 34,
        content_del2 = 39,
        mean_sym = 92
    };

public:
    inline HtmlParser(QIODevice * device) { parse(device); }
    inline HtmlParser(QString & str) {
        QByteArray ar = str.toUtf8();
        QBuffer stream(&ar);
        stream.open(QIODevice::ReadOnly);
        parse((QIODevice *)&stream);
        stream.close();
    }

    inline ~HtmlParser() { delete root; }

    inline HtmlSet find(HtmlSelector * selector) {
        return root -> children().find(selector);
    }

    inline HtmlSet find(QString predicate) {
        HtmlSelector selector(predicate.toUtf8().data());
        return find(&selector);
    }

    inline void output() { qDebug() << (*root); }
private:
    inline bool isSolo(HtmlTag * tag) { return solo.contains(tag -> name()); }

    void initSoloTags();

    void parse(QIODevice * device);
    void parseValue(QIODevice * device, QString & value, char * ch, char & initiator, char & last, PState & state);

    QHash<QString, bool> solo;
    HtmlTag * root;
};

#endif // HTML_PARSER
