#ifndef OD_DEFINES
#define OD_DEFINES

#include "modules/core/interfaces/isource_perm_flags.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"

#include "modules/core/web/grabber_keys.h"
#include "od_keys.h"

#define OD_LIMIT_PER_REQUEST 100

namespace Core {
    namespace Web {
        namespace Od {
            class Base : public IQueriable {
            protected:
                virtual Permissions permissions(const PermitRequest & req_perm = pr_search_media) = 0;

                PolyQueryRules rules(
                    int offset = 0, int items_limit = 200, int pages_count = 10,
                    int per_request = OD_LIMIT_PER_REQUEST,
                    ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_count, DEFAULT_REQUESTS_LIMIT),
                        tkn_limit,
                        qMin(qMin(per_request, items_limit), OD_LIMIT_PER_REQUEST),
                        tkn_offset,
                        offset
                    );
                }

                inline QString audioUrlStr(const QString & predicate, const QUrlQuery & query = QUrlQuery()) {
                    return url_base_audio % predicate % tkn_coma_dot % genDefaultParams(qst_site).toString() %
                            (query.isEmpty() ? QString() : '?' % query.toString());

//                    QUrlQuery base_query = genDefaultParams(qst_site);
//                    QUrl url(url_base_audio % func % tkn_coma_dot % base_query.toString());
//                    url.setQuery(query);
//                    return url.toString();
                }
            };
        }
    }
}

#endif // OD_DEFINES
