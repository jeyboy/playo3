#ifndef FOURSHARED_SET
#define FOURSHARED_SET

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Set : public Base {
            public:
                enum SetType { set_popular };

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular: return QStringLiteral("Popular: ");
                        default: return QStringLiteral("Unknown: ");
                    }
                }

                QJsonValue setByType(const SetType & setType, const SearchLimit & limits) {
                    Permissions perm = permissions(pr_search_media);

                    switch(perm) {
                        case perm_api: {
                            switch(setType) {
                                case set_popular: {
                                    QJsonObject res;

                                    if (limits.include_audio())
                                        res.insert(
                                            block_items_audio,
                                            pRequest(
                                                baseUrlStr(
                                                    qst_api_search, tkn_files,
                                                    {{ tkn_category, music }}
                                                ),
                                                call_type_json,
                                                rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                                0, proc_json_extract, QStringList() << tkn_files
                                            )
                                        );

                                    if (limits.include_video())
                                        res.insert(
                                            block_items_video,
                                            pRequest(
                                                baseUrlStr(
                                                    qst_api_search, tkn_files,
                                                    {{ tkn_category, video }}
                                                ),
                                                call_type_json,
                                                rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                                0, proc_json_extract, QStringList() << tkn_files
                                            )
                                        );


                                    return res;
                                }
                            }
                        break;}

                        case perm_site: {
                            switch(setType) {
                                case set_popular: {
                                    QJsonObject res;

                                    if (limits.include_audio())
                                        res.insert(
                                            block_items_audio,
                                            pRequest(
                                                baseUrlStr(
                                                    qst_site_search,
                                                    QStringLiteral("q/lastmonth/CAQD/%1/music").arg(OFFSET_TEMPLATE),
                                                    {}
                                                ),
                                                call_type_html,
                                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                                0, proc_tracks1
                                            )
                                        );

                                    if (limits.include_video())
                                        res.insert(
                                            block_items_video,
                                            pRequest(
                                                baseUrlStr(
                                                    qst_site_search,
                                                    QStringLiteral("q/lastmonth/CAQD/%1/video").arg(OFFSET_TEMPLATE),
                                                    {}
                                                ),
                                                call_type_html,
                                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                                0, proc_video1
                                            )
                                        );

                                    return res;
                                }
                            }
                        break;}

                        default: Logger::obj().write("Fourshared", "SET BY TYPE is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // FOURSHARED_SET
