#ifndef VK_USER
#define VK_USER

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class User : public Base {
            public:
                QJsonObject userStatus(const QString & user_id) { // deactivated: 'deleted', id, first_name, last_name, counters
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            QUrlQuery query;
                            setParam(query, tkn_fields, tkn_counters);
                            setParam(query, tkn_user_ids, user_id);

                            QJsonObject ret = sRequest(baseUrlStr(qst_api_def, path_user_info, query), call_type_json);
                            return ret.value(tkn_response).toArray().first().toObject();
                        }

                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
                    }

                    return QJsonObject();
                }

                QJsonValue usersByIdsOrPermas(const QStringList & ids) {
                    return usersByIdOrPerma(ids.join(QStringLiteral(",")));
                }

                QJsonValue usersByIdOrPerma(const QString & id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return saRequest(
                                baseUrlStr(
                                    qst_api_def, path_user_info,
                                    {
                                        { tkn_user_ids, id },
                                        { tkn_fields, val_user_fields }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract
                            );
                        }

                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
                    }

                    return QJsonArray();
                }

                QJsonValue usersByName(const QString & name) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return saRequest(
                                baseUrlStr(
                                    qst_api_def, path_users_search,
                                    {
                                        { tkn_q, name },
                                        { tkn_fields, val_user_fields }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_items
                            );
                        }

                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // VK_USER
