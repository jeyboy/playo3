#include "soundcloud_api.h"

SoundcloudApi * SoundcloudApi::self = 0;

SoundcloudApi * SoundcloudApi::instance() {
    if(!self)
        self = new SoundcloudApi();
    return self;
}

SoundcloudApi * SoundcloudApi::instance(QJsonObject obj) {
    if(!self)
        self = new SoundcloudApi(obj);
    else
        SoundcloudApi::instance() -> fromJson(obj);
    return self;
}

QString SoundcloudApi::authUrl() {
    QUrl url(QStringLiteral("https://soundcloud.com/connect"));

    QUrlQuery query = genDefaultParams();
    setParam(query, QStringLiteral("response_type"), QStringLiteral("code"));
    setParam(query, QStringLiteral("scope"), QStringLiteral("non-expiring"));
    setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("http://sos.com"));
    setParam(query, QStringLiteral("display"), QStringLiteral("popup"));

    url.setQuery(query);
    return url.toString();
}

void SoundcloudApi::fromJson(QJsonObject hash) {
    TeuAuth::fromJson(hash);
    WebApi::fromJson(hash);
}
QJsonObject SoundcloudApi::toJson() {
    QJsonObject root;

    TeuAuth::toJson(root);
    WebApi::toJson(root);

    return root;
}

//////////////////////////////////////////////////////////
/// COMMON
//////////////////////////////////////////////////////////

void SoundcloudApi::getGroupInfo(QString uid, QJsonObject & object) {
//    uid = "101";
    WebManager * manager;
    bool isNew = WebManager::valid(manager);

    object.insert(sc_audio_list_key, groupAudio(uid, manager));
    object.insert(sc_playlist_key, groupPlaylists(uid, manager));

    if (isNew) delete manager;
}

void SoundcloudApi::getUserInfo(QString & uid, QJsonObject & object) {
    WebManager * manager;
    bool isNew = WebManager::valid(manager);

    object.insert(sc_audio_list_key, userAudio(uid, manager));
    object.insert(sc_playlist_key, userPlaylists(uid, manager));
    QThread::msleep(REQUEST_DELAY);
    object.insert(sc_followings_key, userFollowings(uid, manager));
    object.insert(sc_followers_key, userFollowers(uid, manager));
    QThread::msleep(REQUEST_DELAY);
    object.insert(sc_groups_key, userGroups(uid, manager));

    if (isNew) delete manager;
}


QJsonObject SoundcloudApi::objectInfo(QString & uid) {
    QJsonObject res;

    if (uid[0] == '-')
        getGroupInfo(uid.mid(1), res);
    else
        getUserInfo(uid, res);

    return res;
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
void SoundcloudApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.query());

    if (query.hasQueryItem(QStringLiteral("error"))) {
        error = query.queryItemValue(QStringLiteral("error_description"));
        emit responseReady(QStringLiteral("reject"));
    } else if (query.hasQueryItem(QStringLiteral("code"))) {
        QNetworkRequest request(authTokenUrl());
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
        QJsonObject doc = WebManager::stock() -> postToJson(request, authTokenUrlParams(query.queryItemValue(QStringLiteral("code"))));

        if (doc.contains(QStringLiteral("access_token"))) {
            QString newToken = doc.value(QStringLiteral("access_token")).toString();

            QNetworkRequest request(confirmAuthUrl(newToken));
            doc = WebManager::stock() -> getToJson(request);

            setParams(newToken, QString::number(doc.value(QStringLiteral("id")).toInt()), "");
            emit authorized();
            emit responseReady(QStringLiteral("accept"));
        }
        else emit responseReady(QStringLiteral("reject"));
    }
    else emit responseReady("");
}
