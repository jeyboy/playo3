#ifndef WEB_APIS
#define WEB_APIS

#include "apis/storage/_storages.h"
#include "apis/site/_sites.h"
#include "apis/social/_socials.h"
#include "apis/service/_services.h"

namespace Core {
    namespace Web {
        class Apis {
        public:
            static QHash<WebSubType, ISearchable *> list();
            inline static ISearchable * engine(WebSubType item_type) { return list().value(item_type); }
            inline static void close() {
                qDeleteAll(sites);
            }
        private:
            static QHash<Playo3::WebSubType, ISearchable *> sites;
        };
    }
}

#endif // WEB_APIS
