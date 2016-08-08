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

//                QJsonValue itemsSearch(const SearchLimit & limits) {
//                    Permissions perm = permissions(pr_search_media);

//                    switch(perm) {
//                        case perm_api: {
//                            QJsonObject res;

//                            if (limits.include_audio()) {
//                                QJsonArray block_content = pRequest(
//                                    baseUrlStr(
//                                        qst_api_search, tkn_files,
//                                        {
//                                            { tkn_category, music },
//                                            { tkn_query, limits.predicate }
//                                        }
//                                    ),
//                                    call_type_json,
//                                    rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
//                                    0, proc_json_extract, QStringList() << tkn_files
//                                );

//                                addBlockToJson(
//                                    res,
//                                    block_items_audio,
//                                    block_content,
//                                    block_content.size() < limits.items_limit ? QString() :
//                                        Cmd::build(
//                                            siteType(),
//                                            cmd_mtd_items_search,
//                                            limits.toICmdParams(block_content.size())
//                                        )
//                                );

////                                res.insert(
////                                    block_items_audio,
////                                    pRequest(
////                                        baseUrlStr(
////                                            qst_api_search, tkn_files,
////                                            {
////                                                { tkn_category, music },
////                                                { tkn_query, limits.predicate }
////                                            }
////                                        ),
////                                        call_type_json,
////                                        rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
////                                        0, proc_json_extract, QStringList() << tkn_files
////                                    )
////                                );
//                            }

//                            if (limits.include_video())
//                                res.insert(
//                                    block_items_video,
//                                    pRequest(
//                                        baseUrlStr(
//                                            qst_api_search, tkn_files,
//                                            {
//                                                { tkn_category, video },
//                                                { tkn_query, limits.predicate }
//                                            }
//                                        ),
//                                        call_type_json,
//                                        rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
//                                        0, proc_json_extract, QStringList() << tkn_files
//                                    )
//                                );


//                            return res;
//                        }

//                        case perm_site: {
//                            QJsonObject res;

//                            if (limits.include_audio())
//                                res.insert(
//                                    block_items_audio,
//                                    pRequest(
//                                        baseUrlStr(
//                                            qst_site_search,
//                                            QStringLiteral("q/CCQD/%1/music/%2").arg(OFFSET_TEMPLATE, limits.predicate), {}
//                                        ),
//                                        call_type_html,
//                                        rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
//                                        0, proc_tracks1
//                                    )
//                                );

//                            if (limits.include_video())
//                                res.insert(
//                                    block_items_video,
//                                    pRequest(
//                                        baseUrlStr(
//                                            qst_site_search,
//                                            QStringLiteral("q/CCQD/%1/video/%2").arg(OFFSET_TEMPLATE, limits.predicate), {}
//                                        ),
//                                        call_type_html,
//                                        rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
//                                        0, proc_video1
//                                    )
//                                );

//                            return res;
//                        }

//                        default: Logger::obj().write("Fourshared", "TRACK SEARCH is not accessable", true);
//                    }

//                    return QJsonArray();
//                }
            };
        }
    }
}

#endif // FOURSHARED_ITEM
