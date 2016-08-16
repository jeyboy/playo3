#ifndef SOUNDCLOUD_USER
#define SOUNDCLOUD_USER

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class User : public Base {
            public:
                QJsonValue usersById(const QUrlQuery & args) {
                    return usersById(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue usersById(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api_def, path_users % '/' % user_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site_def, path_users % '/' % user_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "USER BY ID is not accessable", true);
                    }

                    return prepareBlock(dmt_user, cmd_mtd_users_by_id, response, {{CMD_ID, user_id}});
                }

                QJsonValue usersByName(const QUrlQuery & args) {
                    return usersByName(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue usersByName(const QString & name, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api_def, path_users, {{tkn_q, name}}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("search/users"),
                                    {
                                        { tkn_q, name },
                                        { QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base) },
                                        { QStringLiteral("sc_a_id"), generateMark() },
                                        { QStringLiteral("facet"), QStringLiteral("place") }
                                    }
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "USER BY NAME is not accessable", true);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_users_by_name, response, {{CMD_PREDICATE, name}});
                }

                QJsonValue usersByTrackLikes(const QUrlQuery & args) {
                    return usersByTrackLikes(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue usersByTrackLikes(const QString & track_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api_def, path_track_favorites.arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("tracks/%1/likers").arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "USER BY TRACK LIKES is not accessable", true);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_users_by_track_likes, response, {{CMD_ID, track_id}});
                }

                QJsonValue usersByTrackReposting(const QUrlQuery & args) {
                    return usersByTrackReposting(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue usersByTrackReposting(const QString & track_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_def, QStringLiteral("e1/tracks/%1/reposters").arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "USER BY TRACK REPOST is not accessable", true);
                    }

                    return prepareBlock(dmt_user, cmd_mtd_users_by_track_reposting, response, {{CMD_ID, track_id}});
                }

                QJsonValue userFollowings(const QUrlQuery & args) {
                    return userFollowings(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue userFollowings(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api_def, path_user_followings.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch, COLLECTION_FIELDS
                            );
                        }

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_def, QStringLiteral("users/%1/followings").arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "USER FOLLOWINGS is not accessable", true);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_user_followings, response, {{CMD_ID, user_id}});
                }

                QJsonValue userFollowers(const QUrlQuery & args) {
                    return userFollowers(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue userFollowers(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api_def, path_user_followers.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch, COLLECTION_FIELDS
                            );
                        }

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_def, QStringLiteral("users/%1/followers").arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "USER FOLLOWERS is not accessable", true);
                    }
                    return prepareBlock(dmt_user, cmd_mtd_user_followers, response, {{CMD_ID, user_id}});
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_USER
