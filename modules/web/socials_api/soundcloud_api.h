#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "soundcloud_request_api.h"

const QString sc_audio_list_key = QStringLiteral("audio_list");
const QString sc_playlist_key = QStringLiteral("playlists");
const QString sc_followings_key = QStringLiteral("followings");
const QString sc_followers_key = QStringLiteral("followers");
const QString sc_groups_key = QStringLiteral("groups");

class SoundcloudApi : public WebApi, public TeuAuth, public SoundcloudRequestApi {
    Q_OBJECT
public:
    static SoundcloudApi * instance();
    static SoundcloudApi * instance(QJsonObject obj);
    inline static void close() { delete self; }

    inline QString name() const { return QStringLiteral("soundcloud"); }
    inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("client_id=8f84790a84f5a5acd1c92e850b5a91b7")); }
    QString authUrl();

    void fromJson(QJsonObject hash);
    QJsonObject toJson();

    inline bool isConnected() { return !token().isEmpty(); }

    void getGroupInfo(QString uid, QJsonObject & object);
    void getUserInfo(QString & uid, QJsonObject & object);

    QJsonObject objectInfo(QString & uid);
    inline void objectInfo(QString & uid, Func func) {
        registerAsync(
            QtConcurrent::run(this, &SoundcloudApi::objectInfo, uid), func
        );
    }
public slots:
    inline void disconnect() { WebApi::disconnect(); setParams("", "", ""); }
    void proceedAuthResponse(const QUrl & url);

protected:
    inline QString baseUrlStr(QString & predicate) { return "https://api.soundcloud.com/" % predicate % ".json"; }

    inline QString offsetKey() const { return QStringLiteral("offset"); }
    inline QString limitKey() const { return QStringLiteral("limit"); }
    inline int requestLimit() const { return 200; }

    inline QJsonObject & extractBody(QJsonObject & response) { return response; }
    inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("response")).toArray().isEmpty(); }
    inline bool extractStatus(QUrl & /*url*/, QJsonObject & response, int & code, QString & message) {
        QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
        message = stat_obj.value(QStringLiteral("error_message")).toString();
        return (code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0;
    }
private:
    inline SoundcloudApi(QJsonObject hash) : WebApi(), TeuAuth() { fromJson(hash); }
    inline SoundcloudApi() : WebApi(), TeuAuth() { }
    inline virtual ~SoundcloudApi() {}

    static SoundcloudApi * self;
};

#endif // SOUNDCLOUD_API_H
