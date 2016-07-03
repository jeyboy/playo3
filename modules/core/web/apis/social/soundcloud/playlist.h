#ifndef SOUNDCLOUD_PLAYLIST
#define SOUNDCLOUD_PLAYLIST

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SitePlaylist : SiteBase {
            protected:
                QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT) { // next_href
                    QUrlQuery query = genDefaultParams(qst_html_alt1);
                    setParam(query, QStringLiteral("tag"), tag);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                    QJsonArray res = pRequest(
                        baseUrlStr(
                            qst_html_alt1,
                            QStringLiteral("playlists/discovery"),
                            query
                        ),
                        call_type_json, rules(0, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET),
                        0, proc_json_patch,
                        COLLECTION_FIELDS, call_method_get, headers()
                    );

                    return res;
                }

                QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                    return pRequest(
                        baseUrlStr(
                            qst_html_alt1,
                            QStringLiteral("tracks/%1/playlists").arg(track_id),
                            genDefaultParams(qst_html_alt1)
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch,
                        COLLECTION_FIELDS, call_method_get, headers()
                    );
                }

                QJsonArray playlistsByPredicate(const QString & predicate, int count = 10, int offset = 0) {
                    QUrlQuery query = genDefaultParams(qst_html_alt1);
                    setSearchPredicate(query, predicate);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("genre"));

                    return pRequest(
                        baseUrlStr(
                            qst_html_alt1,
                            QStringLiteral("search/playlists"),
                            genDefaultParams(qst_html_alt1)
                        ),
                        call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET), 0,
                        proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                    );
                }
            };

            class ApiPlaylist : public ApiBase {
            protected:
                QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                    QUrlQuery query = genDefaultParams(qst_json_def);

                    QJsonArray res = pRequest(
                        baseUrlStr(
                            qst_json_def,
                            path_user_playlists.arg(user_id),
                            query
                        ),
                        call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, 25), 0, proc_json_patch
                    );

                    return res;
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_PLAYLIST
