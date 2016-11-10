#ifndef FOURSHARED_AUDIO
#define FOURSHARED_AUDIO

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Audio : public Base {
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
                //TODO: write me
                QJsonValue audioInfo(const QString & id) {
                    // curl 'http://www.4shared.com/web/link/download/file' -H 'Accept: */*' -H 'Accept-Encoding: gzip, deflate' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' -H 'Cookie: _ga=GA1.2.1540151391.1465296058; search.view2=ls; hostid=-1949250789; Login=815437060; Password=b14a5e097683c629bd57bc9087b635b4; 4langcookie=ru; day1host=h; sm_dapi_session=1; JSESSIONID=3DFE9558B4C0A1C711BBEE44953C47AE.dc558; ulin=true; WWW_JSESSIONID=3DFE9558B4C0A1C711BBEE44953C47AE.dc558; sr.vw=1; cd1v=vhbajxce; showApkOwnerGuideYnpqoAQx=null' -H 'DNT: 1' -H 'Host: www.4shared.com' -H 'Referer: http://www.4shared.com/account/home.jsp' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:49.0) Gecko/20100101 Firefox/49.0' -H 'X-Requested-With: XMLHttpRequest' -H 'locale: ru' -H 'x-security: 815437060' --data 'fileId=M3AEm3Qnce'


                    QString url = baseUrlStr(
                        qst_site_base, LSTR("web/link/download/file"), {{LSTR("fileId"), id}}
                    );

                    QJsonObject res = Manager::prepare() -> jsonPost(url, siteHeaders());
                    url = JSON_STR(res, LSTR("link"));

                    Response * response = Manager::prepare() -> getFollowed(url, siteHeaders());

                    //QueriableArg(QJsonArray * arr, const QString & url, const ApiCallType & call_type, const AdditionalProc & post_proc = proc_none
                    QString err;
                    QJsonArray arr;
                    QueriableArg arg(&arr, url, call_type_html, proc_tracks2);
                    htmlToJson(&arg, response, err);

                    return arr;
                }

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
                                    LSTR("q/CCQD/%1/music/%2").arg(OFFSET_TEMPLATE, limits.predicate), {}
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
