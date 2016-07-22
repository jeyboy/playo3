#ifndef OD_DEFINES
#define OD_DEFINES

#include "modules/core/interfaces/isource_perm_flags.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"

#include "modules/core/web/grabber_keys.h"
//#include "od_keys.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Base : public IQueriable {
            protected:
                virtual Permissions permissions(const PermitRequest & req_perm = pr_search_media) = 0;


            };
        }
    }
}

#endif // OD_DEFINES
