#ifndef VK_DEFINES
#define VK_DEFINES

#include "modules/core/interfaces/isource_perm_flags.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"

#include "modules/core/web/grabber_keys.h"
#include "vk_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Base : public IQueriable {
            protected:
                virtual Permissions permissions(const PermitRequest & req_perm = pr_search_media) = 0;

                inline QString boolToStr(const bool & val) { return val ? val_str_true : val_str_false; }

                PolyQueryRules rules(
                    int offset = 0, int items_limit = DEFAULT_ITEMS_LIMIT, int pages_limit = DEFAULT_REQUESTS_LIMIT,
                    int /*per_request*/ = 100,
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
            };
        }
    }
}

#endif // VK_DEFINES
