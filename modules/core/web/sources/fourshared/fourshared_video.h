#ifndef FOURSHARED_VIDEO
#define FOURSHARED_VIDEO

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Video : public Base {
            public:
                QJsonValue videoSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_search_media);

                    switch(perm) {
                        case perm_api:
                            return pRequest(
                                    baseUrlStr(
                                        qst_api_search, tkn_files,
                                        {
                                            { tkn_category, video },
                                            { tkn_query, limits.predicate }
                                        }
                                    ),
                                    call_type_json,
                                    rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                    0, proc_json_extract, QStringList() << tkn_files
                                );

                        case perm_site:
                            return pRequest(
                                baseUrlStr(
                                    qst_site_search,
                                    QStringLiteral("q/CCQD/%1/video/%2").arg(OFFSET_TEMPLATE, limits.predicate), {}
                                ),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0, proc_video1
                            );

                        default: Logger::obj().write("Fourshared", "TRACK SEARCH is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // FOURSHARED_VIDEO
