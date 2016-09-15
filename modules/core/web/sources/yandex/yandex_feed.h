#ifndef YANDEX_FEED_H
#define YANDEX_FEED_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Feed : public virtual Base {
            public:
                // ncrnd=0.9069681936002026
                // days: [{day, tracksToPlay: [], events: [{type = 'recomended-genre-radio', title: {}, genres: [id, artists:[{}], albums: []]}, {type='genre-top, tracks: [], genre, title: {type, text}'}]}]
                inline QString feedsUrl() { return url_site_v1 + QStringLiteral("feed.jsx?nocheck="); }


//                QJsonValue feedsBySource(const QStringList & sources = QStringList()) { // not finished
//                    Permissions perm = permissions(pr_media_content);
//                    QJsonArray block_content;

//                    switch(perm) {
//                        case perm_site:
//                        case perm_api: {
//                            block_content = saRequest(
//                                baseUrlStr(
//                                    qst_api, QStringLiteral("newsfeed.get"),
//                                    {
//                                        { QStringLiteral("filters"), QStringLiteral("audio,video") },
//                                        { QStringLiteral("count"), 100 },
//                                        { QStringLiteral("source_ids"), sources }
//                                    }
//                                ),
//                                call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_items
//                            );
//                        break;}

//                        default: Logger::obj().write(name(), "feedsBySource is not accessable", true);
//                    }

//                    return prepareBlock(dmt_feed, block_content);
//                }
            };
        }
    }
}

#endif // YANDEX_FEED_H
