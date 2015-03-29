#include "soundcloud_api.h"

SoundcloudApi *SoundcloudApi::self = 0;

SoundcloudApi *SoundcloudApi::instance() {
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

QString SoundcloudApi::getClientId() const {
    return "8f84790a84f5a5acd1c92e850b5a91b7";
}

void SoundcloudApi::fromJson(QJsonObject hash) {
    TeuAuth::fromJson(hash);
    WebApi::fromJson(hash);
}
QJsonObject SoundcloudApi::toJson() {
    QJsonObject root;

    root = TeuAuth::toJson(root);
    root = WebApi::toJson(root);

    return root;
}

bool SoundcloudApi::isConnected() {
    return !getToken().isEmpty();
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
QString SoundcloudApi::authUrl() const {
    QUrl url("https://soundcloud.com/connect");
    QUrlQuery queryParams = QUrlQuery();
    queryParams.addQueryItem("client_id", "8f84790a84f5a5acd1c92e850b5a91b7");
//    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
    queryParams.addQueryItem("response_type", "code");
    queryParams.addQueryItem("scope", "non-expiring");
    queryParams.addQueryItem("redirect_uri", "http://sos.com");
    queryParams.addQueryItem("display", "popup");

    url.setQuery(queryParams);
    return url.toString();
}

QUrl SoundcloudApi::authTokenUrl() const {
    QUrl url("https://api.soundcloud.com/oauth2/token");
    return url;
}

QByteArray SoundcloudApi::authTokenUrlParams(QString code) const {
    QUrlQuery queryParams = QUrlQuery();

    queryParams.addQueryItem("client_id", getClientId());
    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
    queryParams.addQueryItem("grant_type", "authorization_code");
    queryParams.addQueryItem("redirect_uri", "http://sos.com");
    queryParams.addQueryItem("code", code);

    return queryParams.toString(QUrl::FullyEncoded).toUtf8();
}


//QString SoundcloudApi::authTokenUrl() const {
//    QUrl url("https://api.soundcloud.com/oauth2/token");
//    QUrlQuery queryParams = QUrlQuery();


//    queryParams.addQueryItem("client_id", "8f84790a84f5a5acd1c92e850b5a91b7");
//    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
//    queryParams.addQueryItem("scope", "non-expiring");

////    queryParams.addQueryItem("grant_type", "password");
////    queryParams.addQueryItem("username", "USERNAME");
////    queryParams.addQueryItem("password", "PASSWORD");

//    url.setQuery(queryParams);
//    return url.toString();
//}

QString SoundcloudApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.query());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        return "reject";
    } else if (query.hasQueryItem("code")) {
        QNetworkRequest request(authTokenUrl());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QNetworkReply * m_http = manager() -> post(request, authTokenUrlParams(query.queryItemValue("code")));
        syncRequest(m_http);

        QJsonObject doc = responseToJson(m_http -> readAll());

        if (doc.contains("access_token")) {
            QString newToken = doc.value("access_token").toString();

            QNetworkRequest request("https://api.soundcloud.com/me.json?oauth_token=" + newToken);
            m_http = manager() -> get(request);
            syncRequest(m_http);

            doc = responseToJson(m_http -> readAll());

            setParams(newToken, QString::number(doc.value("id").toInt()), "");

            return "accept";
        }

        return "reject";
    }

    return "";
}

///////////////////////////////////////////////////////////
/// AUDIO LIST
///////////////////////////////////////////////////////////
//INFO: all requests return max 50 items

void SoundcloudApi::getGroupInfo(FuncContainer func, QString uid) {
//    uid = "101";
    QJsonObject res;

    QUrlQuery query = commonMethodParams();
    query.addQueryItem("types", "original,remix,live,podcast");

    QUrl url(getAPIUrl() + "groups/" + uid + "/tracks.json");
    url.setQuery(query);

    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    syncRequest(m_http);

    QByteArray ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    res.insert("audio_list", responseToJson(ar).value("response").toArray());
    delete m_http;

    //////////////////////////////////////////////////////////////

    url.setUrl(getAPIUrl() + "groups/" + uid + "/playlists.json");
    query = commonMethodParams();
    url.setQuery(query);

    m_http = manager() -> get(QNetworkRequest(url));
    syncRequest(m_http);

    ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    res.insert("playlists", responseToJson(ar).value("response").toArray());
    delete m_http;

    connect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
    emit audioListReceived(res);
    disconnect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
}

