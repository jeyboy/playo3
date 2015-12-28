#ifndef SEARCH_SETTINGS
#define SEARCH_SETTINGS

#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonarray.h>

#define JSON_SEARCH_PREDICATE QStringLiteral("p")
#define JSON_SEARCH_GENRE QStringLiteral("g")
#define JSON_SEARCH_POPULAR QStringLiteral("r")
#define JSON_SEARCH_TYPE QStringLiteral("t")
#define JSON_SEARCH_SUBJECT QStringLiteral("s")

struct SearchSettings {   
    inline SearchSettings(bool sites = false, bool tabs = false, bool comp = false, int predicateLimitation = 999999) : inSites(sites), inTabs(tabs), inComputer(comp), limitPerPredicate(predicateLimitation) { }

    inline int limit(int def) { return qMin(limitPerPredicate, def); }

    bool inSites;
    bool inTabs;
    bool inComputer;

    int limitPerPredicate;

    QList<QString> genres;

    QStringList drives;
    QList<void *> sites;
    QList<void *> tabs;
    QStringList predicates;
    int type;
    bool popular;
};

struct SearchRequest {
    enum RequestType { local, inner, remote };

    inline SearchRequest(RequestType request_type, void * search_interface, QString predicate = QString(), QString genre = QString(), bool most_popular = true)
        : spredicate(predicate), sgenre(genre), popular(most_popular), search_interface(search_interface), search_type(request_type) {}

    QString spredicate;
    QString sgenre;
    bool popular;
    void * search_interface;
    RequestType search_type;

    ~SearchRequest() {
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
