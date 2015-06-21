#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "soundcloud_request_api.h"

class SoundcloudApi : public WebApi, public TeuAuth, public SoundcloudRequestApi {
    Q_OBJECT
public:
    inline QString name() const { return "soundcloud"; }

    static SoundcloudApi * instance();
    static SoundcloudApi * instance(QJsonObject obj);
    inline static void close() { delete self; }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    inline bool isConnected() { return !token().isEmpty(); }

public slots:
    void proceedAuthResponse(const QUrl & url);

protected:
    inline void appendParams(QUrlQuery & query) { setParam(query, "client_id", "8f84790a84f5a5acd1c92e850b5a91b7"); }
    inline QString baseUrl(QString predicate) { return "https://api.soundcloud.com/" + predicate + ".json"; }

//    static inline int offsetLimit() { return 1000; /*8000*/; }



    inline int requestLimit() { return 200; }

    inline int extractAmount(QJsonObject & response) {
//        return response.value("response").toObject().value("total").toInt();
    }
    inline void setLimit(QUrlQuery & query, int limit = 99999, int offset = 0) {
        setParam(query, "limit", QString::number(qMin(limit, requestLimit())));
        if (offset > 0) setParam(query, "offset", QString::number(offset));
    }

    bool proceedQuery(QUrl url, QJsonObject & response, QObject * errorReceiver = 0) {
        CustomNetworkAccessManager * manager;
        bool isNew = CustomNetworkAccessManager::validManager(manager);

        response = manager -> getToJson(QNetworkRequest(url));

        if (isNew) delete manager;
//        int status_code = response.value("response").toObject().value("status").toObject().value("code").toInt();
//        bool status = status_code == 0;

//        if (!status) {
//            QString message = response.value("response").toObject().value("status").toObject().value("message").toString();
//            if (errorReceiver)
//                QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, status_code), Q_ARG(QString, message));
//            else qDebug() << message;
//        }

//        return status;
    }
private:
    inline SoundcloudApi(QJsonObject hash) : WebApi(), TeuAuth() { fromJson(hash); }

    inline SoundcloudApi() : WebApi(), TeuAuth() { }

    static SoundcloudApi * self;
};

#endif // SOUNDCLOUD_API_H
