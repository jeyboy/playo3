#ifndef FOURSHARED_ITEM
#define FOURSHARED_ITEM

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Item : public Base {
            public:
                QJsonValue itemsSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            QJsonObject res;

                            if (limits.include_audio())
                                res.insert(
                                    block_audio,
                                    pRequest(
                                        baseUrlStr(
                                            qst_api_def, tkn_files,
                                            {
                                                { tkn_category, music },
                                                { tkn_query, limits.predicate }
                                            }
                                        ),
                                        call_type_json,
                                        rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_json_extract, QStringList() << tkn_files
                                    )
                                );

                            if (limits.include_video())
                                res.insert(
                                    block_video,
                                    pRequest(
                                        baseUrlStr(
                                            qst_api_def, tkn_files,
                                            {
                                                { tkn_category, video },
                                                { tkn_query, limits.predicate }
                                            }
                                        ),
                                        call_type_json,
                                        rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_json_extract, QStringList() << tkn_files
                                    )
                                );


                            return res;
                        }

                        case perm_site: {
                            QJsonObject res;

                            if (limits.include_audio())
                                res.insert(
                                    block_audio,
                                    pRequest(
                                        QStringLiteral("http://search.4shared.com/q/CCQD/%1/music/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                                        call_type_html,
                                        rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_tracks1
                                    )
                                );

                            if (limits.include_video())
                                res.insert(
                                    block_video,
                                    pRequest(
                                        QStringLiteral("http://search.4shared.com/q/CCQD/%1/video/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                                        call_type_html,
                                        rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_video1
                                    )
                                );

                            return res;
                        }

                        default: Logger::obj().write("Fourshared", "TRACK SEARCH is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // FOURSHARED_ITEM
