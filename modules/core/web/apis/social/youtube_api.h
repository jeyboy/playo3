#ifndef YOUTUBE_API_H
#define YOUTUBE_API_H

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
                inline SubType siteType() { return site_youtube; }
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

                inline QString refresh(const QString & path) {
                    QString response = Web::Manager::prepare() -> followedGet(url_info.arg(path)) -> toText();
                    QUrlQuery query(response);
                    //account_playback_token // token // url_encoded_fmt_stream_map
                    if (!query.hasQueryItem(QStringLiteral("url_encoded_fmt_stream_map"))) {
                        qDebug() << path << query.toString();
                        if (query.hasQueryItem(QStringLiteral("errorcode"))) {
                            //"cbr=Firefox&reason=\u042D\u0442\u043E+\u0432\u0438\u0434\u0435\u043E+\u0441\u043E\u0434\u0435\u0440\u0436\u0438\u0442+\u043A\u043E\u043D\u0442\u0435\u043D\u0442+\u043E\u0442+\u043F\u0430\u0440\u0442\u043D\u0435\u0440\u0430+UMG.+\u041D\u0430+\u043D\u0435\u043A\u043E\u0442\u043E\u0440\u044B\u0445+\u0432\u0435\u0431-\u0441\u0430\u0439\u0442\u0430\u0445+\u0435\u0433\u043E+\u043F\u0440\u043E\u0441\u043C\u043E\u0442\u0440+\u043E\u0433\u0440\u0430\u043D\u0438\u0447\u0435\u043D.%3Cbr%2F%3E%3Cu%3E%3Ca+href%3D%27http%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3DwqcPgGMJMdc%26feature%3Dplayer_embedded%27+target%3D%27_blank%27%3E\u041F\u0440\u043E\u0441\u043C\u043E\u0442\u0440\u0435\u0442\u044C+\u043D\u0430+YouTube%3C%2Fa%3E%3C%2Fu%3E&enablecsi=1&eventid=evXGVr7hDYbjWq6_o5gB&status=fail&csi_page_type=embed&c=WEB&errordetail=0&cosver=6.1&cbrver=39.0&errorcode=150&cos=Windows"
                        } else {
                            // ?
                        }
                        return path;
                    } else {
                        QVector<QStringRef> links = query.queryItemValue(QStringLiteral("url_encoded_fmt_stream_map")).splitRef(',');
                        QUrlQuery vQuery(links.first().toString());
                        QString res;
                        qDebug() << "LINK" << res;
                        return QUrl::fromPercentEncoding(QUrl::fromPercentEncoding(vQuery.queryItemValue(QStringLiteral("url")).toLatin1()).toLatin1());
                    }
                }
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
