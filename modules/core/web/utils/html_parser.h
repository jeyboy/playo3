#ifndef HTML_PARSER
#define HTML_PARSER

//#include <qapplication.h>
#include <qstringbuilder.h>
#include <qhash.h>
#include <qbuffer.h>
#include <qpair.h>
#include <qurl.h>
#include <qurlquery.h>

#include "unicode_decoding.h"
#include "html_parser_keys.h"

#define DEBUG_LIMIT_OUTPUT 10000

namespace Core {
    namespace Web {
        namespace Html {
            struct Selector {
                enum SToken {
                    id_token = 35,
                    class_token = 46,
                    attr_token = 91,
                    attr_separator = 44,
                    attr_rel_eq = 61, // =
                    attr_rel_begin = 94, // ^
                    attr_rel_end = 38, // &
                    attr_rel_not = 33, // !
                    attr_rel_match = 126, // ~
                    attr_token_end = 93,
                    type_token = 58,
                    direct_token = 62,
                    back_direct_token = 60,
                    space_token = 32,
                    cont1_token = 34,
                    cont2_token = 39
                };
                enum SType { direct = direct_token, forward, backward = back_direct_token };
                enum SState { none = attr_token_end, tag, attr, id = id_token, klass = class_token, type = type_token };

                Selector(const char * predicate);

                inline Selector(SType selector_type = forward, Selector * prev_selector = 0) : sType(selector_type), prev(prev_selector), next(0) {
                    if (prev_selector) prev_selector -> next = this;
                }
                inline ~Selector() { delete next; }
                Selector operator= (const char * x) { return Selector(x); }

                void addToken(SState & tType, QString & token, char & rel);

                inline bool isDirect() const { return sType == direct; }
                inline bool isBackward() const { return sType == backward; }
//                bool validTo(int index) const;
//                bool skipable(int index) const;

                QStringList klasses;
                QHash<SState, QString> _tokens;
                QHash<QString, QPair<char, QString> > _attrs;
                SType sType;

                Selector * prev;
                Selector * next;
            };

            class Tag;

            class Set : public QList<Tag *> {
            public:
                QString link();
                QString text();
                QString value(const QString & name = attr_default);

                inline Set find(const Selector * selector, bool findFirst = false) const {
                    Set set;
                    return find(selector, set, findFirst);
                }
                inline Set find(const char * predicate, bool findFirst = false) const {
                    Selector selector(predicate);
                    return find(&selector, findFirst);
                }
                inline Tag * findFirst(const char * predicate) const {
                    Set set = find(predicate, true);
                    return set.isEmpty() ? 0 : set.first();
                }
                inline Tag * findFirst(const Selector * selector) const {
                    Set set = find(selector, true);
                    return set.isEmpty() ? 0 : set.first();
                }

                QHash<QString, QString> & findLinks(const Selector * selector, QHash<QString, QString> & links);
                inline Set & operator <<(const Set & l) { *this += l; return *this; }
            private:
                Set & find(const Selector * selector, Set & set, bool findFirst = false) const;
                friend class Tag;
            };

            class Tag {
            public:
                inline Tag(QString tag, Tag * parent_tag = 0) : _level(parent_tag ? parent_tag -> level() + 1 : 0), _name(tag), parent(parent_tag) {}
                inline ~Tag() { qDeleteAll(tags); }

                inline QString name() const { return _name; }
                inline int level() const { return _level; }
                inline QHash<QString, QString> attributes() const { return attrs; }
                inline Set children() const { return tags; }
                inline QString value(const QString & name = attr_default) {
                    if (name != attr_default || (name == attr_default && _name != tag_select))
                        return attrs.value(name);
                    else {
                        Html::Set options = find("option[selected]");
                        return options.value();
                    }
                }
                inline QString text() const {
                    const Tag * text = (_name == tkn_text_block ? this : childTag(tkn_text_block));
                    return text ? text -> attrs.value(tkn_text_block) : QString();
                }

                QUrl serializeFormToUrl(const QHash<QString, QString> & vals = QHash<QString, QString>(), bool appendable = false);
                QString toText() const;

                inline QString link() const { return attrs.value(attr_href); }

