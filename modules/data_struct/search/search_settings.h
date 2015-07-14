#ifndef SEARCH_SETTINGS
#define SEARCH_SETTINGS

#include <qstringlist.h>
#include <qstringbuilder.h>

#include "media/genres/music_genres.h"
#include "media/genres/web/vk_genres.h"

enum PredicateType { title, artist, song, tag };

struct SearchSettings {   
    inline SearchSettings(bool vk = false, bool sc = false, bool other = false, bool tabs = false, bool comp = false)
        : inVk(vk), inSc(sc), inOther(other), inTabs(tabs), inComputer(comp), onlyOne(false)
    {
        search_in_own = false;
    }

    void addGenre(QString genreName) {
        int id = MusicGenres::instance() -> toInt(genreName);
        genres.insert(id, genreName);

        if (inVk) {
            int vkId = VkGenres::instance() -> fromStandartId(id);
            vkGenres.insert(vkId, VkGenres::instance() -> toString(vkId));
        }
    }

    bool inVk;
    bool inSc;
    bool inOther;
    bool inTabs;
    bool inComputer;

    bool onlyOne;

    QHash<int, QString> genres;
    QHash<int, QString> vkGenres;

    QStringList drives;
    QList<void *> tabs;
    QStringList predicates;
    PredicateType type;
    bool popular;
    bool search_in_own;
};

struct SearchRequest {
    enum RequestObject { request_vk, request_sc, request_computer, request_tabs, request_other };

    inline SearchRequest(RequestObject tp = request_other, QString predicate = QString(), QString genre = QString(), int genre_id = -1, bool most_popular = true)
        : spredicate(predicate), sgenre(genre), sgenre_id(genre_id), popular(most_popular), search_type(tp) {}

    QString spredicate;
    QString sgenre;
    int sgenre_id;
    bool popular;
    RequestObject search_type;

    static inline QString objToStr(RequestObject obj) {
        switch(obj) {
            case request_vk: return QStringLiteral("VK");
            case request_sc: return QStringLiteral("SC");
            case request_computer: return QStringLiteral("Computer");
            case request_tabs: return QStringLiteral("Tabs");
            default: return QStringLiteral("Other");
        }
    }

    QString token() {
        bool has_predicate = !spredicate.isEmpty();
        bool has_genre = !sgenre.isEmpty();

        if (has_predicate) {
            if (has_genre)
                return spredicate % QStringLiteral(" (") % sgenre % QStringLiteral(") ");
            else
                return spredicate;
        } else if (has_genre) return sgenre;
        else if (popular && search_type != request_computer && search_type != request_tabs) return QStringLiteral("Popular");
        else return QStringLiteral("All");
    }
};

#endif // SEARCH_SETTINGS
