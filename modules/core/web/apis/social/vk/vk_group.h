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

                QJsonArray groupsByIdOrPerma(const QString & id) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_group_ids, id);

                    return saRequest(
                        baseUrlStr(qst_api_def, path_groups_by_id, query),
                        call_type_json, 0, proc_json_extract
                    );
                }

                QJsonArray groupsByName(const QString & name) {
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
            };
        }
    }
}

#endif // VK_GROUP
