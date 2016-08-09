#ifndef FOURSHARED_ITEM
#define FOURSHARED_ITEM

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Item : public Base {
            public:
                QJsonValue itemsByCollection(const QString & collection_id) {
                    return sRequest(
                        baseUrlStr(
                            qst_site_base, QStringLiteral("web/accountActions/changeDir"),
                            {{ QStringLiteral("dirId"), collection_id }}
                        ),
                        call_type_json, 0, proc_none,
                        QStringList(), call_method_post, siteHeaders()
                    );
                }
            };
        }
    }
}

#endif // FOURSHARED_ITEM
