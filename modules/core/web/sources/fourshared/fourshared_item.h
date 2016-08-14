#ifndef FOURSHARED_ITEM
#define FOURSHARED_ITEM

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Item : public Base {
            public:
                QJsonValue itemsByCollection(const QString & collection_id) {
                    QJsonArray block_content = saRequest(
                        baseUrlStr(
                            qst_site_base, QStringLiteral("web/accountActions/changeDir"),
                            {{ QStringLiteral("dirId"), collection_id }}
                        ),
                        call_type_json, 0, proc_json_proc,
                        QStringList(), call_method_post, siteHeaders()
                    );

                    return prepareBlock(dmt_any, block_content);
                }
            };
        }
    }
}

#endif // FOURSHARED_ITEM
