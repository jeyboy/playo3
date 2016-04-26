#ifndef YOUTUBE_API_H
#define YOUTUBE_API_H

#include <qregexp.h>
#include "modules/core/interfaces/singleton.h"
//#include "modules/core/web/interfaces/teu_auth.h"
//#include "modules/core/web/apis/service/recaptcha.h"

//#include "modules/core/web/interfaces/sociable.h"

#include "youtube_request_api.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Api : public QObject,/*public TeuAuth,*/ public RequestApi, public Singleton<Api>/*, public Sociable*/ {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_youtube; }
                inline QUrlQuery genDefaultParams() {
                    QUrlQuery query;
                    query.addQueryItem(tkn_key, val_client_tkn);
                    return query;
                }
                QString authUrl();

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return true; /*!token().isEmpty();*/ }

            public slots:
                bool connection() { return true; }
                inline void disconnect() {
//                    clearParams();
//                    clearFriends();
//                    clearGroups();
                }

            protected:
                QUrl buildUrl(QUrl tUrl, int /*offset*/, int limit, const QJsonObject & prev_response) {
//                    "pageInfo": {
//                      "totalResults": integer,
//                      "resultsPerPage": integer
//                    },

                    QUrl url(tUrl);
                    QUrlQuery query = QUrlQuery(url);
                    setLimit(query, limit, prev_response.value(QStringLiteral("nextPageToken")));
                    url.setQuery(query);
                    return url;
                }

                inline QString refresh(const QString & path) { return idToUrl(path); }
                inline QString baseUrlStr(const QString & predicate) { return url_base % predicate; }

                inline QString offsetKey() const { return tkn_page_token; }
                inline QString limitKey() const { return tkn_max_results; }
                inline int requestLimit() const { return 200; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & /*response*/, int /*offset*/) { return false; /*response.value(tkn_response).toArray().isEmpty()*/; }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & response, int & code, QString & message) {
                    QJsonObject error = response.value(QStringLiteral("error")).toObject();
                    if (error.isEmpty())
                        return true;
                    else {
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
