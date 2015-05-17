#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "soundcloud_api_private.h"

class SoundcloudApi : public WebApi, public TeuAuth {
    Q_OBJECT
public:
    inline QString name() const { return "soundcloud"; }

    inline QString authUrl() const { return SoundcloudApiPrivate::authUrl(); }
    inline QUrl authTokenUrl() const { return QUrl("https://api.soundcloud.com/oauth2/token"); }

    QByteArray authTokenUrlParams(QString code) const;
    QString proceedAuthResponse(const QUrl & url);

    void getGroupInfo(const QObject * receiver, const char * respSlot, QString uid);
    void getUidInfo(const QObject * receiver, const char * respSlot, QString uid = "0");
    void search(const QObject * receiver, const char * respSlot, QString predicate = QString(), QString style = QString());

    ~SoundcloudApi() { }

    static SoundcloudApi * instance();
    static SoundcloudApi * instance(QJsonObject obj);
    inline static void close() { delete self; }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    inline bool isConnected() { return !getToken().isEmpty(); }

protected:
    ApiFuncContainer * getGroupInfoRoutine(ApiFuncContainer * func);
    ApiFuncContainer * getUidInfoRoutine(ApiFuncContainer * func);
    ApiFuncContainer * searchRoutine(ApiFuncContainer * func, QString & predicate, QString & style);

    bool responseRoutine(QString fieldName, QNetworkReply * reply, ApiFuncContainer * func);
    void errorSend(QJsonObject & doc, const QObject * obj);
private:
    inline SoundcloudApi(QJsonObject hash) : WebApi(), TeuAuth() {
        fromJson(hash);
    }

    inline SoundcloudApi() : WebApi(), TeuAuth() { }

    static SoundcloudApi * self;
};

#endif // SOUNDCLOUD_API_H
