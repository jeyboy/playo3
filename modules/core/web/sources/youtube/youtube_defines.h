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

#define YOUTUBE_PLAYLIST_FAVORITE_UID LSTR("youtube_fav_pl")
#define YOUTUBE_PLAYLIST_HISTORY_UID LSTR("youtube_hist_pl")
#define YOUTUBE_PLAYLIST_LATER_UID LSTR("youtube_ltr_pl")
#define YOUTUBE_PLAYLIST_LIKED_UID LSTR("youtube_lkd_pl")
#define YOUTUBE_PLAYLIST_UPLOADS_UID LSTR("youtube_upld_pl")

#define YOUTUBE_ITEMS QStringList() << tkn_items

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
//                    setParam(query, tkn_part, tkn_snippet);
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
                        setParam(query, "chart", "mostPopular");

                    setParam(query, tkn_part, "snippet,contentDetails");
                    setParam(query, "fields", "items(contentDetails,fileDetails,id,localizations,player,snippet),nextPageToken,pageInfo");
                    setParam(query, QLatin1String("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT);
                    setParam(query, QLatin1String("regionCode"), siteLocale(const_default_locale));
//                    setMusicVideoCategory(query);

                    return baseUrlStr(qst_api, LSTR("videos"), query);
                }

                QUrlQuery videoQuery(const std::initializer_list<std::pair<QString, QVariant> > & params = {}, const bool & only_music = false) {
                    QUrlQuery query = baseQuery(qst_api, {
                        {tkn_part, tkn_snippet},
                        {LSTR("fields"), LSTR("items(id,snippet),nextPageToken,pageInfo")},
                        {LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT},
                        {LSTR("safeSearch"), LSTR("none")},
                    });

                    if (only_music)
                        setMusicVideoCategory(query);

                    setParam(query, params);

                    return query;
                }


//                inline Headers siteHeaders() {
//                    return Headers({{LSTR("x-security"), Manager::cookie(LSTR("Login"), url_html_site_base)}});
//                }


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

//                inline void setEmbedable(QUrlQuery & query) {
//                    setParam(query, tkn_video_embedable, const_true); // any // true
//                    setParam(query, tkn_type, LSTR("video")); // channel // playlist // video
//                }

                void proceedDurationResult(const QStringList & ids, QJsonArray & arr) {
                    pRequest(
                        videosUrl(ids),
                        call_type_json,
                        rules(QString(), YOUTUBE_INFO_ITEMS_LIMIT),
                        &arr, proc_json_extract, YOUTUBE_ITEMS
                    );
                }

                void initDuration(QJsonArray & arr) {
                    QStringList ids;
                    QJsonArray res;

                    for(QJsonArray::Iterator item = arr.begin(); item != arr.end(); item++) {
                        ids << JSON_CSTR2((*item).toObject(), tkn_id, LSTR("videoId"));

                        if (ids.length() == YOUTUBE_INFO_ITEMS_LIMIT) {
                            proceedDurationResult(ids, res);
                            ids.clear();
                        }
                    }

                    if (!ids.isEmpty())
                        proceedDurationResult(ids, res);

                    arr = res;
                }

                PolyQueryRules rules(const QString & start_token = QString(),
                    int items_limit = YOUTUBE_ITEMS_LIMIT, int pages_limit = YOUTUBE_PAGES_LIMIT)
                {
                    return PolyQueryRules(
                        qMin(pages_limit, YOUTUBE_PAGES_LIMIT),
                        tkn_page_token, start_token,
                        qMin(items_limit, YOUTUBE_ITEMS_LIMIT)
                    );
                }
            };
        }
    }
}

#endif // YOUTUBE_DEFINES_H
