#include "od_api.h"

//https://apiok.ru/wiki/pages/viewpage.action?pageId=81822117
//https://apiok.ru/wiki/pages/viewpage.action?pageId=75989046

namespace Od {
    Api * Api::self = 0;

    Api * Api::instance() {
        if(!self)
            self = new Api();
        return self;
    }

    Api * Api::instance(QObject * parent, const QJsonObject & obj) {
        if(!self)
            self = new Api(parent, obj);
        else
            Api::instance() -> fromJson(obj);
        return self;
    }

    //https://connect.ok.ru/oauth/authorize?client_id={clientId}&scope={scope}&response_type={responseType}&redirect_uri={redirectUri}&layout={layout
    QString Api::authUrl() {
        QUrl url(QStringLiteral("https://connect.ok.ru/oauth/authorize"));

        QUrlQuery query = genDefaultParams();

        setParam(query, QStringLiteral("cliend_id"), hash_key/*QStringLiteral("1152123904")*/);
        setParam(query, QStringLiteral("response_type"), QStringLiteral("token")); // code
        setParam(query, QStringLiteral("scope"), QStringLiteral("VALUABLE_ACCESS"));
        setParam(query, QStringLiteral("redirect_uri"), QStringLiteral("http://sos.com"));
        setParam(query, QStringLiteral("layout"), QStringLiteral("a"));

        url.setQuery(query);
        return url.toString();
    }

    void Api::fromJson(QJsonObject hash) {
        TeuAuth::fromJson(hash);
        WebApi::fromJson(hash);
        hash_key = hash.value(HASH_KEY).toString(QStringLiteral("Umlm81sLBth1zoe4Gvr91su9jMGy8-9YHxVHKKT2mVev577x2yILuVz1rAETfg1kKu5H6kkjmX1umDYLjK0X6t9FFtKWE8FbHqjd3DFIZp9ZcPRGsRTamryfuTHAbFpoa8-fzj08H0XtkftqWJQt-2J6QNHyMPdYyiIzeoMjGupkLxdRFYTvDS6xUjZQRF9WdVe7Cb7_yNyuOThSK775Z6wwK5yrEN-cF8yfzugRquI6oAUberHcry2T_nuc9w2m"));
    }
    QJsonObject Api::toJson() {
        QJsonObject root;

        TeuAuth::toJson(root);
        WebApi::toJson(root);

        root.insert(HASH_KEY, hash_key);

        return root;
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
    void Api::proceedAuthResponse(const QUrl & /*url*/) {
//        QUrlQuery query(url.query());

//        if (query.hasQueryItem(QStringLiteral("error"))) {
//            error = query.queryItemValue(QStringLiteral("error_description"));
//            emit responseReady(QStringLiteral("reject"));
//        } else if (query.hasQueryItem(QStringLiteral("code"))) {
//            QNetworkRequest request(authTokenUrl());
//            request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
//            QJsonObject doc = WebManager::manager() -> postJson(request, authTokenUrlParams(query.queryItemValue(QStringLiteral("code"))));

//            if (doc.contains(QStringLiteral("access_token"))) {
//                QString newToken = doc.value(QStringLiteral("access_token")).toString();
//                doc = WebManager::manager() -> getJson(confirmAuthUrl(newToken));

//                setParams(newToken, QString::number(doc.value(QStringLiteral("id")).toInt()), QString());
//                emit authorized();
//                emit responseReady(QStringLiteral("accept"));
//            }
//            else emit responseReady(QStringLiteral("reject"));
//        }
//        else emit responseReady("");
    }
}
