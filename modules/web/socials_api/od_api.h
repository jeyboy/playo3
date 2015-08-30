#ifndef OD_API_H
#define OD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "od_request_api.h"

namespace Od {
    class Api : public WebApi, public TeuAuth, public RequestApi {
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
            QHash<QString, QString> headers;
            headers.insert("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0");
            QNetworkReply * reply = WebManager::manager() -> postForm(authRequestUrl(), headers);
            qDebug() << "SESSION" << WebManager::cookie(QStringLiteral("JSESSIONID"));
            setParams(grabSID(), grabUserId(reply), QString());
            grabUserId(reply);
            qDebug() << token() << userID();
            reply -> deleteLater();

            QJsonObject obj = WebManager::manager() -> getJson(initAudioUrl());
            qDebug() << "OD INIT" << obj;

            qDebug() << "LOL" << refresh("82297702323201");
        }
        inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
        void proceedAuthResponse(const QUrl & url);

    protected:
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
        inline Api(QJsonObject hash) : WebApi(), TeuAuth(), RequestApi() { fromJson(hash); }
        inline Api() : WebApi(), TeuAuth(), RequestApi() { }
        inline virtual ~Api() {}

        static Api * self;
    };
}

#endif // SOUNDCLOUD_API_H
