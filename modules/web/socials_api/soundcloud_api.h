#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"

class SoundcloudApi : public WebApi, TeuAuth {
    Q_OBJECT
public:
    inline QString name() const { return "soundcloud"; }

    QString authUrl() const;
    inline QUrl authTokenUrl() const { return QUrl("https://api.soundcloud.com/oauth2/token"); }
    QByteArray authTokenUrlParams(QString code) const;
    QString proceedAuthResponse(const QUrl & url);

    QString getClientId() const { return "8f84790a84f5a5acd1c92e850b5a91b7"; }

    void getGroupInfo(const QObject * receiver, const char * respSlot, QString uid);
    void getUidInfo(const QObject * receiver, const char * respSlot, QString uid = "0");

    ~SoundcloudApi() { }

    static SoundcloudApi * instance();
    static SoundcloudApi * instance(QJsonObject obj);
    inline static void close() { delete self; }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    inline bool isConnected() { return !getToken().isEmpty(); }

    QUrlQuery userMethodParams();
    QUrlQuery commonMethodParams();

signals:
    void audioListReceived(QJsonObject &);
    void errorReceived(int, QString &);

protected:
    ApiFuncContainer * getGroupInfoRoutine(ApiFuncContainer * func);
    ApiFuncContainer * getUidInfoRoutine(ApiFuncContainer * func);

    inline QString getAPIUrl() { return "https://api.soundcloud.com/"; }

    void errorSend(QJsonObject & doc, const QObject * obj);
private:
    inline SoundcloudApi(QJsonObject hash) : WebApi(), TeuAuth() {
        fromJson(hash);
    }

    inline SoundcloudApi() : WebApi(), TeuAuth() { }

    static SoundcloudApi * self;
};

#endif // SOUNDCLOUD_API_H
