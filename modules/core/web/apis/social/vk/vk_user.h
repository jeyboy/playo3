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
            };
        }
    }
}

#endif // VK_USER
