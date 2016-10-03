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
                    SourceFlags perm = permissions(sf_feed);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api, QStringLiteral("newsfeed.get"),
                                    {
                                        { QStringLiteral("filters"), QStringLiteral("audio,video") },
                                        { QStringLiteral("count"), 100 },
                                        { QStringLiteral("source_ids"), sources }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_items
                            );
                        break;}

                        default: Logger::obj().write(name(), "feedsBySource is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_feed, block_content);
                }
            };
        }
    }
}

#endif // VK_FEED
