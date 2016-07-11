#ifndef VK_FEED
#define VK_FEED

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Feed : public Base {
            public:
                // https://new.vk.com/dev/newsfeed.getRecommended
                QJsonValue feedsBySource(const QStringList & sources = QStringList()) { // not finished
                    return saRequest(
                        baseUrlStr(
                            qst_api_def, QStringLiteral("newsfeed.get"),
                            {
                                { QStringLiteral("filters"), QStringLiteral("audio,video") },
                                { QStringLiteral("count"), 100 },
                                { QStringLiteral("source_ids"), sources }
                            }
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_items
                    );
                }
            };
        }
    }
}

#endif // VK_FEED
