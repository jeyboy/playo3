#ifndef VK_API_H
#define VK_API_H

#include "../web_api.h"
#include "../func_container.h"
//#include "model/model_item.h"
#include "../auth_chemas/teu_auth.h"
#include "vk_api_private.h"
#include "../api_process.h"

class VkApi : public WebApi, public TeuAuth {
    Q_OBJECT
public:
    inline QString name() const { return "vk"; }

    QString authUrl() const;
    QString proceedAuthResponse(const QUrl & url);

    ApiFuncContainer * wallMediaRoutine(ApiFuncContainer * func, int offset, int count);
    void wallMediaList(FuncContainer responseSlot, QString uid = "0", int offset = 0, int count = 0);

    ApiFuncContainer * audioAlbumsRoutine(ApiFuncContainer * func, int offset = 0);
    void audioAlbums(FuncContainer responseSlot, QString uid);

    ApiFuncContainer * audioListRoutine(ApiFuncContainer * func);
    void audioList(FuncContainer responseSlot, QString uid);


    void refreshAudioList(FuncContainer slot, QHash<ModelItem *, QString> uids);

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
    void audioListUpdate(QJsonObject &, QHash<ModelItem *, QString> &);
    void errorReceived(int, QString);
    void showCaptcha();

protected:
    bool responseRoutine(QNetworkReply * reply, FuncContainer func, QJsonObject & doc);
    bool errorSend(QJsonObject & doc, FuncContainer func, QUrl url);
    bool captchaProcessing(QJsonObject & error, FuncContainer func, QUrl url);

//    QUrl getAudioListUrl() const;
//    QUrl getAudioCountUrl() const;
//    QUrl getAudioSearchUrl() const;
//    QUrl getAudioCopyUrl() const;
//    QUrl getAudioRemoveUrl() const;

//    QUrl getAudioAlbumsListUrl() const;
//    QUrl getAudioAlbumAddUrl() const;
//    QUrl getAudioAlbumEditUrl() const;
//    QUrl getAudioAlbumRemoveUrl() const;
//    QUrl getAudioAlbumMoveToUrl() const;

//    QUrl getAudioSaveServerUrl() const;
//    QUrl getAudioSaveUrl() const;

protected slots:
//    void audioListResponse();

//    void audioCountRequest();
//    void audioSearchRequest();
//    void audioCopyRequest();
//    void audioRemoveRequest();

//    void audioSaveServerRequest();
//    void audioSave();

//    void audioAlbumsRequest();
//    void audioAlbumAddRequest();
//    void audioAlbumEditRequest();
//    void audioAlbumRemoveRequest();
//    void audioAlbumMoveToRequest();

private:   
    VkApi(QJsonObject hash) : WebApi(), TeuAuth() {
        fromJson(hash);
        connect(this, SIGNAL(showCaptcha()), ApiProcess::instance(), SLOT(showCaptcha()), Qt::BlockingQueuedConnection);
    }

    VkApi() : WebApi(), TeuAuth() {

    }

    static VkApi *self;

//    QHash<QNetworkReply *, FuncContainer> responses;
//    QHash<QNetworkReply *, QHash<ModelItem *, QString> > collations;
};

#endif // VK_API_H
