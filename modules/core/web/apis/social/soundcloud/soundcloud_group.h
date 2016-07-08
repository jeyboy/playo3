#ifndef SOUNDCLOUD_IGROUP
#define SOUNDCLOUD_IGROUP

#include "soundcloud_defines.h"
#include "modules/core/misc/format.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Group : public Base {
            protected:
                QJsonObject groupInfo(const QString & group_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            return sRequest(
                                baseUrlStr(qst_api_def, path_groups % '/' % group_id, {}),
                                call_type_json, 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            return sRequest(
                                baseUrlStr(qst_site_def, path_groups % '/' % group_id, {}),
                                call_type_json, 0, proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "GROUP INFO is not accessable", true);
                    }

                    return QJsonObject();
                }

                QJsonValue groupsById(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    if (!Info::isNumber(group_id))
                        return groupsByName(group_id);

                    switch(perm) {
                        case perm_api: {
                            return pRequest(
                                baseUrlStr(qst_api_def, path_groups % '/' % group_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            return pRequest(
                                baseUrlStr(qst_site_def, path_groups % '/' % group_id, {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "GROUP BY ID is not accessable", true);
                    }
                    return QJsonArray();
                }

                QJsonValue groupsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            return pRequest(baseUrlStr(qst_site_alt1, QStringLiteral("tracks/%1/groups").arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "GROUP BY TRACK is not accessable", true);
                    }

                    return QJsonArray();
                }

                QJsonValue groupsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            return pRequest(
                                baseUrlStr(qst_api_def, path_user_groups.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            return pRequest(
                                baseUrlStr(
                                    qst_site_def, path_user_groups.arg(user_id),
                                    {{QStringLiteral("representation"), QStringLiteral("mini")}}
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "GROUP BY USER is not accessable", true);
                    }

                    return QJsonArray();
                }

                QJsonValue groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            return pRequest(
                                baseUrlStr(qst_api_def, path_groups, {{tkn_q, name}}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            return pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("search/groups"),
                                    {
                                        {tkn_q, name},
                                        {QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base)},
                                        {QStringLiteral("sc_a_id"), generateMark()},
                                        {QStringLiteral("facet"), QStringLiteral("model")}
                                    }
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "GROUP BY NAME is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_IGROUP
