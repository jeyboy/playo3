#ifndef SEARCH_SETTINGS
#define SEARCH_SETTINGS

#include <qstringlist.h>
#include <qstringbuilder.h>

struct SearchSettings {   
    inline SearchSettings(bool sites = false, bool tabs = false, bool comp = false) : inSites(sites), inTabs(tabs), inComputer(comp), onlyOne(false) { }

    inline void addGenre(QString genreName) { genres << genreName; }
    inline int limit(int def) { return onlyOne ? 1 : def; }

    bool inSites;
    bool inTabs;
    bool inComputer;

    bool onlyOne;

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

    QString token() {
        bool has_predicate = !spredicate.isEmpty();
        bool has_genre = !sgenre.isEmpty();

        if (has_predicate) {
            if (has_genre)
                return spredicate % QStringLiteral(" (") % sgenre % QStringLiteral(") ");
            else
                return spredicate;
        } else if (has_genre) return sgenre;
        else if (popular && search_type == remote) return QStringLiteral("Popular");
        else return QStringLiteral("All");
    }
};

#endif // SEARCH_SETTINGS
