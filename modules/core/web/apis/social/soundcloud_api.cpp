#include "soundcloud_api.h"

using namespace Core::Web::Soundcloud;

QString Api::authUrl() {
    QUrl url(QStringLiteral("https://soundcloud.com/connect"));

    QUrlQuery query = genDefaultParams();
    setParam(query, QStringLiteral("response_type"), QStringLiteral("code"));
    setParam(query, QStringLiteral("scope"), QStringLiteral("non-expiring"));
    setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("http://sos.com"));
    setParam(query, QStringLiteral("display"), QStringLiteral("popup"));

    url.setQuery(query);
    return url.toString();
}

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();
    TeuAuth::fromJson(obj);
    WebApi::fromJson(obj);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    TeuAuth::toJson(root);
    WebApi::toJson(root);

    hash.insert(name(), root);
}

//////////////////////////////////////////////////////////
/// COMMON
//////////////////////////////////////////////////////////

void Api::getGroupInfo(QString uid, QJsonObject & object) {
//    uid = "101";
    object.insert(Soundcloud::audio_list_key, groupAudio(uid));
    object.insert(Soundcloud::playlist_key, groupPlaylists(uid));
}

void Api::getUserInfo(QString & uid, QJsonObject & object) {
    object.insert(Soundcloud::audio_list_key, userAudio(uid));
    object.insert(Soundcloud::playlist_key, userPlaylists(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(Soundcloud::followings_key, userFollowings(uid)); // return bad request error
    object.insert(Soundcloud::followers_key, userFollowers(uid));
    QThread::msleep(REQUEST_DELAY);
    object.insert(Soundcloud::groups_key, userGroups(uid));
}


QJsonObject Api::objectInfo(QString & uid) {
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
void Api::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.query());

    if (query.hasQueryItem(QStringLiteral("error"))) {
        error = query.queryItemValue(QStringLiteral("error_description"));
        emit responseReady(QStringLiteral("reject"));
    } else if (query.hasQueryItem(QStringLiteral("code"))) {
        QJsonObject doc = Web::Manager::prepare() -> followedForm(authTokenUrl(), authTokenUrlParams(query.queryItemValue(QStringLiteral("code")))) -> toJson();

        if (doc.contains(QStringLiteral("access_token"))) {
            QString newToken = doc.value(QStringLiteral("access_token")).toString();
            doc = Web::Manager::prepare() -> getJson(confirmAuthUrl(newToken));

            setParams(newToken, QString::number(doc.value(QStringLiteral("id")).toInt()), QString());
            emit authorized();
            emit responseReady(QStringLiteral("accept"));
        }
        else emit responseReady(QStringLiteral("reject"));
    }
    else emit responseReady("");
}
