#ifndef SOUNDCLOUD_PLAYLIST
#define SOUNDCLOUD_PLAYLIST

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SitePlaylist : SiteBase {
            protected:
                enum ChartType { new_hot = 1, top_50 };

                QJsonValue chart(const ChartType & chartType, const QString & genre = QStringLiteral("all-music")) { // next_href
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("charts"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    query.addQueryItem(QStringLiteral("genre"), QStringLiteral("soundcloud:genres:") % genre);
                    query.addQueryItem(QStringLiteral("kind"), chartType == new_hot ? QStringLiteral("trending") : QStringLiteral("top"));
                    // linked_partitioning=1
                    url.setQuery(query);

                    QJsonObject obj = request(url);
                    QJsonArray res; // "last_updated":"2016-07-01T05:27:38Z"

                    QJsonArray collection = obj.value(QStringLiteral("collection")).toArray();
                    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++)
                        res.append((*it).toObject().value(QStringLiteral("track")));

                    return res;
                }

                QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT) { // next_href
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("playlists/discovery"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    setParam(query, QStringLiteral("tag"), tag);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    url.setQuery(query);

                    QJsonArray res = pRequest(
                        url.toString(),
                        call_type_json,
                        rules(0, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET),
                        0,
                        proc_json_patch,
                        QStringList() << DEF_JSON_FIELD << QStringLiteral("collection")
                    );

                    return res;
                }

                QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("tracks/%1/playlists").arg(track_id));
                    QUrlQuery query = genDefaultParams(qst_html);
                    url.setQuery(query);

                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count),
                        0,
                        proc_json_patch,
                        QStringList() << DEF_JSON_FIELD << QStringLiteral("collection")
                    );
                }

                QJsonArray playlistsByPredicate(const QString & predicate, int count = 10, int offset = 0) {
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("search/playlists"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    setParam(query, QStringLiteral("q"), predicate);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("genre"));
                    url.setQuery(query);


                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, 10),
                        0,
                        proc_json_patch
                    );
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_PLAYLIST
