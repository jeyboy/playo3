#ifndef VK_USER
#define VK_USER

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class User : public Base {
            public:
                QJsonObject userStatus(QString & user_id) { // deactivated: 'deleted', id, first_name, last_name, counters
                    QUrlQuery query;
                    setParam(query, tkn_fields, tkn_counters);
                    setParam(query, tkn_user_ids, user_id);

                    QJsonObject ret = sRequest(
                        baseUrlStr(qst_api_def, path_user_info, query),
                        call_type_json
                    );


                    return ret.value(tkn_response).toArray().first().toObject();
                }

                QJsonValue usersByIdsOrPermas(const QStringList & ids) {
                    return usersByIdOrPerma(ids.join(QStringLiteral(",")));
                }

                QJsonValue usersByIdOrPerma(const QString & id) {
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

                QJsonValue usersByName(const QString & name) {
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
            };
        }
    }
}

#endif // VK_USER
