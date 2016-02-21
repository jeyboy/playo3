#ifndef YOUTUBE_API_H
#define YOUTUBE_API_H

#include "modules/core/interfaces/singleton.h"
//#include "modules/core/web/interfaces/teu_auth.h"
//#include "modules/core/web/apis/service/recaptcha.h"

//#include "modules/core/web/interfaces/sociable.h"
#include "modules/core/web/utils/js_parser.h"

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
                decryptSignature(const QString & s) {

                }


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

// cpn
//                var l11 = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-", "_"];
//                var l1I = "";
//                var lIl = 0;
//                while (lIl < 16) {
//                l1I = (l1I + l11[Math.floor((Math.random() * 64))]);
//                lIl++;
//                }
//                l1I;

                QString tryToTakeUrl(const QString & path) {
                    QString url_map_tkn = QStringLiteral("url_encoded_fmt_stream_map");

                    //there should be 2 variants:
                        //  str has 's' param, which should be decoded first with usage of separate js file
                            // http://stackoverflow.com/questions/21510857/best-approach-to-decode-youtube-cipher-signature-using-php-or-js
                            // http://www.codeproject.com/Articles/2352/JavaScript-call-from-C
                            // https://github.com/bitnol/CipherAPI
                        //  str has 'signature' or 'sig' which simply copied to link


                    QString response = Web::Manager::prepare() -> followedGet(url_embed.arg(path)) -> toText();
                    int pos = response.indexOf(url_map_tkn);
                    if (pos != -1) {
                        pos += url_map_tkn.length() + 3;
                        int len = response.indexOf('"', pos) - pos;
                        QString map = response.mid(pos, len);
                        qDebug() << "------------------------------";
                        QStringList links = map.split(','); // each link contains param type equal to the 'video' or 'audio'


                        // need to split by \\u0026
//                        QString res = links.first().replace("\\u0026", "&");
                        qDebug() << "LINK!" << res;
                        qDebug() << "------------------------------";
                        QUrlQuery vQuery(res);
                        res = QUrl::fromPercentEncoding(vQuery.queryItemValue(QStringLiteral("url")).toLatin1()) + "&cpn=Twcgu-mcw4SjsEIM";
                        qDebug() << "LINK" << res << vQuery.toString();
                        return res;
                    }

                    QString url_adaptive_map_tkn = QStringLiteral("adaptive_fmts");
                    pos = response.indexOf(url_adaptive_map_tkn);
                    if (pos != -1) {
                        pos += url_map_tkn.length() + 3;
                        int len = response.indexOf('"', pos) - pos;
                        qDebug() << "-------------- ADAPTIVE  ----------------";
                        QString map = response.mid(pos, len);
                        qDebug() << "------------------------------";
//                        QStringList links = map.split(','); // each link contains param type equal to the 'video' or 'audio'

                    }
                }

                inline QString refresh(const QString & path) {
//                    !url.contains("ratebypass"))
//                            {
//                                url = url.append("&ratebypass=yes");
//                            }


                    QString response = Web::Manager::prepare() -> followedGet(url_info.arg(path)) -> toText();
                    QUrlQuery query(response);

                    // https://www.quora.com/How-can-I-make-a-YouTube-video-downloader-web-application-from-scratch
                    // https://github.com/rg3/youtube-dl/blob/9dd8e46a2d0860421b4bb4f616f05e5ebd686380/youtube_dl/extractor/youtube.py
                    // http://superuser.com/questions/773719/how-do-all-of-these-save-video-from-youtube-services-work

                    if (query.hasQueryItem(QStringLiteral("url_encoded_fmt_stream_map"))) {
                        QVector<QStringRef> links = query.queryItemValue(QStringLiteral("url_encoded_fmt_stream_map")).splitRef(',');

                        QUrlQuery vQuery(links.first().toString());
                        QString res = QUrl::fromPercentEncoding(QUrl::fromPercentEncoding(vQuery.queryItemValue(QStringLiteral("url")).toLatin1()).toLatin1());
                        qDebug() << "LINK" << res << vQuery.toString();
                        return res;
                    } else if (query.hasQueryItem(QStringLiteral("adaptive_fmts"))) {
                        Logger::obj().write(QStringLiteral("Youtube API ADAPTIVE"), path, QStringList() << query.toString(), true);
                    } else {
                        if (query.hasQueryItem(QStringLiteral("errorcode")))
                            return tryToTakeUrl(path);
                        else {
                            // ?
                            Logger::obj().write(QStringLiteral("Youtube API"), path, QStringList() << query.toString(), true);
                        }
                    }

                    return path;
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
