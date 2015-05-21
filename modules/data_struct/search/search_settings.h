#ifndef SEARCH_SETTINGS
#define SEARCH_SETTINGS

#include <qstringlist.h>
#include "media/genres/music_genres.h"
#include "media/genres/web/vk_genres.h"

enum PredicateType {
    style,
    title,
    artist,
    song,
    tag
};

struct SearchSettings {   
    inline SearchSettings(bool vk = false, bool sc = false, bool other = false, bool tabs = false, bool comp = false)
        : inVk(vk), inSc(sc), inOther(other), inTabs(tabs), inComputer(comp)
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

    QHash<int, QString> genres;
    QHash<int, QString> vkGenres;

    QStringList drives;
    QList<void *> tabs;
    QStringList predicates;
    PredicateType type;
    bool popular;
    bool search_in_own;

    QString activePredicate; //remove later
};

struct SearchRequest {
    enum ReuestObject {
        request_vk,
        request_sc,
        request_computer,
        request_tabs,
        request_other
    };

    SearchRequest(ReuestObject tp = request_other, QString predicate = QString(), QString genre = QString(), int genre_id = -1, bool most_popular = true)
        : spredicate(predicate), sgenre(genre), sgenre_id(genre_id), popular(most_popular), search_type(tp) {}

    QString spredicate;
    QString sgenre;
    int sgenre_id;
    bool popular;
    ReuestObject search_type;
};

#endif // SEARCH_SETTINGS
