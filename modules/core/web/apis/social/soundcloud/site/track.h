#ifndef SOUNDCLOUD_SITE_TRACK
#define SOUNDCLOUD_SITE_TRACK

#include "defines.h"
#include "../interfaces/itrack.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class Track : public QueryBase, public ITrack {
                public:
                    QString trackUrl(const QString & track_id) {
                        QJsonObject obj = sRequest(
                            baseUrlStr(qst_site_def, QStringLiteral("i1/tracks/%1/streams").arg(track_id), {}),
                            call_type_json, 0, proc, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );

                        QString res = obj.value(QStringLiteral("http_mp3_128_url")).toString();
                        return res;
                    }

                    bool likeTrack(const QString & user_id, const QString & track_id) {
                        // alt // https://api.soundcloud.com/e1/me/track_likes/265476609
                        QJsonObject obj = Manager::prepare() -> putFollowed(
                            baseUrlStr(qst_site_def, path_user_favorites.arg(user_id) % '/' % track_id, {}),
                            headers()
                        ) -> toJson();

                        return obj.value(QStringLiteral("status")).toString().startsWith(QStringLiteral("201"));
                    }

                    QJsonObject trackInfo(const QString & track_id) { //TODO: test me
                        QJsonObject obj = sRequest(
                            baseUrlStr(qst_site_alt1, path_tracks, {{ tkn_ids, track_id }}),
                            call_type_json, 0, proc, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );

                        return obj;
                    }

                    QJsonValue tracksInfo(const QStringList & track_ids) { //TODO: test me
                        QJsonArray arr;
                        int limiter = track_ids.size() / SOUNDCLOUD_IDS_PER_REQUEST + (int)(track_ids.size() % SOUNDCLOUD_IDS_PER_REQUEST != 0);
                        for(int step = 0; step < limiter; step++) {
                            QJsonObject obj = sRequest(
                                baseUrlStr(qst_site_alt1, path_tracks, {{ tkn_ids, track_ids.mid(step * SOUNDCLOUD_IDS_PER_REQUEST, SOUNDCLOUD_IDS_PER_REQUEST) }}),
                                call_type_json, 0, proc_json_wrap, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );

                            QJsonArray new_items = obj.value(DEF_JSON_FIELD).toArray();
                            QueriableArg::arrAppend(arr, new_items);
                        }

                        return arr;
                    }

                    QJsonValue trackRelations(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                        return pRequest(
                            baseUrlStr(qst_site_alt1, path_related_tracks.arg(track_id), {}),
                            call_type_json, rules(offset, count), 0,
                            proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue tracksSearch(const SearchLimit & limitations) {
    //                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                        return pRequest(
                            baseUrlStr(
                                qst_site_alt1,
                                QStringLiteral("search/tracks"),
                                {
                                    { tkn_q, limitations.predicate },
                                    { QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base) },
                                    { QStringLiteral("sc_a_id"), generateMark() },
                                    { QStringLiteral("facet"), QStringLiteral("genre") }
                                }
                            ),
                            call_type_json, rules(limitations.start_offset, limitations.items_limit), 0,
                            proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue tracksByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
//                        setParam(query, QStringLiteral("linked_partitioning"), 1);

                        QJsonArray res = pRequest(
                            baseUrlStr(
                                qst_site_def, QStringLiteral("search/sounds"),
                                {
                                    { tkn_q, QStringLiteral("*") },
                                    { QStringLiteral("filter.genre"), tag } // its a genius !!! // try to use a comma separated list of tags
                                }
                            ),
                            call_type_json, rules(offset, count), 0,
                            proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                        );

                        return res;
                    }

                    QJsonValue tracksByGroup(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(
                                qst_site_def, QStringLiteral("groups/%1/tracks").arg(group_id), {}),
                            call_type_json, rules(offset, count), 0,
                            proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue tracksByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        // linked_partitioning=1

                        return pRequest(
                            baseUrlStr(
                                qst_site_alt1, QStringLiteral("users/%1/tracks").arg(user_id),
                                {{ QStringLiteral("representation"), QString() }}
                            ),
                            call_type_json, rules(offset, count), 0,
                            proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue tracksByUserLikes(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(qst_site_alt1, QStringLiteral("users/%1/likes").arg(user_id), {}),
                            call_type_json, rules(offset, count), 0,
                            proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_TRACK
