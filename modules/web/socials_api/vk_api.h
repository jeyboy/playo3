#ifndef VK_API_H
#define VK_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "vk_api_private.h"
#include "media/genres/web/vk_genres.h"

class VkApi : public WebApi, public TeuAuth {
    Q_OBJECT
public:
    inline QString name() const { return "vk"; }

    QString authUrl() const;
    QString proceedAuthResponse(const QUrl & url);

    ApiFuncContainer * wallMediaRoutine(ApiFuncContainer * func, int offset, int count);
    void wallMediaList(const QObject * receiver, const char * respSlot, QString uid = "0", int offset = 0, int count = 0);

    ApiFuncContainer * audioAlbumsRoutine(ApiFuncContainer * func, int offset = 0);
    void audioAlbums(const QObject * receiver, const char * respSlot, QString uid);

    ApiFuncContainer * audioListRoutine(ApiFuncContainer * func);
    void audioList(const QObject * receiver, const char * respSlot, QString uid);

    void refreshAudioList(const QObject * receiver, const char * respSlot, QList<QString> uids);

    inline ~VkApi() {}

    static VkApi * instance();
    static VkApi * instance(QObject * parent, QJsonObject obj);
    inline static void close() { delete self; }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    bool isConnected();

signals:
    void audioListReceived(QJsonObject &);
    void audioListUpdate(QJsonObject &, QList<QString> &);
    void errorReceived(int, QString);
    void showCaptcha();

protected:
    inline QString adapteUid(QString & uid) { return uid == "0" ? getUserID() : uid; }
    bool responseRoutine(QNetworkReply * reply, ApiFuncContainer * func, QJsonObject & doc);
    bool errorSend(QJsonObject & doc, ApiFuncContainer * func, QUrl url);
    bool captchaProcessing(QJsonObject & error, ApiFuncContainer * func, QUrl url);

private:   
    VkApi(QObject * parent, QJsonObject hash) : WebApi(parent), TeuAuth() {
        fromJson(hash);
        connect(this, SIGNAL(showCaptcha()), this, SLOT(showingCaptcha()), Qt::BlockingQueuedConnection);
    }

    VkApi() : WebApi(), TeuAuth() {}

    static VkApi * self;
};

#endif // VK_API_H
