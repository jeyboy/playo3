#ifndef SOUNDCLOUD_TRACK
#define SOUNDCLOUD_TRACK

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class ApiTrack : ApiBase {
            protected:

            };

            class SiteTrack : SiteBase {
            protected:
                QString trackUrl(const QString & track_id) {
                    QJsonObject obj = sRequest(
                        baseUrlStr(
                            qst_html_def,
                            QStringLiteral("i1/tracks/%1/streams").arg(track_id),
                            genDefaultParams(qst_html_def)
                        ),
                        call_type_json, 0, proc,
                        IQUERY_DEF_FIELDS, call_method_get,
                        headers()
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
                        baseUrlStr(
                            qst_html_alt1,
                            path_related_tracks.arg(track_id),
                            genDefaultParams(qst_html_alt1)
                        ),
                        call_type_json, rules(offset, count), 0,
                        proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                    );
                }

                QJsonValue tracksSearch(const SearchLimit & limitations) {
                    QUrlQuery query = genDefaultParams(qst_html_alt1);
                    setSearchPredicate(query, limitations.predicate);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("genre"));

                    return pRequest(
                        baseUrlStr(
                            qst_html_alt1,
                            QStringLiteral("search/tracks"),
                            query
                        ),
                        call_type_json, rules(limitations.start_offset, limitations.items_limit), 0,
                        proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                    );
                }

                QJsonValue tracksByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                    QUrlQuery query = genDefaultParams(qst_html_def);
                    setSearchPredicate(query, QStringLiteral("*"));
                    setParam(query, QStringLiteral("filter.genre"), tag); // its a genius !!!
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                    QJsonArray res = pRequest(
                        baseUrlStr(
                            qst_html_def,
                            QStringLiteral("search/sounds"),
                            query
                        ),
                        call_type_json, rules(offset, count), 0,
                        proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                    );

                    return res;
                }
            };

            class ApiTrack : public ApiBase {
                QString trackSearchUrl(const QString & predicate, const QString & genre, bool hottest = false) {
                    QUrlQuery query = genDefaultParams(qst_json_def);
                    setAudioTypesParam(query);
                    setOrder(query, hottest);

                    if (!genre.isEmpty())
                        setGenreLimitation(query, genre);

                    if (!predicate.isEmpty())
                        setSearchPredicate(query, predicate);

                    return baseUrlStr(qst_json_def, path_tracks, query);
                }

            protected:
                QJsonValue trackRelations(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                    return pRequest(
                        baseUrlStr(
                            qst_json_def,
                            path_related_tracks.arg(track_id),
                            genDefaultParams(qst_json_def)
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch
                    );
                }

                QJsonValue tracksSearch(const SearchLimit & limitations) {
                    return pRequest(
                        audioSearchUrl(limitations.predicate, limitations.genre, limitations.by_popularity()),
                        call_type_json,
                        rules(limitations.start_offset, limitations.items_limit), 0, proc_json_patch
                    );
                }

                QJsonObject trackInfo(const QString & track_id) {
                    return sRequest(
                        baseUrlStr(
                            qst_json_def,
                            path_tracks % '/' % track_id,
                            genDefaultParams(qst_json_def)
                        ),
                        call_type_json
                    );
                }


                //"id": 142370360,
                //"permalink": "sam-smith-stay-with-me",
                QJsonValue tracksInfo(const QStringList & track_ids) { //TODO: test me
                    QUrl url(baseUrlStr(qst_json_def, path_tracks));
                    QUrlQuery query = genDefaultParams(qst_json_def);

                    QJsonArray arr;
                    int limiter = track_ids.size() / SOUNDCLOUD_IDS_PER_REQUEST + (int)(track_ids.size() % SOUNDCLOUD_IDS_PER_REQUEST != 0);
                    for(int step = 0; step < limiter; step++) {
                        setIdsFilter(query, track_ids.mid(step * SOUNDCLOUD_IDS_PER_REQUEST, SOUNDCLOUD_IDS_PER_REQUEST));
                        url.setQuery(query);

                        QJsonObject obj = sRequest(url.toString(), call_type_json, 0, proc_json_wrap);

                        QJsonArray new_items = obj.value(DEF_JSON_FIELD).toArray();
                        QueriableArg::arrAppend(arr, new_items);
                    }

                    return arr;
                }

                QJsonValue tracksByGroup(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                    QUrlQuery query = genDefaultParams(qst_json_def);
                    setAudioTypesParam(query);

                    QJsonArray res = pRequest(
                        baseUrlStr(
                            qst_json_def,
                            path_group_tracks.arg(group_id),
                            query
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch
                    );

                    return res;
                }

                QJsonValue tracksByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                    QUrlQuery query = genDefaultParams(qst_json_def);

                    QJsonArray res = pRequest(
                        baseUrlStr(
                            qst_json_def,
                            path_user_tracks.arg(user_id),
                            query
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch
                    );

                    return res;
                }

                QJsonValue tracksByUserLikes(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                    QUrlQuery query = genDefaultParams(qst_json_def);

                    QJsonArray res = pRequest(
                        baseUrlStr(
                            qst_json_def,
                            path_user_favorites.arg(user_id),
                            query
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch
                    );

                    return res;
                }

                void likeTrack(const QString & user_id, const QString & track_id) {
                    QString res = Manager::prepare() -> putFollowed(
                        baseUrlStr(
                            qst_json_def,
                            path_user_favorites.arg(user_id) % '/' % track_id,
                            genDefaultParams(qst_json_def)
                        ),
                        Headers()
                    ) -> toText();
                    qDebug() << "FAV" << res;
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_TRACK
