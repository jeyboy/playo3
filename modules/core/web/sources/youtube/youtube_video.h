#ifndef YOUTUBE_VIDEO_H
#define YOUTUBE_VIDEO_H

#include "youtube_defines.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Video : public virtual Base {
            public:
                QJsonValue videoRecommendations(const QUrlQuery & /*args*/) {
                    return videoRecommendations();
                }

                QJsonValue videoRecommendations(const QUrlQuery & /*args*/) {
                    return QJsonObject();
                }



//                QueriableResponse response = pRequest(
//                    baseUrlStr(qst_api, path_search, {
//                        {tkn_part, tkn_snippet},
//                        {LSTR("fields"), LSTR("items(id,snippet),nextPageToken,pageInfo")},
//                        {LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT},
//                        {LSTR("safeSearch"), LSTR("none")},
//                        {tkn_video_embedable, LSTR("true")}, // any // true
//                        {tkn_type, LSTR("video")}, // channel // playlist // video
//                    //  Your request can also use the Boolean NOT (-) and OR (|) operators to exclude videos or to find videos that are associated with one of several
//                    //  search terms. For example, to search for videos matching either "boating" or "sailing", set the q parameter value to boating|sailing.
//                    //  Similarly, to search for videos matching either "boating" or "sailing" but not "fishing", set the q parameter value to boating|sailing -fishing.
//                    //  Note that the pipe character must be URL-escaped when it is sent in your API request. The URL-escaped value for the pipe character is %7C.
//                        {tkn_q, limits.predicate},
//                    }),

////                                setOrder(query, hottest ? LSTR("rating") : LSTR("relevance"));
////                                setMusicVideoCategory(query);

////                                else if (!relatedVideoId.isEmpty())
////                                    setParam(query, LSTR("relatedToVideoId"), relatedVideoId);
////                        searchUrl(limits.predicate, limits.genre, limits.by_popularity()),
//                    call_type_json,
//                    rules(QString(), limits.items_limit)
//                );

                QJsonValue videoCategories(const QUrlQuery & args) {
                    return videoCategories(args.queryItemValue(CMD_OFFSET));
                }
                QJsonValue videoCategories(const QString & offset_token = QString()) {
//                    {
//                       "kind": "youtube#videoCategory",
//                       "etag": "\"I_8xdZu766_FSaexEaDXTIfEWc0/HwXKamM1Q20q9BN-oBJavSGkfDI\"",
//                       "id": "15",
//                       "snippet": {
//                        "channelId": "UCBR8-60-B28hp2BmDPdntcQ",
//                        "title": "Pets & Animals",
//                        "assignable": true
//                       }
//                      }

                    QueriableResponse response = pRequest(
                        baseUrlStr(qst_api, path_video_cats, {
                            {tkn_part,          tkn_snippet},
                            {tkn_region_code,   siteLocale(LSTR("ua"))},
//                           id 	string
//                           The id parameter specifies a comma-separated list of video category IDs for the resources that you are retrieving.
                        }),
                        call_type_json, rules(offset_token)
                    );

                    return prepareBlock(dmt_video_set, cmd_mtd_video_categories, response);
                }
            };
        }
    }
}

#endif // YOUTUBE_VIDEO_H
