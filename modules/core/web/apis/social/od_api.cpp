#include "od_api.h"

//https://apiok.ru/wiki/pages/viewpage.action?pageId=81822117
//https://apiok.ru/wiki/pages/viewpage.action?pageId=75989046

using namespace Core::Web::Od;

//https://connect.ok.ru/oauth/authorize?client_id={clientId}&scope={scope}&response_type={responseType}&redirect_uri={redirectUri}&layout={layout
QString Api::authUrl() {
    QUrl url(QStringLiteral("https://connect.ok.ru/oauth/authorize"));

    QUrlQuery query = genDefaultParams();

    setParam(query, QStringLiteral("cliend_id"), token()/*QStringLiteral("1152123904")*/);
    setParam(query, QStringLiteral("response_type"), QStringLiteral("token")); // code
    setParam(query, QStringLiteral("scope"), QStringLiteral("VALUABLE_ACCESS"));
    setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("http://sos.com"));
    setParam(query, QStringLiteral("layout"), QStringLiteral("a"));

    url.setQuery(query);
    return url.toString();
}

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();

    TeuAuth::fromJson(obj);
    WebApi::fromJson(obj);

    if (!sessionIsValid())
        connection(true);
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

//    void Api::getGroupInfo(QString uid, QJsonObject & object) {
//    //    uid = "101";
//        object.insert(Soundcloud::audio_list_key, groupAudio(uid));
//        object.insert(Soundcloud::playlist_key, groupPlaylists(uid));
//    }

//    void Api::getUserInfo(QString & uid, QJsonObject & object) {
//        object.insert(Soundcloud::audio_list_key, userAudio(uid));
//        object.insert(Soundcloud::playlist_key, userPlaylists(uid));
//        QThread::msleep(REQUEST_DELAY);
//        object.insert(Soundcloud::followings_key, userFollowings(uid));
//        object.insert(Soundcloud::followers_key, userFollowers(uid));
//        QThread::msleep(REQUEST_DELAY);
//        object.insert(Soundcloud::groups_key, userGroups(uid));
//    }


//    QJsonObject Api::objectInfo(QString & uid) {
//        QJsonObject res;

//        if (uid[0] == '-')
//            getGroupInfo(uid.mid(1), res);
//        else
//            getUserInfo(uid, res);

//        return res;
//    }

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
bool Api::hashConnection(bool onlyAuto) {
    QNetworkReply * reply = Manager::prepare() -> followedGet(initUrl(), initHeaders());
    qDebug() << "!!!!!!!" << reply -> readAll();
    reply -> deleteLater();

    Manager::printCookies();

    if (!sessionIsValid())
        if (!onlyAuto && !formConnection())
            return false;

    setParams(grabSID(), userID(), QString());
    return true;
}

bool Api::formConnection() {
    QString err;

    while(true) {
        if (!showingLogin(QStringLiteral("Odnoklassniki auth"), authE, authP, err)) return false;

        Response * reply = Manager::prepare() -> unfollowedForm(authRequestUrl(), initHeaders());
        QUrl url = reply -> toRedirectUrl();
        QString hash_key = Manager::paramVal(url, QStringLiteral("httpsdata"));

        reply = Manager::prepare() -> followedGet(url, initHeaders());
        err = reply -> paramVal(QStringLiteral("st.error"));
        if (!err.isEmpty()) {
            reply -> deleteLater();
            continue;
        }

        Html::Document doc = reply -> toHtml();
        checkSecurity(doc);

        if (!Manager::cookie(QStringLiteral("AUTHCODE")).isEmpty()) {
            setParams(hash_key, grabUserId(doc), QString());
            break;
        }
        else err = doc.find(".anonym_e").text();
    }

    return true;
}
