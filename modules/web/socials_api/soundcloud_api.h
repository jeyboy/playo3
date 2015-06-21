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
    inline QUrlQuery genDefaultParams() { return QUrlQuery("client_id=8f84790a84f5a5acd1c92e850b5a91b7"); }
    inline QString baseUrlStr(QString & predicate) { return "https://api.soundcloud.com/" + predicate + ".json"; }

    inline QString offsetKey() { return "start"; }
    inline QString limitKey() { return "results"; }
    inline int requestLimit() { return 200; }

//    inline QJsonObject & extractBody(QJsonObject & response) { return (response = response.value("response").toObject()); }
//    inline int extractAmount(QJsonObject & response) { return extractBody(response).value("total").toInt(); }
//    inline void extractStatus(QJsonObject & response, int & code, QString & message) {
//        QJsonObject stat_obj = extractBody(response).value("status").toObject();
//        code = stat_obj.value("code").toInt();
//        message = stat_obj.value("message").toString();
//    }
//    inline int offsetLimit() { return 1000; /*8000*/; }
private:
    inline SoundcloudApi(QJsonObject hash) : WebApi(), TeuAuth() { fromJson(hash); }

    inline SoundcloudApi() : WebApi(), TeuAuth() { }

    static SoundcloudApi * self;
};

#endif // SOUNDCLOUD_API_H
