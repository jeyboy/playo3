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

                    QJsonValue tracksInfo(const QStringList & track_ids) { //TODO: test me
                        QUrl url(baseUrlStr(qst_html_alt1, path_tracks));
                        QUrlQuery query = genDefaultParams(qst_html_alt1);

                        QJsonArray arr;
                        int limiter = track_ids.size() / SOUNDCLOUD_IDS_PER_REQUEST + (int)(track_ids.size() % SOUNDCLOUD_IDS_PER_REQUEST != 0);
                        for(int step = 0; step < limiter; step++) {
                            setIdsFilter(query, track_ids.mid(step * SOUNDCLOUD_IDS_PER_REQUEST, SOUNDCLOUD_IDS_PER_REQUEST));
                            url.setQuery(query);

                            QJsonObject obj = sRequest(
                                url.toString(), call_type_json, 0, proc_json_wrap,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
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
                                    { QStringLiteral("filter.genre"), tag } // its a genius !!!
                                }
                            ),
                            call_type_json, rules(offset, count), 0,
                            proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                        );

                        return res;
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_TRACK
