#ifndef FOURSHARED_AUDIO
#define FOURSHARED_AUDIO

#include "fourshared_item.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Audio : virtual public Item {
            public:
                QString audioUrlFromId(const QString & id) {
                    QString url = baseUrlStr(
                        qst_site_base,
                        LSTR("web/rest/v1/playlist?itemType=file&beforeId=null&afterId=null&index=0&itemId=") % id, {}
                    );

                    QString res = Manager::prepare() -> putFollowed(url, siteHeaders()) -> toText();
                    Info::extract(res, LSTR("http"), LSTR("\""), res);
                    return res;
                }
                QString audioUrlFromPath(const QString & path) {
                    Html::Document doc = Web::Manager::prepare() -> getFollowed(path) -> toHtml();
                    return doc.find("input.jsD1PreviewUrl").value();
                }

                QJsonValue audioInfo(const QUrlQuery & args) { return audioInfo(args.queryItemValue(CMD_ID)); }
                QJsonValue audioInfo(const QString & id) { return itemInfo(dmt_audio, id); }

                QJsonValue audioSearch(const QUrlQuery & args) { return audioSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue audioSearch(const SearchLimit & limits, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    SourceFlags perm = permissions(sf_audio_by_title);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api: {
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

                        case sf_site: {
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

                    return prepareBlock(dmt_audio, cmd_mtd_audio_search, response, limits, block_params);
                }
            };
        }
    }
}

#endif // FOURSHARED_AUDIO
