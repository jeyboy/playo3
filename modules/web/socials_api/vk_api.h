#ifndef VK_API_H
#define VK_API_H

#include "../web_api.h"
//#include "../func_container.h"
#include "../auth_chemas/teu_auth.h"
#include "vk_api_private.h"
//#include "../api_process.h"

class VkApi : public WebApi, public TeuAuth {
    Q_OBJECT
public:
    inline QString name() const { return "vk"; }

    QString authUrl() const;
    QString proceedAuthResponse(const QUrl & url);

    ApiFuncContainer * wallMediaRoutine(ApiFuncContainer * func, int offset, int count);
    void wallMediaList(ApiFuncContainer responseSlot, QString uid = "0", int offset = 0, int count = 0);

    ApiFuncContainer * audioAlbumsRoutine(ApiFuncContainer * func, int offset = 0);
    void audioAlbums(ApiFuncContainer responseSlot, QString uid);

    ApiFuncContainer * audioListRoutine(ApiFuncContainer * func);
    void audioList(FuncContainer responseSlot, QString uid);

    void refreshAudioList(ApiFuncContainer slot, QList<QString> uids);

    ~VkApi() {
    }

    static VkApi * instance();
    static VkApi * instance(QJsonObject obj);
    static void close() {
        delete self;
    }

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    bool isConnected();

signals:
    void audioListReceived(QJsonObject &);
    void audioListUpdate(QJsonObject &, QList<QString> &);
    void errorReceived(int, QString);
    void showCaptcha();

protected:
    bool responseRoutine(QNetworkReply * reply, ApiFuncContainer func, QJsonObject & doc);
    bool errorSend(QJsonObject & doc, ApiFuncContainer func, QUrl url);
    bool captchaProcessing(QJsonObject & error, ApiFuncContainer func, QUrl url);

private:   
    VkApi(QJsonObject hash) : WebApi(), TeuAuth() {
        fromJson(hash);
        connect(this, SIGNAL(showCaptcha()), this, SLOT(showingCaptcha()), Qt::BlockingQueuedConnection);
    }

    VkApi() : WebApi(), TeuAuth() {}

    static VkApi * self;

//    QHash<QNetworkReply *, FuncContainer> responses;
};

#endif // VK_API_H