void SoundcloudApi::getUidInfo(FuncContainer func, QString uid) {
    if (uid[0] == '-') {
        return getGroupInfo(func, uid.mid(1));
    }

    uid = uid == "0" ? getUserID() : uid; // 183
    QJsonObject res;

    QUrlQuery query = commonMethodParams();
    query.addQueryItem("types", "original,remix,live,podcast");
//    “original”
//    “remix”
//    “live”
//    “recording”
//    “spoken”
//    “podcast”
//    “demo”
//    “in progress”
//    “stem”
//    “loop”
//    “sound effect”
//    “sample”
//    “other”


    QUrl url(getAPIUrl() + "users/" + uid + "/tracks.json");
    url.setQuery(query);

    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
    syncRequest(m_http);

    QByteArray ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    res.insert("audio_list", responseToJson(ar).value("response").toArray());
    delete m_http;

    //////////////////////////////////////////////////////////////

    url.setUrl(getAPIUrl() + "users/" + uid + "/playlists.json");
    query = commonMethodParams();
    url.setQuery(query);

    m_http = manager() -> get(QNetworkRequest(url));
    syncRequest(m_http);

    ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    res.insert("playlists", responseToJson(ar).value("response").toArray());
    delete m_http;

    //////////////////////////////////////////////////////////////

    if (uid == getUserID()) {
        QThread::sleep(1);
        //////////////////////////////////////////////////////////////
        url.setUrl(getAPIUrl() + "users/" + uid + "/followings.json");
        url.setQuery(query);
        m_http = manager() -> get(QNetworkRequest(url));
        syncRequest(m_http);

        ar = m_http -> readAll();
        ar.prepend("{\"response\":"); ar.append("}");
        res.insert("followings", responseToJson(ar).value("response").toArray());
        delete m_http;

        //////////////////////////////////////////////////////////////

        url.setUrl(getAPIUrl() + "users/" + uid + "/followers.json");
        url.setQuery(query);
        m_http = manager() -> get(QNetworkRequest(url));
        syncRequest(m_http);

        ar = m_http -> readAll();
        ar.prepend("{\"response\":"); ar.append("}");
        res.insert("followers", responseToJson(ar).value("response").toArray());
        delete m_http;

        //////////////////////////////////////////////////////////////
        QThread::sleep(1);
        //////////////////////////////////////////////////////////////

        url.setUrl(getAPIUrl() + "users/" + uid + "/groups.json");
        url.setQuery(query);
        m_http = manager() -> get(QNetworkRequest(url));
        syncRequest(m_http);

        ar = m_http -> readAll();
        ar.prepend("{\"response\":"); ar.append("}");
        res.insert("groups", responseToJson(ar).value("response").toArray());
        delete m_http;

        //////////////////////////////////////////////////////////////
    }

    connect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
    emit audioListReceived(res);
    disconnect(this, SIGNAL(audioListReceived(QJsonObject &)), func.obj, func.slot);
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

QUrlQuery SoundcloudApi::userMethodParams() {
    QUrlQuery query = QUrlQuery();

    query.addQueryItem("oauth_token", getToken());

    return query;
}

QUrlQuery SoundcloudApi::commonMethodParams() {
    QUrlQuery query = QUrlQuery();

    query.addQueryItem("client_id", getClientId());

    return query;
}

QString SoundcloudApi::getAPIUrl() {
    return "https://api.soundcloud.com/";
}

void SoundcloudApi::errorSend(QJsonObject & doc, const QObject * obj) {
    QJsonObject error = doc.value("error").toObject();
    int err_code = error.value("error_code").toInt();
    QString err_msg = error.value("error_msg").toString();

    connect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
    emit errorReceived(err_code, err_msg);
    disconnect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
}
