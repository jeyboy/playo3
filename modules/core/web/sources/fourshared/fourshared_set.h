#ifndef FOURSHARED_SET
#define FOURSHARED_SET

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Set : public Base {
            protected:
                enum SetType { set_popular_audio, set_popular_video };

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_audio: return QStringLiteral("Popular audio");
                        case set_popular_video: return QStringLiteral("Popular video");
                        default: return QStringLiteral("Unknown");
                    }
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    Permissions perm = permissions(pr_search_media);
                    QueriableResponse response;
                    DataMediaType dmt_val = dmt_unknow;

                    switch(set_type) {
                        case set_popular_audio: {
                            dmt_val = dmt_audio;

                            switch(perm) {
                                case perm_api: {
                                    response = pRequest(
                                        baseUrlStr(
                                            qst_api_search, tkn_files,
                                            {{ tkn_category, music }}
                                        ),
                                        call_type_json,
                                        rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_json_extract, QStringList() << tkn_files
                                    );
                                break;}

                                case perm_site: {
                                    response = pRequest(
                                        baseUrlStr(
                                            qst_site_search,
                                            QStringLiteral("q/lastmonth/CAQD/%1/music").arg(OFFSET_TEMPLATE),
                                            {}
                                        ),
                                        call_type_html,
                                        rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_tracks1
                                    );
                                break;}

                                default: Logger::obj().write("Fourshared", "SET BY TYPE is not accessable", true);
                            }
                        break;}

                        case set_popular_video: {
                            dmt_val = dmt_video;

                            switch(perm) {
                                case perm_api: {
                                    response = pRequest(
                                        baseUrlStr(
                                            qst_api_search, tkn_files,
                                            {{ tkn_category, video }}
                                        ),
                                        call_type_json,
                                        rulesApi(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_json_extract, QStringList() << tkn_files
                                    );
                                break;}

                                case perm_site: {
                                    response = pRequest(
                                        baseUrlStr(
                                            qst_site_search,
                                            QStringLiteral("q/lastmonth/CAQD/%1/video").arg(OFFSET_TEMPLATE),
                                            {}
                                        ),
                                        call_type_html,
                                        rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                        0, proc_video1
                                    );
                                break;}

                                default: Logger::obj().write("Fourshared", "SET BY TYPE is not accessable", true);
                            }
                        break;}
                    }

                    return prepareBlock(dmt_val, cmd_mtd_set_by_type, response, limits, {}, {{CMD_SET_TYPE, set_type}});
                }
            public:
                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                inline QMap<QString, QString> setsList() {
                    return {
                        {setTypeToStr(set_popular_audio),       QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_popular_audio))},
                        {setTypeToStr(set_popular_video),       QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_popular_video))}
                    };
                }
                QJsonValue openSet(const QUrlQuery & attrs) {
                    return QJsonArray() << Set::setByType(attrs);
                }
            };
        }
    }
}

#endif // FOURSHARED_SET
