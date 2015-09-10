#ifndef OD_API_H
#define OD_API_H

#include "../auth_chemas/teu_auth.h"
#include "od_request_api.h"

#define HASH_KEY QStringLiteral("hash")

namespace Od {
    class Api : public RequestApi, public TeuAuth {
        Q_OBJECT
    public:
        static Api * instance();
        static Api * instance(QJsonObject obj);
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Od"); }
        inline Playo3::WebSubType siteType() { return Playo3::od_site; }
        inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("jsessionid=") %  token()); }
        QString authUrl();

        void fromJson(QJsonObject hash);
        QJsonObject toJson();

        inline bool isConnected() { return !token().isEmpty(); }

    public slots:
        void connection() {
            formConnection();
            qDebug() << "LOL" << refresh("82297702323201");
        }
        inline void disconnect() {
            WebApi::disconnect(); setParams(QString(), QString(), QString());
            hash_key = QString();
        }
        void proceedAuthResponse(const QUrl & url);

    protected:
        bool sessionIsValid() {
            QJsonObject obj = WebManager::manager() -> getJson(initAudioUrl());
            return !obj.contains(QStringLiteral("error"));
        }

        void hashConnection() {
            QNetworkReply * reply = WebManager::manager() -> followedGet(initUrl(), initHeaders());
            reply -> deleteLater();

            if (!sessionIsValid())
                if (!formConnection())
                    return;

            setParams(grabSID(), userID(), QString());
        }

        bool formConnection() {
            qDebug() << "SOSO" << authRequestUrl();

            checkCredentials();

            WebResponse * reply = WebManager::manager() -> unfollowedForm(authRequestUrl(), initHeaders());

            // need to check - if credentials is wrong - nullify it

            QUrl url = reply -> redirectUrl().toUrl();
            QUrlQuery query(url.query());

            hash_key = query.queryItemValue(QStringLiteral("httpsdata"));
            reply -> deleteLater();


            if (hash_key.isEmpty()) {
                nullifyCredentials();
                return;
            }

            reply = WebManager::manager() -> followedGet(url, initHeaders());
            Html::Document doc(reply);

            checkSecurity(doc);
            WebManager::printCookies();

            setParams(grabSID(), grabUserId(doc), QString());
            qDebug() << token() << userID();
            reply -> deleteLater();
        }

        QJsonArray search_postprocess(QString & /*predicate*/, QString & /*genre*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        inline QString baseUrlStr(const QString & predicate) { return base_url % predicate; }

        inline QString offsetKey() const { return offset_key; }
        inline QString limitKey() const { return limit_key; }
        inline int requestLimit() const { return 100; }

        inline QJsonObject & extractBody(QJsonObject & response) { return response; }
        inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("response")).toArray().isEmpty(); }
        inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) {
//            QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
//            message = stat_obj.value(QStringLiteral("error_message")).toString();
            return true/*(code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0*/;
        }
    private:
        inline Api(QJsonObject hash) : TeuAuth(), RequestApi() { fromJson(hash); }
        inline Api() : TeuAuth(), RequestApi() { }
        inline virtual ~Api() {}

        static Api * self;
    };
}

#endif // SOUNDCLOUD_API_H
