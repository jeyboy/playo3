#ifndef SOUNDCLOUD_SITE_PLAYLIST
#define SOUNDCLOUD_SITE_PLAYLIST

#include "defines.h"
#include "../interfaces/iplaylist.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class Playlist : public QueryBase, public IPlaylist {
                public:
                    QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT) { // next_href
    //                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                        QJsonArray res = pRequest(
                            baseUrlStr(
                                qst_site_alt1, QStringLiteral("playlists/discovery"),
                                {{ QStringLiteral("tag"), tag }}
                            ),
                            call_type_json, rules(0, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET),
                            0, proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                        );

                        return res;
                    }

                    QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                        return pRequest(
                            baseUrlStr(qst_site_alt1, QStringLiteral("tracks/%1/playlists").arg(track_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            COLLECTION_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonArray playlistsByPredicate(const QString & predicate, int count = 10, int offset = 0) {
    //                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                        return pRequest(
                            baseUrlStr(
                                qst_site_alt1, QStringLiteral("search/playlists"),
                                {
                                    { tkn_q, predicate },
                                    { QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base) },
                                    { QStringLiteral("sc_a_id"), generateMark() },
                                    { QStringLiteral("facet"), QStringLiteral("genre") }
                                }
                            ),
                            call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET), 0,
                            proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                        return pRequest(
                            baseUrlStr(qst_site_alt1, QStringLiteral("users/%1/playlists").arg(user_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            COLLECTION_FIELDS, call_method_get, headers()
                        );
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_PLAYLIST
