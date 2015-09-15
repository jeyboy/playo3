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
        static Api * instance(QObject * parent, const Json::Obj & obj);
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Od"); }
        inline Playo3::WebSubType siteType() { return Playo3::od_site; }
        inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("jsessionid=") % token()); }
        QString authUrl();

        void fromJson(const Json::Obj & hash);
        Json::Obj toJson();

        inline bool isConnected() { return !token().isEmpty(); }

        void objectInfo(const QString & uid, Func func) {
            registerAsync(
                QtConcurrent::run((RequestApi *)this, &RequestApi::userInfo, uid), func
            );
        }

        inline QString refresh(QString refresh_page) { // here refresh_page must by eq to track id
            Json::Obj obj = WebManager::manager() -> getJson(playAudioUrl(refresh_page));
            if (hasError(obj)) {
                connection(true);
                obj = WebManager::manager() -> getJson(playAudioUrl(refresh_page));
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
                res |= hashConnection(onlyAuto);
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

            Html::Doc doc(reply);
            checkSecurity(doc);

            setParams(QString(), grabUserId(doc), QString());
            reply -> deleteLater();
            return !WebManager::cookie(QStringLiteral("AUTHCODE")).isEmpty();
        }

        inline QString baseUrlStr(const QString & predicate) { return base_url % predicate; }

        inline QString offsetKey() const { return offset_key; }
        inline QString limitKey() const { return limit_key; }
        inline int requestLimit() const { return 100; }

        inline Json::Obj & extractBody(Json::Obj & response) { return response; }
        inline bool endReached(Json::Obj & response, int /*offset*/) {
            Json::Obj chunk_obj = response.obj(QStringLiteral("chunk"));
            if (chunk_obj.isEmpty()) return false;
            return chunk_obj.value(QStringLiteral("count")).toInt() < requestLimit();
        }
        inline bool extractStatus(QUrl & /*url*/, Json::Obj & /*response*/, int & /*code*/, QString & /*message*/) {
//            Json::Obj stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
//            message = stat_obj.value(QStringLiteral("error_message")).toString();
            return true/*(code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0*/;
        }
    private:
        inline Api(QObject * parent, const Json::Obj & hash) : RequestApi(parent), TeuAuth() {
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
