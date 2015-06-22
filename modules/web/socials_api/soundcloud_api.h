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

    QJsonObject objectInfo(QString uid);
public slots:
    void proceedAuthResponse(const QUrl & url);

protected:
    inline QUrlQuery genDefaultParams() { return QUrlQuery("client_id=8f84790a84f5a5acd1c92e850b5a91b7"); }
    inline QString baseUrlStr(QString & predicate) { return "https://api.soundcloud.com/" + predicate + ".json"; }  

    inline QString offsetKey() { return "offset"; }
    inline QString limitKey() { return "limit"; }
    inline int requestLimit() { return 200; }
    //    inline int offsetLimit() { return 1000; /*8000*/; }

    inline QJsonObject & extractBody(QJsonObject & response) { return response; }
    inline bool endReched(QJsonObject & response, int /*offset*/) { return response.value("response").toArray().isEmpty(); }
    inline void extractStatus(QJsonObject & response, int & code, QString & message) {
        QJsonObject stat_obj = response.value("response").toObject().value("errors").toArray().first().toObject();
        code = stat_obj.value("error_code").toInt();
        message = stat_obj.value("error_message").toString();
    }
private:
    inline SoundcloudApi(QJsonObject hash) : WebApi(), TeuAuth() { fromJson(hash); }
    inline SoundcloudApi() : WebApi(), TeuAuth() { }

    static SoundcloudApi * self;
};

#endif // SOUNDCLOUD_API_H
