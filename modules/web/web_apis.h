#ifndef WEB_APIS
#define WEB_APIS

#include "storages/fourshared/fourshared_api.h"
#include "socials_api/vk_api.h"
#include "socials_api/soundcloud_api.h"
#include "sites/site_apis.h"

namespace Web {
    class Apis {
    public:
        static QHash<Playo3::WebSubType, ISearchable *> list();
        inline static ISearchable * engine(Playo3::WebSubType item_type) { return list().value(item_type); }
        inline static void close() {
            qDeleteAll(sites);
        }
    private:
        static QHash<Playo3::WebSubType, ISearchable *> sites;
    };
}

#endif // WEB_APIS
