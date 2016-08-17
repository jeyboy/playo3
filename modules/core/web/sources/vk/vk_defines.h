#ifndef VK_DEFINES
#define VK_DEFINES

#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"

#include "vk_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Base : public virtual ISource, public virtual IQueriable {
            protected:
                PolyQueryRules rules(
                    int offset = 0, int items_limit = DEFAULT_ITEMS_LIMIT, int pages_limit = DEFAULT_REQUESTS_LIMIT,
//                    int per_request = 100,
                    ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_limit, DEFAULT_REQUESTS_LIMIT),
                        QString(), // limit already inserted to scripts
                        0, // per_request
                        QString(), // template already inserted to scripts
                        offset
                    );
                }

                void permaToId(const QString & perma, QString & id, QString & id_type) {
                    QUrlQuery query;
                    setParam(query, tkn_screen_name, perma);

                    QJsonObject ret = sRequest(
                        baseUrlStr(qst_api_def, path_resole_user, query),
                        call_type_json, 0, proc_json_extract
                    );

                    id = QString::number(ret.value(tkn_object_id).toInt());
                    id_type = ret.value(tkn_type).toString();
                }
            };
        }
    }
}

#endif // VK_DEFINES
