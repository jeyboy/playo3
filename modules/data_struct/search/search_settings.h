#ifndef SEARCH_SETTINGS
#define SEARCH_SETTINGS

#include <qstringlist.h>

enum PredicateType {
    style,
    title,
    artist,
    song,
    tag
};

struct SearchSettings {
    inline SearchSettings(bool vk, bool sc, bool other, bool tabs, bool comp)
        : inVk(vk), inSc(sc), inOther(other), inTabs(tabs), inComputer(comp)
    {
        search_in_own = false;
    }

    bool inVk;
    bool inSc;
    bool inOther;
    bool inTabs;
    bool inComputer;

    QList<void *> tabs;
    QStringList predicates;
    PredicateType type;
    bool popular;
    bool search_in_own;
};

#endif // SEARCH_SETTINGS
