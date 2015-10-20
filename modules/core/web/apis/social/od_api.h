#ifndef OD_API_H
#define OD_API_H

#include "modules/core/misc/thread_utils.h"
#include "modules/core/web/auth_chemas/teu_auth.h"
#include "od_request_api.h"

#define HASH_KEY QStringLiteral("hash")

namespace Core {
    namespace Web {
        namespace Od {
            class Api : public RequestApi, public TeuAuth {
                Q_OBJECT
            public:
                static Api * instance();
                static Api * instance(QObject * parent, const QJsonObject & obj);
                inline static void close() { delete self; }

                inline QString name() const { return QStringLiteral("Od"); }
                inline Web::SubType siteType() { return od_site; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("jsessionid=") % token()); }
                QString authUrl();

                void fromJson(QJsonObject hash);
                QJsonObject toJson();

                inline bool isConnected() { return !token().isEmpty(); }

                void objectInfo(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, func);
                }

                inline QString refresh(QString refresh_page) { // here refresh_page must by eq to track id
                    QJsonObject obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
                    if (hasError(obj)) {
                        connection(true);
                        obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
                        qDebug() << "RECONECTION";
                    }
                    QUrl url(obj.value(QStringLiteral("play")).toString());
                    QUrlQuery query = QUrlQuery(url.query());
                    query.addQueryItem(QStringLiteral("clientHash"), calcMagicNumber(query.queryItemValue(QStringLiteral("md5"))));
                    url.setQuery(query);
                    return url.toString();
                }

            public slots:
                bool connection(bool onlyAuto = false) {
                    bool res = !onlyAuto || (onlyAuto && !token().isEmpty());

                    if (res) {
                        res &= hashConnection(onlyAuto);
                        if (res) emit authorized();
                    }

                    return res;
                }
                inline void disconnect() {
                    WebApi::disconnect(); setParams(QString(), QString(), QString());
                    hash_key = QString();
                }
                void proceedAuthResponse(const QUrl & url);

            protected:
                bool hashConnection(bool onlyAuto) {
                    QNetworkReply * reply = Manager::prepare() -> followedGet(initUrl(), initHeaders());
                    reply -> deleteLater();

                    if (!sessionIsValid())
                        if (!onlyAuto && !formConnection())
                            return false;

                    setParams(grabSID(), userID(), QString());
                    return true;
                }

                bool formConnection() {
                    Manager::printCookies();
                    if (!checkCredentials()) return false;

                    Response * reply = Manager::prepare() -> unfollowedForm(authRequestUrl(), initHeaders());
                    QUrl url = reply -> redirectUrl().toUrl();
                    hash_key = Manager::paramVal(url, QStringLiteral("httpsdata"));
                    reply -> deleteLater();

                    reply = Manager::prepare() -> followedGet(url, initHeaders());
                    QString error = reply -> paramVal(QStringLiteral("st.error"));
                    if (!error.isEmpty()) {
                        nullifyCredentials();
                        reply -> deleteLater();
                        return false;
                    }

                    Html::Document doc(reply);
                    checkSecurity(doc);

                    setParams(QString(), grabUserId(doc), QString());
                    reply -> deleteLater();
                    return !Manager::cookie(QStringLiteral("AUTHCODE")).isEmpty();
                }

                inline QString baseUrlStr(const QString & predicate) { return base_url % predicate; }

                inline QString offsetKey() const { return offset_key; }
                inline QString limitKey() const { return limit_key; }
                inline int requestLimit() const { return 100; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) {
                    QJsonObject chunk_obj = response.value(QStringLiteral("chunk")).toObject();
                    if (chunk_obj.isEmpty()) return false;
                    return chunk_obj.value(QStringLiteral("count")).toInt() < requestLimit();
                }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) {
        //            QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
        //            message = stat_obj.value(QStringLiteral("error_message")).toString();
                    return true/*(code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0*/;
                }
            private:
                inline Api(QObject * parent, QJsonObject hash) : RequestApi(parent), TeuAuth() {
                    fromJson(hash);
                    if (!sessionIsValid())
                       connection(true);
                }
                inline Api() : RequestApi(), TeuAuth() { }
                inline virtual ~Api() {}

                static Api * self;
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
