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
        static Api * instance(QObject * parent, const QJsonObject & obj);
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Od"); }
        inline Playo3::WebSubType siteType() { return Playo3::od_site; }
        inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("jsessionid=") % token()); }
        QString authUrl();

        void fromJson(QJsonObject hash);
        QJsonObject toJson();

        inline bool isConnected() { return !token().isEmpty(); }

        void objectInfo(QString & uid, Func func) {
            registerAsync(
                QtConcurrent::run((RequestApi *)this, &RequestApi::userInfo, uid), func
            );
        }

    public slots:
        bool connection(bool onlyAuto = false) {
            bool res = !onlyAuto || (onlyAuto && !token().isEmpty());

            if (res) {
                res |= hashConnection(onlyAuto);
                if (res) emit authorized();
            }
            return res;

//            setParams(grabSID(), userID(), QString());
//            qDebug() << "SID" << token();

//            if (!token().isEmpty())
//                qDebug() << "LOL" << refresh("82297702323201");
        }
        inline void disconnect() {
            WebApi::disconnect(); setParams(QString(), QString(), QString());
            hash_key = QString();
        }
        void proceedAuthResponse(const QUrl & url);

    protected:
        bool hashConnection(bool onlyAuto) {
            QNetworkReply * reply = WebManager::manager() -> followedGet(initUrl(), initHeaders());
            reply -> deleteLater();

            if (!sessionIsValid())
                if (!onlyAuto && !formConnection())
                    return false;

            setParams(grabSID(), userID(), QString());
            return true;
        }

        bool formConnection() {
            WebManager::printCookies();
            checkCredentials();

            WebResponse * reply = WebManager::manager() -> unfollowedForm(authRequestUrl(), initHeaders());
            QUrl url = reply -> redirectUrl().toUrl();           
            hash_key = WebManager::paramVal(url, QStringLiteral("httpsdata"));
            reply -> deleteLater();

            reply = WebManager::manager() -> followedGet(url, initHeaders());
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
            return !WebManager::cookie(QStringLiteral("AUTHCODE")).isEmpty();
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

#endif // SOUNDCLOUD_API_H
