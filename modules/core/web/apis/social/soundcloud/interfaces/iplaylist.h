#ifndef SOUNDCLOUD_IPLAYLIST
#define SOUNDCLOUD_IPLAYLIST

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class IPlaylist {
            protected:
                virtual QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                virtual QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                // predicate is used for search in title - genre - tags - permalinks
                virtual QJsonValue playlistsByPredicate(const QString & predicate, int count = 10, int offset = 0) = 0;
                virtual QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
            };
        }
    }
}

#endif // SOUNDCLOUD_IPLAYLIST
