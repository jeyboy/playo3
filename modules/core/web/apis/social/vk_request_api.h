#ifndef VK_REQUEST_API
#define VK_REQUEST_API

//https://vk.com/dev/execute

#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/igenreable.h"
#include "vk_api_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class RequestApi : public IQueriable, public IGenreable {
            protected:
            public:


                // https://new.vk.com/dev/newsfeed.getRecommended
                // not finished
                QJsonArray usersFeeds(const QStringList & sources = QStringList()) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, QStringLiteral("filters"), QStringLiteral("audio,video"));
                    setParam(query, QStringLiteral("count"), 100);

                    if (!sources.isEmpty())
                        setParam(query, QStringLiteral("source_ids"), sources);

                    return saRequest(
                        baseUrlStr(qst_api_def, QStringLiteral("newsfeed.get"), query),
                        call_type_json,
                        0,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_items
                    );
                }

                QString videoAlbumsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_audio_albums.arg(uid));
                    return baseUrlStr(qst_api_def, tkn_execute, query).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE);
                }
                QJsonArray videoAlbums(QString & uid, int offset = 0) {
                    return pRequest(
                        videoAlbumsUrl(uid),
                        call_type_json,
                        rules(offset),
                        0,
                        proc_json_extract,
                        QStringList() << tkn_albums
                    );

//                    return lQuery(
//                        audioAlbumsUrl(uid),
//                        QueryRules(tkn_albums, 5, DEFAULT_ITEMS_LIMIT, offset),
//                        extract
//                    );
                }

                QString videoSearchUrl(const SearchLimit & limits) {
                    // count max eq 200 , limit is 1000
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code, QString(
                        "var limit = " % QString::number(qMin(1000, limits.items_limit)) % ";"
                        "var search = []; var rule = true;"
                        "do {"
                        "    var count = limit - search.length;"
                        "    if (count > 200) count = 200;"
                        "    var items = API.video.search({"
                        "        q: \"" % encodeStr(limits.predicate) % "\", count: count, offset: search.length, lyrics: 0,"
                        "        adult: " % boolToStr(true) % ","
                        "        sort: " % QString::number(limits.in_relative() ? vss_relativity : vss_creation_date) % ","
                        "        search_own: " % boolToStr(limits.in_owns()) % ""
                        "    }).items;"
                        "    search = search %2b items;"
                        "    rule = search.length < limit && items.length != 0;"
                        "} while(rule);"
                        "return {" % tkn_video_list % ": search};"
                    ));

                    return baseUrlStr(qst_api_def, tkn_execute, query);
                }

                QJsonArray videoSearch(const SearchLimit & limits, QJsonArray * arr = 0) {
                    return saRequest(
                        videoSearchUrl(limits),
                        call_type_json,
                        arr,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_video_list
                    );
                }

                QJsonArray videoPopular(const SearchLimit & /*limits*/, QJsonArray * /*arr*/ = 0) { // write me
                    return QJsonArray();
//                    return saRequest(
//                        audioPopularUrl(onlyEng, genres.toInt(genre)),
//                        call_type_json,
//                        0,
//                        proc_json_extract,
//                        QStringList() << tkn_response << tkn_audio_list
//                    ); //.value(QStringLiteral("audio_list")).toArray();
                }
            };
        }
    }
}

#endif // VK_REQUEST_API
