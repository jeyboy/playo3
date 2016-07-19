#ifndef VK_GROUP
#define VK_GROUP

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Group : public Base {
            public:
                QJsonValue groupsByIdOrPermas(const QStringList & ids) {
                    return groupsByIdOrPerma(ids.join(QStringLiteral(",")));
                }

                QJsonValue groupsByIdOrPerma(const QString & id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return saRequest(
                                baseUrlStr(
                                    qst_api_def, path_groups_by_id,
                                    { { tkn_group_ids, id } }
                                ),
                                call_type_json, 0, proc_json_extract
                            );
                        }

                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
                    }

                    return QJsonArray();
                }

                QJsonValue groupsByName(const QString & name) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return saRequest(
                                baseUrlStr(
                                    qst_api_def, path_groups_search,
                                    {
                                        { tkn_type, val_group_types },
                                        { tkn_q, name },
                                        { QStringLiteral("count"), 100/*0*/ }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract,
                                QStringList() << tkn_response << tkn_items
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

#endif // VK_GROUP
