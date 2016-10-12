#ifndef SOUNDCLOUD_GROUP
#define SOUNDCLOUD_GROUP

#include "soundcloud_defines.h"
#include "modules/core/misc/format.h"

// https://soundcloudgroups.com/

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Group : public Base {
            public:
                QJsonObject groupInfo(const QString & group_id) {
                    SourceFlags perm = permissions(sf_group_by_id);

                    switch(perm) {
                        case sf_api: {
                            return sRequest(
                                baseUrlStr(qst_api, path_groups % '/' % group_id, {}),
                                call_type_json, 0, proc_json_patch
                            );
                        break;}

                        case sf_site: {
                            return sRequest(
                                baseUrlStr(qst_site, path_groups % '/' % group_id, {}),
                                call_type_json, 0, proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", Logger::log_error);
                    }

                    return QJsonObject();
                }

                QJsonValue groupsById(const QUrlQuery & args) {
                    return groupsById(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue groupsById(const QString & group_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_group_by_id);

                    if (!Info::isNumber(group_id))
                        return groupsByName(group_id, count, offset);

                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_groups % '/' % group_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, path_groups % '/' % group_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP BY ID is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_id, response, {}, {{CMD_ID, group_id}});
                }

                QJsonValue groupsByAudio(const QUrlQuery & args) {
                    return groupsByAudio(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue groupsByAudio(const QString & track_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_group_by_audio);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api:
                        case sf_site: {
                            response = pRequest(baseUrlStr(qst_site_alt1, QStringLiteral("tracks/%1/groups").arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP BY TRACK is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_audio, response, {}, {{CMD_ID, track_id}});
                }

                QJsonValue groupsByUser(const QUrlQuery & args) {
                    return groupsByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue groupsByUser(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_group_by_user);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_user_groups.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site, path_user_groups.arg(user_id),
                                    {{QStringLiteral("representation"), QStringLiteral("mini")}}
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP BY USER is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_user, response, {}, {{CMD_ID, user_id}});
                }

                QJsonValue groupsByName(const QUrlQuery & args) {
                    return groupsByName(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue groupsByName(const QString & gname, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    SourceFlags perm = permissions(sf_group_by_title);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_groups, {{tkn_q, gname}}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("search/groups"),
                                    {
                                        {tkn_q, gname},
                                        {QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base)},
                                        {QStringLiteral("sc_a_id"), generateMark()},
                                        {QStringLiteral("facet"), QStringLiteral("model")}
                                    }
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP BY NAME is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_name, response, {}, {{CMD_PREDICATE, name}});
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_GROUP
