#ifndef SOUNDCLOUD_TRACK
#define SOUNDCLOUD_TRACK

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SiteTrack : SiteBase {
            protected:
                QString trackUrl(const QString & track_id) {
                    QUrl url(OFFLINE_RESTPOINT_V1 % QStringLiteral("i1/tracks/%1/streams").arg(track_id));
                    url.setQuery(genDefaultParams(qst_html));

                    QJsonObject obj = request(url);
                    QString res = obj.value(QStringLiteral("http_mp3_128_url")).toString();
                    return res;
                }

                QJsonValue tracksInfo(const QStringList & track_ids) { //TODO: test me
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("tracks"));
                    QUrlQuery query = genDefaultParams(qst_html);

                    QJsonArray arr;
                    int ids_limit = 25;
                    int limiter = track_ids.size() / ids_limit + (int)(track_ids.size() % ids_limit != 0);
                    for(int step = 0; step < limiter; step++) {
                        setParam(query, QStringLiteral("ids"), track_ids.mid(step * ids_limit, ids_limit));
                        url.setQuery(query);

                        QJsonObject obj = request(url);
                        QJsonArray new_items = obj.value(DEF_JSON_FIELD).toArray();
                        QueriableArg::arrAppend(arr, new_items);
                    }

                    return arr;
                }

                QJsonValue trackRelated(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("tracks/%1/related").arg(track_id));
                    QUrlQuery query = genDefaultParams(qst_html);
                    url.setQuery(query);

                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count),
                        0,
                        proc_json_patch
                    );
                }

                QJsonValue tracksSearch(const SearchLimit & limitations) {
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("search/tracks"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    setParam(query, QStringLiteral("q"), limitations.predicate);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("genre"));
                    url.setQuery(query);

                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(limitations.start_offset, limitations.items_limit),
                        0,
                        proc_json_patch
                    );
                }

                QJsonValue tracksByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                    QUrl url(OFFLINE_RESTPOINT_V1 % QStringLiteral("search/sounds"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    setParam(query, QStringLiteral("q"), QStringLiteral("*"));
                    setParam(query, QStringLiteral("filter.genre"), tag); // its a genius !!!
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    url.setQuery(query);

                    QJsonArray res = pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count),
                        0,
                        proc_json_patch,
                        QStringList() << DEF_JSON_FIELD << QStringLiteral("collection");
                    );

                    return res;
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_TRACK
