#ifndef WEB_APIS
#define WEB_APIS

#include "storages/fourshared/fourshared_api.h"
#include "socials_api/vk_api.h"
#include "socials_api/soundcloud_api.h"
#include "sites/site_apis.h"

namespace Web {
    class Apis {
    public:
        static QList<ISearchable *> list();
        inline static void close() {
            qDeleteAll(sites);
        }
    private:
        static QList<ISearchable *> sites;
    };
}

#endif // WEB_APIS
