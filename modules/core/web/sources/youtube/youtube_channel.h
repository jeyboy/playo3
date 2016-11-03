#ifndef YOUTUBE_CHANNEL_H
#define YOUTUBE_CHANNEL_H

#include "youtube_defines.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Channel : public virtual Base {
            public:
//                path_subscriptions

//                forChannelId 	string
//                The forChannelId parameter specifies a comma-separated list of channel IDs. The API response will then only contain subscriptions matching those channels.

                QJsonValue videoChannelsById(const QUrlQuery & args) {
                    return videoChannelsById(args.queryItemValue(CMD_ID));
                }
                QJsonValue videoChannelsById(const QString & channel_ids) {
                    SourceFlags perm = permissions(sf_channel_by_id);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            QUrlQuery query = baseQuery(qst_api, {
                                {tkn_part, tkn_snippet},
        //                        {LSTR("fields"), LSTR("items(id,snippet),nextPageToken,pageInfo")},
                                {LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT},
                                {LSTR("id"), channel_ids}
                            });


                            QueriableResponse response = pRequest(
                                baseUrlStr(qst_api, path_channels, query),
                                call_type_json, rules(), 0, proc_json_extract, YOUTUBE_ITEMS
                            );

                            return prepareBlock(dmt_user, cmd_mtd_video_channels_by_id, response);
                        }
                        default: return QJsonObject();
                    }
                }

                QJsonValue videoChannelsByName(const QUrlQuery & args) {
                    return videoChannelsByName(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoChannelsByName(const QString & predicate, const QString & token = QString()) {
                    SourceFlags perm = permissions(sf_channel_by_title);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            QUrlQuery query = videoQuery({
                                {tkn_type, LSTR("channel")}, // channel // playlist // video
                                {tkn_q, predicate}
                            });

                            QueriableResponse response = pRequest(
                                baseUrlStr(qst_api, path_search, query), call_type_json,
                                rules(token), 0, proc_json_extract, YOUTUBE_ITEMS
                            );

                            return prepareBlock(dmt_user, cmd_mtd_video_channels_by_name, response, {}, {{CMD_PREDICATE, predicate}});
                        break;}
                        default: Logger::obj().write(name(), "videoChannelsByName", Logger::log_error);
                    }

                    return QJsonObject();
                }

                QJsonValue videoChannelsByUser(const QUrlQuery & args) {
                    return videoChannelsByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoChannelsByUser(const QString & user_id, const QString & token = QString()) {
                    SourceFlags perm = permissions(sf_channel_by_user);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            QUrlQuery query = baseQuery(qst_api, {
                                {tkn_part, tkn_snippet},
                                {LSTR("fields"), LSTR("items(id,snippet),nextPageToken,pageInfo")},
                                {LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT},
                            });

                            if (user_id == apiUserID())
                                query.addQueryItem(LSTR("mine"), const_true);
                            else
                                query.addQueryItem(LSTR("channelId"), user_id);


                            QueriableResponse response = pRequest(
                                baseUrlStr(qst_api, path_subscriptions, query),
                                call_type_json, rules(token), 0, proc_json_extract,
                                YOUTUBE_ITEMS, call_method_get, authHeaders()
                            );

                            return prepareBlock(dmt_user, cmd_mtd_video_channels_by_user, response);
                        break;}
                        default: Logger::obj().write(name(), "videoChannelsByUser", Logger::log_error);
                    }

                    return QJsonObject();
                }

                QJsonValue videoChannelsByCategory(const QUrlQuery & args) {
                    return videoChannelsByCategory(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoChannelsByCategory(const QString & category_id, const QString & token = QString()) {
                    SourceFlags perm = permissions(sf_channel_by_category);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {

                            QUrlQuery query = baseQuery(qst_api, {
                                {tkn_part, tkn_snippet},
        //                        {LSTR("fields"), LSTR("items(id,snippet),nextPageToken,pageInfo")},
                                {LSTR("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT},
                                {LSTR("categoryId"), category_id}
                            });


                            QueriableResponse response = pRequest(
                                baseUrlStr(qst_api, path_channels, query),
                                call_type_json, rules(token)
                            );

                            return prepareBlock(dmt_user, cmd_mtd_video_channels_by_category, response);

                        break;}
                        default: Logger::obj().write(name(), "videoChannelsByCategory", Logger::log_error);
                    }

                    return QJsonObject();
                }
            };
        }
    }
}

#endif // YOUTUBE_CHANNEL_H
