#ifndef SEARCH_SETTINGS
#define SEARCH_SETTINGS

#include <qstringlist.h>

enum PredicateType {
    style,
    title,
    artist,
    song,
    tag,
    popular
};

struct SearchSettings {
    SearchSettings() {
    }

    QStringList predicates;
    PredicateType type;
};

#endif // SEARCH_SETTINGS
