#ifndef FOURSHARED_VIDEO
#define FOURSHARED_VIDEO

#include "fourshared_item.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Video : virtual public Item {
            public:
                QString videoUrlFromId(const QString & id) {
                    return videoUrlFromPath(
                        baseUrlStr(
                            qst_site_base, LSTR("web/account/videoPreview?fileID=") % id, {}
                        )
                    );
                }
                QString videoUrlFromPath(const QString & path) {
                    QString res = Manager::prepare() -> getFollowed(path, siteHeaders()) -> toText();
                    return Info::extractLimitedBy(res, LSTR("file: \""), LSTR("\""));
                }

                QJsonValue videoInfo(const QUrlQuery & args) { return videoInfo(args.queryItemValue(CMD_ID)); }
                QJsonValue videoInfo(const QString & id) { return itemInfo(dmt_video, id); }

                QJsonValue videoSearch(const QUrlQuery & args) { return videoSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue videoSearch(const SearchLimit & limits, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    SourceFlags perm = permissions(sf_video_by_title);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
                            response = pRequest(
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
                        break;}

                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_search,
                                    LSTR("q/CCQD/%1/video/%2").arg(OFFSET_TEMPLATE, limits.predicate), {}
                                ),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0, proc_video1
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_video, cmd_mtd_video_search, response, limits, block_params);
                }
            };
        }
    }
}

#endif // FOURSHARED_VIDEO
