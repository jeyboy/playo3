#ifndef YOUTUBE_VIDEO_H
#define YOUTUBE_VIDEO_H

#include "youtube_defines.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Video : public virtual Base {
                // The hl parameter specifies the language that will be used for text values in the API response. The default value is en-US.
            public:
                QJsonValue videoInfo(const QUrlQuery & args) { return videoInfo(args.queryItemValue(CMD_ID)); }
                //TODO: write me
                QJsonValue videoInfo(const QString & id) { return QJsonObject(); }

                QJsonValue videoSearch(const QUrlQuery & args) { return videoSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue videoSearch(const SearchLimit & limits) {
                    // setOrder(query, hottest ? LSTR("rating") : LSTR("relevance"));

                    QueriableResponse response = pRequest(
                        baseUrlStr(qst_api, path_search, {
                            videoQuery({
//                               {tkn_video_embedable, const_true}, // any // true
                               {tkn_type, LSTR("video")}, // channel // playlist // video
                           //  Your request can also use the Boolean NOT (-) and OR (|) operators to exclude videos or to find videos that are associated with one of several
                           //  search terms. For example, to search for videos matching either "boating" or "sailing", set the q parameter value to boating|sailing.
                           //  Similarly, to search for videos matching either "boating" or "sailing" but not "fishing", set the q parameter value to boating|sailing -fishing.
                           //  Note that the pipe character must be URL-escaped when it is sent in your API request. The URL-escaped value for the pipe character is %7C.
                               {tkn_q, limits.predicate},
                            })
                        }),
                        call_type_json,
                        rules(QString(), limits.items_limit), 0, proc_json_extract, YOUTUBE_ITEMS
                    );

                    initDuration(response.content);
                    return prepareBlock(dmt_video, cmd_mtd_unknown, response, limits);
                }

                QJsonValue videoByCategory(const QUrlQuery & args) {
                    return videoByCategory(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoByCategory(const QString & category_id, const QString & token = QString()) {
                    QueriableResponse response = pRequest(
                        baseUrlStr(qst_api, path_search,
                            videoQuery({
//                                {tkn_video_embedable, const_true}, // any // true
                                {tkn_type, LSTR("video")}, // channel // playlist // video
                                {LSTR("videoCategoryId"), category_id}
                            })
                        ),
                        call_type_json, rules(token), 0, proc_json_extract, YOUTUBE_ITEMS
                    );

                    initDuration(response.content);
                    return prepareBlock(dmt_video, cmd_mtd_video_by_category, response);
                }

                QJsonValue videoByUser(const QUrlQuery & args) {
                    return videoByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoByUser(const QString & user_id, const QString & token = QString()) {
                    SourceFlags perm = permissions(sf_video_by_user);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            QUrlQuery query = videoQuery({
//                                {tkn_video_embedable, const_true}, // any // true
                                {tkn_type, LSTR("video")}, // channel // playlist // video
                            });

                            bool is_current_user = user_id == apiUserID();

                            if (is_current_user)
                                query.addQueryItem(LSTR("forMine"), const_true);
                            else
                                query.addQueryItem(LSTR("channelId"), user_id);

                            QueriableResponse response = pRequest(
                                baseUrlStr(qst_api, path_search, query),
                                call_type_json, rules(token),
                                0, proc_json_extract, YOUTUBE_ITEMS, call_method_get,
                                is_current_user ? authHeaders() : Headers()
                            );

                            initDuration(response.content);
                            return prepareBlock(dmt_video, cmd_mtd_video_by_user, response, {}, {{CMD_ID, user_id}});
                        break;}
                        default: Logger::obj().write(name(), "videoByUser", Logger::log_error);
                    }

                    return QJsonObject();
                }

                QJsonValue videoRecommendations(const QUrlQuery & args) {
                    return videoRecommendations(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoRecommendations(const QString & video_id, const QString & offset_token = QString()) {
                    QueriableResponse response = pRequest(
                        baseUrlStr(qst_api, path_search,
                           videoQuery({
                              {tkn_type, LSTR("video")},
                              {LSTR("relatedToVideoId"), video_id}
                           })
                        ),
                        call_type_json,
                        rules(offset_token)
                    );

                    initDuration(response.content);
                    return prepareBlock(dmt_video, cmd_mtd_video_recommendations, response);
                }

                //https://developers.google.com/youtube/v3/docs/videoCategories/list#part
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

                    initDuration(response.content);
                    return prepareBlock(dmt_video_set, cmd_mtd_video_categories, response);
                }

                QJsonValue videoByUserRating(const QUrlQuery & args) {
                    return videoByUserRating(args.queryItemValue(CMD_PREDICATE) == LSTR("1"));
                }
                QJsonValue videoByUserRating(const bool liked) {
                    SourceFlags perm = permissions(sf_video_by_user);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            QueriableResponse response = pRequest(
                                baseUrlStr(qst_api, path_videos, {
                                    {tkn_part, tkn_snippet % ',' % LSTR("contentDetails")},
                                    {LSTR("fields"), LSTR("items(id,snippet,contentDetails),nextPageToken,pageInfo")},
                                    {LSTR("myRating"), liked ? LSTR("like") : LSTR("dislike ")},
                                    {LSTR("maxResults"), 50}
                                }),
                                call_type_json, rules(token),
                                0, proc_json_extract, YOUTUBE_ITEMS, call_method_get,
                                authHeaders()
                            );

                            initDuration(response.content);
                            return prepareBlock(dmt_video, cmd_mtd_video_by_user_rating, response, {}, {{CMD_PREDICATE, liked ? LSTR("1") : LSTR("0")}});
                        break;}
                        default: Logger::obj().write(name(), "videoByRating", Logger::log_error);
                    }

                    return QJsonObject();
                }

                // https://developers.google.com/youtube/v3/docs/guideCategories/list
                QJsonValue officialVideoCategories() {
//                    {
//                       "kind": "youtube#videoCategory",
//                       "etag": "\"I_8xdZu766_FSaexEaDXTIfEWc0/HwXKamM1Q20q9BN-oBJavSGkfDI\"",
//                       "id": "15",
//                       "snippet": {
//                        "channelId": "UCBR8-60-B28hp2BmDPdntcQ",
//                        "title": "Pets & Animals"
//                       }
//                      }

                    QueriableResponse response = pRequest(
                        baseUrlStr(qst_api, path_quide_cats, {
                            {tkn_part,          tkn_snippet},
                            {tkn_region_code,   siteLocale(LSTR("ua"))},
//                           id 	string
//                           The id parameter specifies a comma-separated list of video category IDs for the resources that you are retrieving.
                        }),
                        call_type_json, rules(QString())
                    );
                    return prepareBlock(dmt_video_set, cmd_mtd_unknown, response);
                }
            };
        }
    }
}

#endif // YOUTUBE_VIDEO_H
