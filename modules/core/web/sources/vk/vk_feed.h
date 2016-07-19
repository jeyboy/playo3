#ifndef VK_FEED
#define VK_FEED

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Feed : public Base {
//                Permissions perm = permissions(pr_media_content);

//                switch(perm) {
//                    case perm_site:
//                    case perm_api: {

//                    }

//                    default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
//                }

//                return QJsonArray();

            public:
                // https://new.vk.com/dev/newsfeed.getRecommended
                QJsonValue feedsBySource(const QStringList & sources = QStringList()) { // not finished
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
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

                        default: Logger::obj().write("VK", "feedsBySource is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // VK_FEED
