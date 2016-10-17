#ifndef YOUTUBE_REQUEST_API
#define YOUTUBE_REQUEST_API

#include "youtube_misc.h"

//#include <qregexp.h>
////#include "modules/core/web/interfaces/teu_auth.h"
////#include "modules/core/web/apis/service/recaptcha.h"

#define YOUTUBE_INFO_ITEMS_LIMIT 50
#define YOUTUBE_ITEMS_LIMIT 100
#define YOUTUBE_PAGES_LIMIT 10

namespace Core {
    namespace Web {
        namespace Youtube {
            class Requests : public Misc {
                bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message) {
                    QJsonObject error = json.value(LSTR("error")).toObject();
                    if (error.isEmpty()) {
                        arg -> prepareRequestUrlByToken(
                            json.value(LSTR("nextPageToken")).toString()
                        );
                        return true;
                    } else {
                        code = error.value(LSTR("code")).toInt();
                        message = error.value(LSTR("message")).toString();
                        return false;
                    }
                }

                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) {
                    return url_base % predicate;
                }
                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_api) {
                    QUrlQuery query;
                    query.addQueryItem(tkn_key, val_client_tkn);
                    return query;
                }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                bool connectUser() { return true; }

            public:               
                inline virtual ~Requests() {}

                QJsonValue popular(const SearchLimit & limits) {
                    QueriableResponse response;

                    response = pRequest(
                        videosUrl(),
                        call_type_json,
                        rules()
                    );

                    return prepareBlock(dmt_set, cmd_mtd_set_by_type, response, limits, {}/*, {{CMD_SET_TYPE, set_type}}*/);
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
            };
        }
    }
}

#endif // YOUTUBE_REQUEST_API
