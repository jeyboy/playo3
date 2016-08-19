#ifndef YOUTUBE_API_H
#define YOUTUBE_API_H

#include <qregexp.h>
#include "modules/core/interfaces/singleton.h"
#include "modules/core/interfaces/isource.h"
//#include "modules/core/web/interfaces/teu_auth.h"
//#include "modules/core/web/apis/service/recaptcha.h"

//#include "modules/core/web/interfaces/sociable.h"

#include "youtube/youtube_request_api.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Api : public ISource, public RequestApi, public Singleton<Api>/*, public Sociable*/ {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType sourceType() const { return dt_web_youtube; }
                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_api) {
                    QUrlQuery query;
                    query.addQueryItem(tkn_key, val_client_tkn);
                    return query;
                }
//                inline SourceFlags defaultFlags() {
//                    return (SourceFlags)(
//                        /*sf_auth_api_has | */sf_auth_site_has | sf_site_user_content_auth_only |
//                        sf_api_search_auth_only | sf_api_user_content_auth_only
//                    );
//                }
//                QString authUrl();

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                bool connectUser() { return true; }

            protected:
                QJsonValue popular(const SearchLimit & limits) {
                    QueriableResponse response;

                    response = pRequest(
                        videosUrl(),
                        call_type_json,
                        rules()
                    );

//                    return lQuery(
//                        videosUrl(),
//                        queryRules(100)
//                    );

                    return prepareBlock(dmt_set, cmd_mtd_set_by_type, response, limits/*, {{CMD_SET_TYPE, set_type}}*/);
                }

                QJsonValue searchProc(const SearchLimit & limits) { //count = 5
                    QueriableResponse response = pRequest(
                        searchUrl(limits.predicate, limits.genre, limits.by_popularity()),
                        call_type_json,
                        rules(QString(), limits.items_limit)
                    );

                    initDuration(response.content);
                    return prepareBlock(dmt_video, cmd_mtd_unknown, response, limits);
                }

                inline QString refresh(const QString & path, const DataMediaType & /*itemMediaType*/) { return idToUrl(path); }
                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) { return url_base % predicate; }

//                inline QString offsetKey() const { return tkn_page_token; }
//                inline QString limitKey() const { return tkn_max_results; }
//                inline int requestLimit() const { return 200; }

                inline bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message) {
                    QJsonObject error = json.value(QStringLiteral("error")).toObject();
                    if (error.isEmpty()) {
                        arg -> prepareRequestUrlByToken(
                            QStringLiteral("pageToken"),
                            json.value(QStringLiteral("nextPageToken")).toString()
                        );
                        return true;
                    } else {
                        code = error.value(QStringLiteral("code")).toInt();
                        message = error.value(QStringLiteral("message")).toString();
                        return false;
                    }
                }                
            };
        }
    }
}

#endif // YOUTUBE_API_H
