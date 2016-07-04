#ifndef SOUNDCLOUD_API_PLAYLIST
#define SOUNDCLOUD_API_PLAYLIST

#include "defines.h"
#include "../interfaces/iplaylist.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Playlist : public QueryBase, public IPlaylist {
                public:
                    QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                        QJsonArray res = pRequest(
                            baseUrlStr(qst_api_def, path_user_playlists.arg(user_id), {}),
                            call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, 25), 0, proc_json_patch
                        );

                        return res;
                    }

                    // predicate is used for search in title - genre - tags - permalinks
                    QJsonValue playlistByPredicate(const QString & predicate, int count = 10, int offset = 0) {
                        return pRequest(
                            baseUrlStr(qst_api_def, path_playlists, {{tkn_q, predicate}}),
                            call_type_json,
                            rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET), // playlists is very weighted for loading - so set limitation to 2 playlists per request
                            0, proc_json_patch
                        );
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_PLAYLIST
