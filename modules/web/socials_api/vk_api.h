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
        static Api * instance(QObject * parent, const QJsonObject & obj);
        inline static void close() { delete self; }

        void fromJson(const QJsonObject & hash);
        QJsonObject toJson();

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

        QJsonArray search_postprocess(QString & predicate, QString & genre, const SearchLimit & limitations) {
            if (predicate.isEmpty() && limitations.by_popularity())
                return audioPopular(true, genre);
            else
                return audioSearch(predicate, limitations.by_artists(), limitations.by_owns(), limitations.by_popularity(), limitations.count);
        }

    public slots:
        inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
        void proceedAuthResponse(const QUrl & url);
    protected:
        inline QString baseUrlStr(const QString & predicate) { return base_url % predicate; }

        inline QString offsetKey() const { return offset_key; }
        inline QString limitKey() const { return limit_key; }
        inline int requestLimit() const { return 200; }
        inline void iterateOffset(int & offset, QJsonObject & response, QUrl & /*url*/) { offset = response.value(offsetKey()).toInt(); }

        inline QJsonObject & extractBody(QJsonObject & response) { return (response = response.value(response_key).toObject()); }
        inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(finished_key).toBool(); }
        bool extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message);

        QUrl buildUrl(QUrl tUrl, int offset, int limit);
        bool captchaProcessing(QJsonObject & response, QUrl & url);
    //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }

    private:
        inline Api(QObject * parent, QJsonObject hash) : WebApi(parent), TeuAuth(), RequestApi() {
            fromJson(hash);
        }

        inline Api() : WebApi(), TeuAuth() {}

        static Api * self;
    };
}

#endif // VK_API_H
