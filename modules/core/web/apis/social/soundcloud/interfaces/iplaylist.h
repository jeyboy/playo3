#ifndef SOUNDCLOUD_IPLAYLIST
#define SOUNDCLOUD_IPLAYLIST

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class IPlaylist {
            protected:
                QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                // predicate is used for search in title - genre - tags - permalinks
                QJsonValue playlistsByPredicate(const QString & predicate, int count = 10, int offset = 0) = 0;
                QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
            };
        }
    }
}

#endif // SOUNDCLOUD_IPLAYLIST
