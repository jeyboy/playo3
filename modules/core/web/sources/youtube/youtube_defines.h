#ifndef YOUTUBE_DEFINES_H
#define YOUTUBE_DEFINES_H

#include "youtube_misc.h"

//#include "modules/core/web/interfaces/iqueriable.h"
//#include "modules/core/interfaces/search_limits.h"
//#include "modules/core/misc/file_utils/extensions.h"

//#include "modules/core/media/genres/music_genres.h"

#define YOUTUBE_INFO_ITEMS_LIMIT 50
#define YOUTUBE_ITEMS_LIMIT 100
#define YOUTUBE_PAGES_LIMIT 10

namespace Core {
    namespace Web {
        namespace Youtube {
            class Base : public virtual Misc {
            protected:
//                videoDuration 	string
//                The videoDuration parameter filters video search results based on their duration. If you specify a value for this parameter, you must also set the type parameter's value to video.

//                Acceptable values are:

//                    any – Do not filter video search results based on their duration. This is the default value.
//                    long – Only include videos longer than 20 minutes.
//                    medium – Only include videos that are between four and 20 minutes long (inclusive).
//                    short – Only include videos that are less than four minutes long.

//                //encodeStr
//                QString searchUrl(const QString & predicate, const QString & /*genre*/, bool hottest = false, const QString & relatedVideoId = QString()) {
//                    QUrlQuery query = genDefaultParams();
//                    setOrder(query, hottest ? LSTR("rating") : LSTR("relevance"));
//                    setEmbedable(query);
//                    setParam(query, tkn_part, LSTR("snippet"));
//                    setParam(query, LSTR("fields"), LSTR("items(id,snippet),nextPageToken,pageInfo"));
//                    setParam(query, LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT); // 50
////                    setParam(query, LSTR("safeSearch"), LSTR("none"));
//                    setMusicVideoCategory(query);

//                    if (!predicate.isEmpty())
//                        setSearchPredicate(query, predicate);
//                    else if (!relatedVideoId.isEmpty())
//                        setParam(query, LSTR("relatedToVideoId"), relatedVideoId);

//                    return baseUrlStr(qst_api, path_search, query);
//                }

                QString videosUrl(const QStringList & ids = QStringList()) {
                    QUrlQuery query = genDefaultParams();
                    if (!ids.isEmpty())
                        setParam(query, LSTR("id"), ids.join(','));
                    else
                        setParam(query, LSTR("chart"), LSTR("mostPopular"));

                    setParam(query, tkn_part, LSTR("snippet,contentDetails"));
                    setParam(query, LSTR("fields"), LSTR("items(contentDetails,fileDetails,id,localizations,player,snippet),nextPageToken,pageInfo"));
                    setParam(query, LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT); // 50
                    setParam(query, LSTR("regionCode"), siteLocale(LSTR("ua")));
                    setMusicVideoCategory(query);

                    return baseUrlStr(qst_api, LSTR("videos"), query);
                }




//                enum CategoryTypes {
//                    music = 1, video = 2, photo = 3, archive = 4, book = 5,
//                    program = 6, web = 7, mobile = 8, android = 10
//                };

//                inline Headers siteHeaders() {
//                    return Headers({{LSTR("x-security"), Manager::cookie(LSTR("Login"), url_html_site_base)}});
//                }


//                Your request can also use the Boolean NOT (-) and OR (|) operators to exclude videos or to find videos that are associated with one of several
//                search terms. For example, to search for videos matching either "boating" or "sailing", set the q parameter value to boating|sailing.
//                Similarly, to search for videos matching either "boating" or "sailing" but not "fishing", set the q parameter value to boating|sailing -fishing.
//                Note that the pipe character must be URL-escaped when it is sent in your API request. The URL-escaped value for the pipe character is %7C.
                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_q, predicate); }

//                date – Resources are sorted in reverse chronological order based on the date they were created.
//                rating – Resources are sorted from highest to lowest rating.
//                relevance – Resources are sorted based on their relevance to the search query. This is the default value for this parameter.
//                title – Resources are sorted alphabetically by title.
//                videoCount – Channels are sorted in descending order of their number of uploaded videos.
//                viewCount – Resources are sorted from highest to lowest number of views. For live broadcasts, videos are sorted by number of concurrent viewers while the broadcasts are ongoing.
                inline void setOrder(QUrlQuery & query, const QString & order = LSTR("relevance")) {
                    setParam(query, tkn_order, order);
                }

//            {
//             "kind": "youtube#videoCategory",
//             "etag": "\"DsOZ7qVJA4mxdTxZeNzis6uE6ck/nqRIq97-xe5XRZTxbknKFVe5Lmg\"",
//             "id": "10",
//             "snippet": {
//              "channelId": "UCBR8-60-B28hp2BmDPdntcQ",
//              "title": "Music",
//              "assignable": true
//             }
//            },
                void setMusicVideoCategory(QUrlQuery & query) {
                    setParam(query, LSTR("videoCategoryId"), 10);
                }

                inline void setEmbedable(QUrlQuery & query) {
                    setParam(query, tkn_video_embedable, LSTR("true")); // any // true
                    setParam(query, tkn_type, LSTR("video")); // channel // playlist // video
                }


                void proceedDurationResult(QStringList & ids, QJsonArray & arr) {
                    pRequest(
                        videosUrl(ids),
                        call_type_json,
                        rules(QString(), YOUTUBE_INFO_ITEMS_LIMIT),
                        &arr
                    );

//                    lQuery(
//                        videosUrl(ids),
//                        queryRules(50),
//                        res
//                    );
                    ids.clear();
                }

                void initDuration(QJsonArray & arr) {
                    QStringList ids;
                    QJsonArray res;

                    for(QJsonArray::Iterator item = arr.begin(); item != arr.end(); item++)
                        if (ids.length() == YOUTUBE_INFO_ITEMS_LIMIT)
                            proceedDurationResult(ids, res);
                        else
                            ids << (*item).toObject().value(tkn_id).toObject().value(LSTR("videoId")).toString();

                    if (!ids.isEmpty())
                        proceedDurationResult(ids, res);

                    arr = res;
                }

                PolyQueryRules rules(
                    QString start_token = QString(), int items_limit = YOUTUBE_ITEMS_LIMIT,
                        int pages_limit = YOUTUBE_PAGES_LIMIT)
                {
                    return PolyQueryRules(
                        qMin(pages_limit, YOUTUBE_PAGES_LIMIT),
                        tkn_page_token, start_token,
                        qMin(items_limit, YOUTUBE_ITEMS_LIMIT)
                    );
                }

//                QueryRules queryRules(int count = YOUTUBE_OFFSET_LIMIT, int offset = 0, int per_request = 99999) {
//                    return QueryRules(tkn_items, qMin(per_request, requestLimit()), qMin(count, YOUTUBE_OFFSET_LIMIT), offset);
//                }
            };
        }
    }
}

#endif // YOUTUBE_DEFINES_H
