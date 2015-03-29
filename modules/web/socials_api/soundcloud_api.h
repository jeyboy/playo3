//#ifndef SOUNDCLOUD_API_H
//#define SOUNDCLOUD_API_H

//#include "web/web_api.h"
//#include "misc/func_container.h"
//#include "model/model_item.h"
//#include "web/auth_chemas/teu_auth.h"

//class SoundcloudApi : public WebApi, TeuAuth {
//    Q_OBJECT
//public:
//    inline QString name() const { return "soundcloud"; }

//    QString authUrl() const;
//    QUrl authTokenUrl() const;
//    QByteArray authTokenUrlParams(QString code) const;
//    QString proceedAuthResponse(const QUrl & url);

//    QString getClientId() const;

//    void getGroupInfo(FuncContainer func, QString uid);
//    void getUidInfo(FuncContainer responseSlot, QString uid = "0");

//    ~SoundcloudApi() { }

//    static SoundcloudApi * instance();
//    static SoundcloudApi * instance(QJsonObject obj);
//    static void close() {
//        delete self;
//    }

//    void fromJson(QJsonObject hash);
//    QJsonObject toJson();

//    bool isConnected();

//    QUrlQuery userMethodParams();
//    QUrlQuery commonMethodParams();

//signals:
//    void audioListReceived(QJsonObject &);
//    void errorReceived(int, QString &);

//protected:
//    QString getAPIUrl();
//    void errorSend(QJsonObject & doc, const QObject * obj);

////protected slots:
////    void audioListRequest();
//private:
//    SoundcloudApi(QJsonObject hash) : WebApi(), TeuAuth() {
//        fromJson(hash);
//    }

//    SoundcloudApi() : WebApi(), TeuAuth() { }

//    static SoundcloudApi *self;

//    QHash<QNetworkReply *, FuncContainer> responses;
//};

//#endif // SOUNDCLOUD_API_H
