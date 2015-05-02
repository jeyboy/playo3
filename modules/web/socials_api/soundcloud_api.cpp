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
        QNetworkReply * m_http = manager() -> postSync(request, authTokenUrlParams(query.queryItemValue("code")));

        QJsonObject doc = responseToJson(m_http -> readAll());
        delete m_http;

        if (doc.contains("access_token")) {
            QString newToken = doc.value("access_token").toString();

            QNetworkRequest request("https://api.soundcloud.com/me.json?oauth_token=" + newToken);
            m_http = manager() -> getSync(request);

            doc = responseToJson(m_http -> readAll());
            delete m_http;

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

void SoundcloudApi::getGroupInfo(const QObject * receiver, const char * respSlot, QString uid) {
    startApiCall(QtConcurrent::run(this, &SoundcloudApi::getGroupInfoRoutine, new ApiFuncContainer(receiver, respSlot, uid)));
}

ApiFuncContainer * SoundcloudApi::getGroupInfoRoutine(ApiFuncContainer * func) {
//    uid = "101";
    CustomNetworkAccessManager * netManager = createManager();

    QUrlQuery query = commonMethodParams();
    query.addQueryItem("types", "original,remix,live,podcast");

    QUrl url(getAPIUrl() + "groups/" + func -> uid + "/tracks.json");
    url.setQuery(query);

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));

    QByteArray ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    func -> result.insert("audio_list", responseToJson(ar).value("response").toArray());
    delete m_http;

    //////////////////////////////////////////////////////////////

    url.setUrl(getAPIUrl() + "groups/" + func -> uid + "/playlists.json");
    query = commonMethodParams();
    url.setQuery(query);

    m_http = netManager -> getSync(QNetworkRequest(url));

    ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    func -> result.insert("playlists", responseToJson(ar).value("response").toArray());
    delete m_http;

    delete netManager;

    return func;
}

void SoundcloudApi::getUidInfo(const QObject * receiver, const char * respSlot, QString uid) {
    startApiCall(QtConcurrent::run(this, &SoundcloudApi::getUidInfoRoutine, new ApiFuncContainer(receiver, respSlot, uid)));
}

ApiFuncContainer * SoundcloudApi::getUidInfoRoutine(ApiFuncContainer * func) {
    if (func -> uid[0] == '-') {
        func -> uid = func -> uid.mid(1);
        return getGroupInfoRoutine(func);
    }

    CustomNetworkAccessManager * netManager = createManager();

    func -> uid = func -> uid == "0" ? getUserID() : func -> uid; // 183

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


    QUrl url(getAPIUrl() + "users/" + func -> uid + "/tracks.json");
    url.setQuery(query);

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));

    QByteArray ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    func -> result.insert("audio_list", responseToJson(ar).value("response").toArray());
    delete m_http;

    //////////////////////////////////////////////////////////////

    url.setUrl(getAPIUrl() + "users/" + func -> uid + "/playlists.json");
    query = commonMethodParams();
    url.setQuery(query);

    m_http = netManager -> getSync(QNetworkRequest(url));

    ar = m_http -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    func -> result.insert("playlists", responseToJson(ar).value("response").toArray());
    delete m_http;

    //////////////////////////////////////////////////////////////

    if (func -> uid == getUserID()) {
        QThread::msleep(REQUEST_DELAY);
        //////////////////////////////////////////////////////////////
        url.setUrl(getAPIUrl() + "users/" + func -> uid + "/followings.json");
        url.setQuery(query);
        m_http = netManager -> getSync(QNetworkRequest(url));

        ar = m_http -> readAll();
        ar.prepend("{\"response\":"); ar.append("}");
        func -> result.insert("followings", responseToJson(ar).value("response").toArray());
        delete m_http;

        //////////////////////////////////////////////////////////////

        url.setUrl(getAPIUrl() + "users/" + func -> uid + "/followers.json");
        url.setQuery(query);
        m_http = netManager -> getSync(QNetworkRequest(url));

        ar = m_http -> readAll();
        ar.prepend("{\"response\":"); ar.append("}");
        func -> result.insert("followers", responseToJson(ar).value("response").toArray());
        delete m_http;

        //////////////////////////////////////////////////////////////
        QThread::sleep(REQUEST_DELAY);
        //////////////////////////////////////////////////////////////

        url.setUrl(getAPIUrl() + "users/" + func -> uid + "/groups.json");
        url.setQuery(query);
        m_http = netManager -> getSync(QNetworkRequest(url));

        ar = m_http -> readAll();
        ar.prepend("{\"response\":"); ar.append("}");
        func -> result.insert("groups", responseToJson(ar).value("response").toArray());
        delete m_http;

        //////////////////////////////////////////////////////////////
    }

    delete netManager;

    return func;
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

void SoundcloudApi::errorSend(QJsonObject & doc, const QObject * obj) {
    QJsonObject error = doc.value("error").toObject();
    int err_code = error.value("error_code").toInt();
    QString err_msg = error.value("error_msg").toString();

    connect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
    emit errorReceived(err_code, err_msg);
    disconnect(this, SIGNAL(errorReceived(int,QString&)), obj, SLOT(errorReceived(int,QString&)));
}