                inline bool is_link() { return _name == tag_a; }
                inline bool is_script() { return _name == tag_script; }
                inline bool is_head() { return _name == tag_head; }
                inline bool is_meta() { return _name == tag_meta; }
                inline bool is_xml_head() { return _name == tag_xml; }

                inline Tag * parentTag() { return parent; }
                inline Tag * childTag(int pos) const { return tags[pos]; }
                inline Tag * childTag(const QString & name_predicate, int pos = 0) const {
                    Set::ConstIterator tag = tags.cbegin();
                    for(int i = 0; tag != tags.cend(); tag++) {
                        if ((*tag) -> name() == name_predicate)
                            if (i++ == pos) return (*tag);
                    }

                    return 0;
                }
                inline int childrenCount() { return tags.size(); }

                inline bool has(const char * predicate) const { return !find(predicate).isEmpty(); }
                inline Set find(const Selector * selector) const { return tags.find(selector); }
                inline Set find(const char * predicate) const {
                    Selector selector(predicate);
                    return tags.find(&selector);
                }
                inline Tag * findFirst(const char * predicate) const {
                    Selector selector(predicate);
                    return findFirst(&selector);
                }
                inline Tag * findFirst(const Selector * selector) const {
                    Set set = tags.find(selector, true);
                    return set.isEmpty() ? 0 : set.first();
                }

                inline QHash<QString, QString> & findLinks(const Selector * selector, QHash<QString, QString> & links) {
                    return tags.findLinks(selector, links);
                }

                inline void addAttr(QString & name, QString & val) { attrs.insert(name, val);  name.clear(); val.clear(); }
                inline Tag * appendTag(QString & tname) {
                    Tag * newTag = new Tag(tname, this); tname.clear();
                    tags.append(newTag);
                    return newTag;
                }
                inline void appendText(QString & val) {
                    QString tnm(tkn_text_block);
                    Tag * newTag = appendTag(tnm);
                    QString nm(tkn_text_block);
                    newTag -> addAttr(nm, val); val.clear();
                }
                inline void appendComment(QString & val) {
                    QString tnm(tkn_comment_block);
                    Tag * newTag = appendTag(tnm);
                    QString nm(tkn_comment_block);
                    val = val.mid(1, val.length() - 3);
                    newTag -> addAttr(nm, val); val.clear();
                }

                inline bool hasClass(const QString & class_name) {
                    return attrs[attr_class].split(tkn_split, QString::SkipEmptyParts).contains(class_name);
                }

                bool validTo(const Selector * selector);
                Set & backwardFind(Selector * selector, Set & set);
                QHash<QString, QString> & backwardFindLinks(Selector * selector, QHash<QString, QString> & links);

                friend QDebug operator<< (QDebug debug, const Tag & c) {
                    QString attrStr;
                    QHash<QString, QString> vals = c.attributes();

                    for (QHash<QString, QString>::iterator it = vals.begin(); it != vals.end(); ++it)
                        attrStr.append("(" + it.key() + " : " + (it.value().size() > DEBUG_LIMIT_OUTPUT ? (it.value().mid(0, DEBUG_LIMIT_OUTPUT / 2) % "..." % it.value().mid(it.value().size() - DEBUG_LIMIT_OUTPUT / 2, DEBUG_LIMIT_OUTPUT / 2)) : it.value()) + ")");

                    if (attrStr.isEmpty())
                        qDebug("%s%s", QString(c.level() * 3, ' ').toUtf8().constData(), c.name().toUtf8().constData());
                    else
                        qDebug("%s%s%s%s%s", QString(c.level() * 3, ' ').toUtf8().constData(), c.name().toUtf8().constData(), " ||| [", attrStr.toUtf8().constData(), "]");

                    foreach(Tag * it, c.children())
                        qDebug() << (*it);

                    return debug;
                }

            private:
                int _level;
                QString _name;
                QHash<QString, QString> attrs;
                Set tags;
                Tag * parent;
            };

            class Document : public UnicodeDecoding {
                enum Flags { none = 0, skip_text, skip_comment };

                enum PState {
                    content = 1, tag = 2, attr = 4, in_attr = 8, val = 16, in_val = 32, comment = 64, attr_val = attr | val
                };

