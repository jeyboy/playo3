#ifndef VK_API_H
#define VK_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "vk_api_private.h"
#include "media/genres/web/vk_genres.h"
#include "modules/data_struct/search/search_settings.h"

class VkApi : public WebApi, public TeuAuth {
    Q_OBJECT
public:
    inline QString name() const { return "vk"; }

    inline QString authUrl() const { return VkApiPrivate::authUrl(); }

    ApiFuncContainer * wallMediaRoutine(ApiFuncContainer * func, int offset, int count);
    void wallMediaList(const QObject * receiver, const char * respSlot, QString uid = "0", int offset = 0, int count = 0);

    ApiFuncContainer * audioAlbumsRoutine(ApiFuncContainer * func, int offset = 0);
    void audioAlbums(const QObject * receiver, const char * respSlot, QString uid);

    ApiFuncContainer * audioListRoutine(ApiFuncContainer * func);
    void audioList(const QObject * receiver, const char * respSlot, QString uid);

    ApiFuncContainer * audioRecomendationRoutine(ApiFuncContainer * func, bool byUser, bool randomize);
    void audioRecomendation(const QObject * receiver, const char * respSlot, QString uid, bool byUser, bool randomize);

    ApiFuncContainer * searchAudioRoutine(ApiFuncContainer * func, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular);
    void audioSearch(const QObject * receiver, const char * respSlot, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular);
    QJsonObject audioSearchSync(const QObject * receiver, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular);
    QJsonObject audioSearchSync(const QObject * receiver, QString predicate, int limitation = 1);

    ApiFuncContainer * audioPopularRoutine(ApiFuncContainer * func, bool onlyEng, int genreId);
    void audioPopular(const QObject * receiver, const char * respSlot, bool onlyEng, int genreId = -1);
    QJsonObject audioPopularSync(const QObject * receiver, bool onlyEng, int genreId = -1);

    QString refreshAudioItemUrl(QString audio_uid);
    QJsonObject getAudioInfo(QString audio_uid);
    QJsonObject getAudiosInfo(QStringList audio_uid);

//    void refreshAudioList(const QObject * receiver, const char * respSlot, QList<QString> uids);

    inline ~VkApi() {}

    static VkApi * instance();
    static VkApi * instance(QObject * parent, QJsonObject obj);
    inline static void close() { delete self; }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    bool isConnected();

signals:
    void showCaptcha();
public slots:
    void proceedAuthResponse(const QUrl & url);
protected:
    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }
    bool responseRoutine(QNetworkReply * reply, ApiFuncContainer * func, QJsonObject & doc);
    bool errorSend(QJsonObject & doc, ApiFuncContainer * func, QUrl url);
    bool captchaProcessing(QJsonObject & error, ApiFuncContainer * func, QUrl url);

private:   
    inline VkApi(QObject * parent, QJsonObject hash) : WebApi(parent), TeuAuth() {
        fromJson(hash);
        connect(this, SIGNAL(showCaptcha()), this, SLOT(showingCaptcha()), Qt::BlockingQueuedConnection);
    }

    inline VkApi() : WebApi(), TeuAuth() {}

    static VkApi * self;
};

#endif // VK_API_H
