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
    QUrl url("https://soundcloud.com/connect");

    QUrlQuery query = genDefaultParams();
    setParam(query, "response_type", "code");
    setParam(query, "scope", "non-expiring");
    setParam(query, "redirect_uri", "http://sos.com");
    setParam(query, "display", "popup");

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
    CustomNetworkAccessManager * manager;
    bool isNew = CustomNetworkAccessManager::validManager(manager);

    object.insert("audio_list", groupAudio(uid, manager));
    object.insert("playlists", groupPlaylists(uid, manager));

    if (isNew) delete manager;
}

void SoundcloudApi::getUserInfo(QString uid, QJsonObject & object) {
    CustomNetworkAccessManager * manager;
    bool isNew = CustomNetworkAccessManager::validManager(manager);

    object.insert("audio_list", userAudio(uid, manager));
    object.insert("playlists", userPlaylists(uid, manager));
    QThread::msleep(REQUEST_DELAY);
    object.insert("followings", userFollowings(uid, manager));
    object.insert("followers", userFollowers(uid, manager));
    QThread::msleep(REQUEST_DELAY);
    object.insert("groups", userGroups(uid, manager));

    if (isNew) delete manager;
}


QJsonObject SoundcloudApi::objectInfo(QString uid) {
    QJsonObject res;

    if (uid[0] == '-')
        getGroupInfo(uid.mid(1), res);
    else
        getUserInfo(uid, res);

    return res;
}

void SoundcloudApi::objectInfo(QString & uid, Func func) {
    registerAsync(QtConcurrent::run(this, &SoundcloudApi::objectInfo, uid), func);
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
void SoundcloudApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.query());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        emit responseReady("reject");
    } else if (query.hasQueryItem("code")) {
        QNetworkRequest request(authTokenUrl());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QJsonObject doc = CustomNetworkAccessManager::manager() -> postToJson(request, authTokenUrlParams(query.queryItemValue("code")));

        if (doc.contains("access_token")) {
            QString newToken = doc.value("access_token").toString();

            QNetworkRequest request(confirmAuthUrl(newToken));
            doc = CustomNetworkAccessManager::manager() -> getToJson(request);

            setParams(newToken, QString::number(doc.value("id").toInt()), "");
            emit authorized();
            emit responseReady("accept");
        }
        else emit responseReady("reject");
    }
    else emit responseReady("");
}