                enum PToken {
                    open_tag = 60, // <
                    close_tag_predicate = 47, // /
                    close_tag = 62, // >
                    space = 32,
                    comment_token = 33, // !
                    comment_post_token = 45, // -
                    attr_rel = 61, // =
                    content_del1 = 34, // "
                    content_del2 = 39, // '
                    mean_sym = 92, // \\
                    code_start = 38, // &
                    code_unicode = 35, // #
                    code_end = 59 // ;
                };

            public:
                inline Document(QIODevice * device, const CharsetType & doc_charset = charset_utf8, const Flags & parse_flags = skip_comment) : flags(parse_flags), charset(doc_charset), charset_finded(false), using_default_charset(false) { parse(device); }
                inline Document(const QString & str, const CharsetType & doc_charset = charset_utf8, const Flags & parse_flags = skip_comment) : flags(parse_flags), charset(doc_charset), charset_finded(false), using_default_charset(false) {
                    QByteArray ar = str.toUtf8();
                    QBuffer stream(&ar);
                    stream.open(QIODevice::ReadOnly);
                    parse((QIODevice *)&stream);
                    stream.close();
                }

                inline ~Document() { delete root; }

                inline bool isXml() {
                    QString name = root -> children().first() -> name();
                    return name.contains(tag_xml, Qt::CaseInsensitive);
                }
                inline bool has(const char * predicate) const { return root -> has(predicate); }
                inline Set find(const Selector * selector, bool findFirst = false) const { return root -> children().find(selector, findFirst); }
                inline Set find(const char * predicate) const {
                    Selector selector(predicate);
                    return find(&selector);
                }
                inline Tag * findFirst(const char * predicate) const {
                    Selector selector(predicate);
                    Set set = find(&selector, true);
                    return set.isEmpty() ? 0 : set.first();
                }

//                inline void dump() {
//                    QString p = QCoreApplication::applicationDirPath() % '/' % QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz") % QStringLiteral(".html");
//                    QFile f(p);
//                    if (f.open(QFile::WriteOnly)) {
//                        QString;


//                        f.write(readAll());
//                        f.close();
//                    }
//                }

                inline void output() { qDebug() << (*root); }
            private:
                inline bool isSolo(Tag * tag) { return solo.contains(tag -> name()); }
                inline bool isSolo(const QString & tag_name) { return solo.contains(tag_name); }

                void parse(QIODevice * device);
                inline QString parseCode(QIODevice * device, char * ch) {
                    QString code;
                    bool is_unicode = false;
                    while(true) {
                        if (device -> getChar(ch)) {
                            switch(*ch) {
                                case code_unicode: { is_unicode = true; break;}
                                case code_end:
                                    return is_unicode ? QChar(code.toInt()) : html_entities.value(code);
                                default:
                                    if ((*ch > 47 && *ch < 58) || (*ch > 96 && *ch < 123))
                                        code.append(*ch);
                                    else { device -> ungetChar(*ch); return code.prepend('&'); }
                            }
                        } else return code.prepend('&');
                    }
                }

                inline void checkCharset(Tag * tag) {
                    if (tag -> is_meta() || tag -> is_xml_head())
                        proceedCharset(tag);
                    else if (tag -> is_head()) {
                        using_default_charset = true;
                        charset = charset_utf8;
                    }
                }

                inline void proceedCharset(Tag * tag) {
                    if (tag -> is_xml_head()) {
                        QString xml_encoding = tag -> value(tkn_encoding);
                        qDebug() << xml_encoding;
                        if (!xml_encoding.isEmpty()) {
                            charset = toCharsetType(xml_encoding.toLower());
                            charset_finded = true;
                        }
                    } else {
                        QString meta = tag -> value(tkn_charset);
                        if (meta.isEmpty()) {
                            if (tag -> value(tkn_http_equiv).toLower() == tkn_content_type) {
                                meta = tag -> value(tkn_content);
                                meta = meta.section(tkn_charset_attr, 1).section(' ', 0);
                            }
                        }

                        if (!meta.isEmpty()) {
                            qDebug() << meta;
                            charset = toCharsetType(meta.toLower());
                            charset_finded = true;
                        }
                    }
                }

                Tag * root;
                Flags flags;
                CharsetType charset;
                bool charset_finded, using_default_charset;

                static QHash<QString, bool> solo;
                static QHash<QString, QChar> html_entities;
            };
        }
    }
}

#endif // HTML_PARSER
