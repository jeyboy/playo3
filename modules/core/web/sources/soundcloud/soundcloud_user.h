#ifndef SOUNDCLOUD_USER
#define SOUNDCLOUD_USER

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class User : public Base {
            public:
                QJsonValue usersById(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_user_by_id);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_users % '/' % user_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, path_users % '/' % user_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "USER BY ID is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_user, cmd_mtd_users_by_id, response, {}, {{CMD_ID, user_id}});
                }

                QJsonValue usersByName(const QString & uname, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_user_by_title);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_users, {{tkn_q, uname}}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, LSTR("search/users"),
                                    {
                                        { tkn_q, uname },
                                        { LSTR("user_id"),  Manager::cookie(LSTR("sc_anonymous_id"), url_site_base) },
                                        { LSTR("sc_a_id"),  generateMark() },
                                        { LSTR("facet"),    LSTR("place") }
                                    }
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "USER BY NAME is not accessable", Logger::log_error);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_users_by_name, response, {}, {{CMD_PREDICATE, name}});
                }

                QJsonValue usersByAudioLikes(const QString & track_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_user_by_likes);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_track_favorites.arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, LSTR("tracks/%1/likers").arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "USER BY TRACK LIKES is not accessable", Logger::log_error);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_users_by_audio_likes, response, {}, {{CMD_ID, track_id}});
                }

                QJsonValue usersByAudioReposting(const QString & track_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_user_by_audio_repost);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api:
                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site, LSTR("e1/tracks/%1/reposters").arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "USER BY TRACK REPOST is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_user, cmd_mtd_users_by_audio_reposting, response, {}, {{CMD_ID, track_id}});
                }

                QJsonValue userFollowings(const QUrlQuery & args) {
                    return userFollowings(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue userFollowings(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_user_by_user);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_user_followings.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch, COLLECTION_FIELDS
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site, LSTR("users/%1/followings").arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS << LSTR("collection"), call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "USER FOLLOWINGS is not accessable", Logger::log_error);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_user_followings, response, {}, {{CMD_ID, user_id}});
                }

                QJsonValue userFollowers(const QUrlQuery & args) {
                    return userFollowers(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue userFollowers(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_user_by_user);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_user_followers.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch, COLLECTION_FIELDS
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site, LSTR("users/%1/followers").arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS << LSTR("collection"), call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "USER FOLLOWERS is not accessable", Logger::log_error);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_user_followers, response, {}, {{CMD_ID, user_id}});
                }

                QJsonValue userAudioRecommendations(const QUrlQuery & args) {
                    return userAudioRecommendations(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue userAudioRecommendations(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_audio_recs_by_user);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api:
                        case sf_site: {
                            // curl 'https://api-v2.soundcloud.com/me/personalized-tracks?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1473279344' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0'
                            // curl 'https://api-v2.soundcloud.com/me/personalized-tracks?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1467724310&offset=0&limit=200' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7'

                            bool has_station_head = user_id.startsWith(val_station_head_tkn);

                            if (has_station_head || user_id != userID()) {
                                QString uid = has_station_head ? user_id : QString(val_station_head_tkn % user_id);

                                response = pRequest(
                                    baseUrlStr(qst_site_alt1, LSTR("stations/%1/tracks").arg(uid), {}),
                                    call_type_json, rules(offset, count), 0,
                                    proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                                );
                                return prepareBlock(dmt_user, cmd_mtd_user_audio_recommendations, response, {}, {{CMD_ID, user_id}});
                            } else {
                                response = pRequest(
                                    baseUrlStr(qst_site_alt1, LSTR("me/personalized-tracks"), {}),
                                    call_type_json, rules(offset, count), 0,
                                    proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                                );

                                QJsonArray block_content;
                                for(QJsonArray::Iterator rec = response.content.begin(); rec != response.content.end(); rec++) {
                                    QJsonObject obj = (*rec).toObject();
                                    QueriableArg::arrAppend(block_content, JSON_ARR(obj, LSTR("recommended")));
                                }
                                response.content = block_content;

                                return prepareBlock(dmt_user, cmd_mtd_user_audio_recommendations, response, {}, {{CMD_ID, user_id}});
                            }
                        break;}

                        default: Logger::obj().write(name(), "USER RECOMMENDATIONS is not accessable", Logger::log_error);
                    }

                    return QJsonObject();
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_USER
