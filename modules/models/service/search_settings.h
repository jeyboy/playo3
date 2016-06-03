#ifndef SEARCH_SETTINGS
#define SEARCH_SETTINGS

#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonarray.h>
#include <qjsonobject.h>

#define JSON_SEARCH_PREDICATE QStringLiteral("p")
#define JSON_SEARCH_GENRE QStringLiteral("g")
#define JSON_SEARCH_TYPE QStringLiteral("t")
#define JSON_SEARCH_SUBJECT QStringLiteral("s")
#define JSON_SEARCH_LIMIT QStringLiteral("l")
#define JSON_SEARCH_CONTENT_LIMIT QStringLiteral("c")

struct SearchSettings {   
    inline SearchSettings(bool sites = false, bool tabs = false, bool comp = false, int predicateLimitation = 999999) : inSites(sites), inTabs(tabs), inComputer(comp), limitPerPredicate(predicateLimitation) { }

    inline int limit(int def) { return qMin(limitPerPredicate, def); }

    QJsonObject toJson() {
        QJsonObject set;
        set.insert(JSON_SEARCH_TYPE, stype);
        set.insert(JSON_SEARCH_LIMIT, limitPerPredicate);
        set.insert(JSON_SEARCH_CONTENT_LIMIT, ctype);
        return set;
    }

    void fromJson(const QJsonObject & obj) {
        stype = obj.value(JSON_SEARCH_TYPE).toInt();
        ctype = obj.value(JSON_SEARCH_CONTENT_LIMIT).toInt();
        limitPerPredicate = obj.value(JSON_SEARCH_LIMIT).toInt();
    }

    bool inSites;
    bool inTabs;
    bool inComputer;

    int limitPerPredicate;

    QList<QString> genres;
    QStringList drives;
    QList<void *> sites;
    QList<void *> tabs;
    QStringList predicates;

    int stype;
    int ctype;
};

struct SearchRequest {
    enum RequestType { local, inner, remote };

    inline SearchRequest(RequestType request_type, void * search_interface, int source_type, int content_type, QString predicate = QString(), QString genre = QString())
        : spredicate(predicate), sgenre(genre), search_interface(search_interface), search_type(request_type),
          content_type(content_type), source_type(source_type) {}

    QString spredicate;
    QString sgenre;
    void * search_interface;
    RequestType search_type;
    int content_type;
    int source_type;

    ~SearchRequest() {}

    void clearInterface() {
        switch(search_type) {
            case local: { delete (QString *)search_interface; break;}
            default:;
        }
    }

    QString token();
    static void fromJson(const QJsonArray & objs, QList<SearchRequest> & list);
    void save(QJsonArray & arr);
};

#endif // SEARCH_SETTINGS
