#ifndef WEB_APIS
#define WEB_APIS

#include "apis/storage/_storages.h"
#include "apis/site/_sites.h"
#include "apis/social/_socials.h"
#include "apis/service/_services.h"

namespace Core {
    namespace Web {
        class Apis {
            static QHash<Web::SubType, ISearchable *> sites;
        public:
            static void initiate(const QJsonObject & obj);
            static QHash<Web::SubType, ISearchable *> list() { return sites; }
            inline static ISearchable * engine(const Web::SubType & item_type) { return sites.value(item_type); }
            static void close(QJsonObject & obj);
        };
    }
}

#endif // WEB_APIS
