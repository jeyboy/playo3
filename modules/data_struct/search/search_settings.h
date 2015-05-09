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
    SearchSettings() {
    }

    QStringList predicates;
    PredicateType type;
    bool popular;
};

#endif // SEARCH_SETTINGS
