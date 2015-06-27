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
    inline QString name() const { return "vk"; }
    inline QUrlQuery genDefaultParams() {
        QUrlQuery query = QUrlQuery();

        query.addQueryItem("v", apiVersion());
        query.addQueryItem("access_token", token);
        query.addQueryItem("test_mode", "1");

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

signals:
    void showCaptcha();
public slots:
    void proceedAuthResponse(const QUrl & url);
protected:
    inline QString baseUrlStr(QString & predicate) { return "https://api.vk.com/method/" + predicate; }

    inline QString offsetKey() const { return "offset"; }
    inline QString limitKey() const { return "count"; }
//    inline int requestLimit() const { return 200; }

//    inline QJsonObject & extractBody(QJsonObject & response) { return response; }
//    inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value("response").toArray().isEmpty(); }
    inline bool extractStatus(QJsonObject & response, int & code, QString & message) {
        QJsonObject stat_obj = response.value("error").toObject();
        message = stat_obj.value("error_msg").toString();
        return (code = stat_obj.value("error_code").toInt()) == 0;
    }

    QUrl buildUrl(QUrl tUrl, int offset, int limit) {
        QString urlStr = tUrl.toString();
        urlStr.replace("%%1", QString::number(offset)).replace("%%2", QString::number(limit));
        return QUrl(urlStr);
    }


//    bool captchaProcessing(QJsonObject & error, ApiFunc * func, QUrl url);
//    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }

private:   
    inline VkApi(QObject * parent, QJsonObject hash) : WebApi(parent), TeuAuth() {
        fromJson(hash);
        connect(this, SIGNAL(showCaptcha()), this, SLOT(showingCaptcha()), Qt::BlockingQueuedConnection);
    }

    inline VkApi() : WebApi(), TeuAuth() {}

    static VkApi * self;
};

#endif // VK_API_H
