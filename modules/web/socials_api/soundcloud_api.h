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

    void getGroupInfo(const QObject * receiver, const char * respSlot, QString uid);
    void getUidInfo(const QObject * receiver, const char * respSlot, QString uid = "0");
    QJsonObject getAudioInfo(QString audio_uid);
    QJsonObject getAudiosInfo(QStringList audio_uids);

    void searchAudio(const QObject * receiver, const char * respSlot, QString predicate = QString(), QString genre = QString(), bool popular = true);
    QJsonObject searchAudioSync(const QObject * receiver, QString predicate = QString(), QString genre = QString(), bool popular = true);

    ~SoundcloudApi() { }

    static SoundcloudApi * instance();
    static SoundcloudApi * instance(QJsonObject obj);
    inline static void close() { delete self; }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    inline bool isConnected() { return !getToken().isEmpty(); }   

public slots:
    void proceedAuthResponse(const QUrl & url);

protected:
    ApiFuncContainer * getGroupInfoRoutine(ApiFuncContainer * func);
    ApiFuncContainer * getUidInfoRoutine(ApiFuncContainer * func);
    ApiFuncContainer * searchAudioRoutine(ApiFuncContainer * func, QString & predicate, QString & genre, bool popular);

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
