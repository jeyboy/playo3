#ifndef FOURSHARED_TRACK
#define FOURSHARED_TRACK

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Track : public Base {
            public:
                QString trackUrlFromId(const QString & id) {
                    QUrl url = QUrl(QStringLiteral("http://www.4shared.com/web/rest/v1/playlist?itemType=file&beforeId=null&afterId=null&index=0&itemId=") % id);
                    QString res = Manager::prepare() -> putFollowed(url, siteHeaders()) -> toText();
                    Info::extract(res, QStringLiteral("http"), QStringLiteral("\""), res);
                    return res;
                }

                QString trackUrlFromPath(const QString & path) {
                    Html::Document doc = Web::Manager::prepare() -> getFollowed(path) -> toHtml();
                    return doc.find("input.jsD1PreviewUrl").value();
                }

                QJsonValue tracksSearch(const QUrlQuery & args) { return tracksSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue tracksSearch(const SearchLimit & limits, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    Permissions perm = permissions(pr_search_media);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_api_search, tkn_files,
                                    {
                                        { tkn_category, music },
                                        { tkn_query, limits.predicate }
                                    }
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
                                    QStringLiteral("q/CCQD/%1/music/%2").arg(OFFSET_TEMPLATE, limits.predicate), {}
                                ),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0, proc_tracks1
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_search, response, limits, block_params);
                }
            };
        }
    }
}

#endif // FOURSHARED_TRACK
