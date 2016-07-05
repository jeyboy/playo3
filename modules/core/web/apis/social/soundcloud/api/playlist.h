#ifndef SOUNDCLOUD_API_PLAYLIST
#define SOUNDCLOUD_API_PLAYLIST

#include "defines.h"
#include "../interfaces/iplaylist.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Playlist : public QueryBase, public IPlaylist { // playlists is very weighted for loading
                public:
                    QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0);

                    //TODO: need to test
                    QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(qst_api_def, QStringLiteral("tracks/%1/playlists").arg(track_id), {}),
                            call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET),
                            0, proc_json_patch
                        );
                    }

                    // predicate is used for search in title - genre - tags - permalinks
                    QJsonValue playlistByPredicate(const QString & predicate, int count = 10, int offset = 0) {
                        return pRequest(
                            baseUrlStr(qst_api_def, path_playlists, {{tkn_q, predicate}}),
                            call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET),
                            0, proc_json_patch
                        );
                    }

                    QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                        QJsonArray res = pRequest(
                            baseUrlStr(qst_api_def, path_user_playlists.arg(user_id), {}),
                            call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, 25), 0, proc_json_patch
                        );

                        return res;
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_PLAYLIST
