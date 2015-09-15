#ifndef VK_API_H
#define VK_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "vk_request_api.h"
//#include "modules/data_struct/search/search_settings.h"

namespace Vk {
    class Api : public WebApi, public TeuAuth, public RequestApi {
        Q_OBJECT
    public:
        inline QString name() const { return QStringLiteral("Vk"); }
        inline Playo3::WebSubType siteType() { return Playo3::vk_site; }
        inline QUrlQuery genDefaultParams() {
            QUrlQuery query = QUrlQuery();

            query.addQueryItem(version_key, apiVersion());
            query.addQueryItem(access_token_key, token());
    //        query.addQueryItem(QStringLiteral("test_mode"), "1");

            return query;
        }

        inline QString authUrl() { return RequestApi::authUrl(); }

        inline ~Api() {}

        static Api * instance();
        static Api * instance(QObject * parent, const Json::Obj & obj);
        inline static void close() { delete self; }

        void fromJson(const Json::Obj & hash);
        Json::Obj toJson();

        inline bool isConnected() { return !token().isEmpty() && !userID().isEmpty(); }

        void userInfo(QString & uid, bool fullInfo, Func func) {
            registerAsync(
                QtConcurrent::run((RequestApi *)this, &RequestApi::userInfo, uid, fullInfo), func
            );
        }
    //    void wallAudio(QString & uid, Func func) {
    //        registerAsync(
    //            QtConcurrent::run((RequestApi *)this, &RequestApi::wallAudio, uid), func
    //        );
    //    }
        void audioRecomendations(QString & uid, bool byUser, bool randomize, Func func) {
            registerAsync(
                QtConcurrent::run((RequestApi *)this, &RequestApi::audioRecomendations, uid, byUser, randomize), func
            );
        }

    public slots:
        inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
        void proceedAuthResponse(const QUrl & url);
    protected:
        inline QString baseUrlStr(const QString & predicate) { return base_url % predicate; }

        inline QString offsetKey() const { return offset_key; }
        inline QString limitKey() const { return limit_key; }
        inline int requestLimit() const { return 200; }
        inline void iterateOffset(int & offset, Json::Obj & response, QUrl & /*url*/) { offset = response.value(offsetKey()).toInt(); }

        inline Json::Obj & extractBody(Json::Obj & response) { return (response = response.obj(response_key)); }
        inline bool endReached(Json::Obj & response, int /*offset*/) { return response.value(finished_key).toBool(); }
        bool extractStatus(QUrl & url, Json::Obj & response, int & code, QString & message);

        QUrl buildUrl(QUrl tUrl, int offset, int limit);
        bool captchaProcessing(Json::Obj & response, QUrl & url);
    //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }

    private:
        inline Api(QObject * parent, const Json::Obj & hash) : WebApi(parent), TeuAuth(), RequestApi() {
            fromJson(hash);
        }

        inline Api() : WebApi(), TeuAuth() {}

        static Api * self;
    };
}

#endif // VK_API_H
