#ifndef SOUNDCLOUD_BASE_DEFINES
#define SOUNDCLOUD_BASE_DEFINES

#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"
#include "keys.h"

#define SOUNDCLOUD_PAGES_LIMIT 25
#define SOUNDCLOUD_ITEMS_LIMIT 2000
#define SOUNDCLOUD_PER_REQUEST_LIMIT 200
#define SOUNDCLOUD_IDS_PER_REQUEST 25

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Base {
            protected:
                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_q, predicate); }
                inline void setIdsFilter(QUrlQuery & query, const QStringList & uids) { setParam(query, tkn_ids, uids); }

                PolyQueryRules rules(
                    int offset = 0, int items_limit = SOUNDCLOUD_ITEMS_LIMIT, int pages_limit = SOUNDCLOUD_PAGES_LIMIT,
                    int per_request = SOUNDCLOUD_PER_REQUEST_LIMIT,
                    ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, SOUNDCLOUD_ITEMS_LIMIT),
                        qMin(pages_limit, SOUNDCLOUD_PAGES_LIMIT),
                        tkn_limit,
                        qMin(qMin(per_request, items_limit), SOUNDCLOUD_PER_REQUEST_LIMIT),
                        tkn_offset,
                        offset
                    );
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_BASE_DEFINES
