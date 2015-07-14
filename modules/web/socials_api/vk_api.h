#ifndef VK_API_H
#define VK_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "vk_request_api.h"
#include "media/genres/web/vk_genres.h"
//#include "modules/data_struct/search/search_settings.h"

class VkApi : public WebApi, public TeuAuth, public VkRequestApi {
    Q_OBJECT
public:
    inline QString name() const { return QStringLiteral("vk"); }
    inline QUrlQuery genDefaultParams() {
        QUrlQuery query = QUrlQuery();

        query.addQueryItem(QStringLiteral("v"), apiVersion());
        query.addQueryItem(QStringLiteral("access_token"), token());
//        query.addQueryItem(QStringLiteral("test_mode"), "1");

        return query;
    }

    inline QString authUrl() { return VkRequestApi::authUrl(); }

    inline ~VkApi() {}

    static VkApi * instance();
    static VkApi * instance(QObject * parent, QJsonObject obj);
    inline static void close() { delete self; }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    inline bool isConnected() { return !token().isEmpty() && !userID().isEmpty(); }

    void userInfo(QString & uid, bool fullInfo, Func func) {
        registerAsync(
            QtConcurrent::run((VkRequestApi *)this, &VkRequestApi::userInfo, uid, fullInfo), func
        );
    }
//    void wallAudio(QString & uid, Func func) {
//        registerAsync(
//            QtConcurrent::run((VkRequestApi *)this, &VkRequestApi::wallAudio, uid), func
//        );
//    }
    void audioRecomendations(QString & uid, bool byUser, bool randomize, Func func) {
        registerAsync(
            QtConcurrent::run((VkRequestApi *)this, &VkRequestApi::audioRecomendations, uid, byUser, randomize), func
        );
    }

signals:
    void showCaptcha();
public slots:
    inline void disconnect() { WebApi::disconnect(); setParams("", "", ""); }
    void proceedAuthResponse(const QUrl & url);
protected:
    inline QString baseUrlStr(QString & predicate) { return QStringLiteral("https://api.vk.com/method/") % predicate; }

    inline QString offsetKey() const { return QStringLiteral("offset"); }
    inline QString limitKey() const { return QStringLiteral("count"); }
    inline int requestLimit() const { return 200; }
    inline void iterateOffset(int & offset, QJsonObject & response, QUrl & /*url*/) { offset = response.value(offsetKey()).toInt(); }

    inline QJsonObject & extractBody(QJsonObject & response) { return (response = response.value(QStringLiteral("response")).toObject()); }
    inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("finished")).toBool(); }
    bool extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message);

    QUrl buildUrl(QUrl tUrl, int offset, int limit);
    bool captchaProcessing(QJsonObject & response, QUrl & url);
//    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }

private:   
    inline VkApi(QObject * parent, QJsonObject hash) : WebApi(parent), TeuAuth(), VkRequestApi() {
        fromJson(hash);
        connect(this, SIGNAL(showCaptcha()), this, SLOT(showingCaptcha()), Qt::BlockingQueuedConnection);
    }

    inline VkApi() : WebApi(), TeuAuth() {}

    static VkApi * self;
};

#endif // VK_API_H
