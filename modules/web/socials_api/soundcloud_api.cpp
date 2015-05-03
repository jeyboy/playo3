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
QString SoundcloudApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.query());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        return "reject";
    } else if (query.hasQueryItem("code")) {
        QNetworkRequest request(authTokenUrl());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QNetworkReply * m_http = manager() -> postSync(request, SoundcloudApiPrivate::authTokenUrlParams(query.queryItemValue("code")));

        QJsonObject doc = responseToJson(m_http -> readAll());
        delete m_http;

        if (doc.contains("access_token")) {
            QString newToken = doc.value("access_token").toString();

            QNetworkRequest request(SoundcloudApiPrivate::confirmAuthUrl(newToken));
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

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::groupAudiosUrl(func -> uid)));

    if (responseRoutine("audio_list", m_http, func)) {
        m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::groupPlaylistsUrl(func -> uid)));
        responseRoutine("playlists", m_http, func);
    }

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

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userAudiosUrl(func -> uid)));

    if (responseRoutine("audio_list", m_http, func)) {
        m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userPlaylistsUrl(func -> uid)));

        if (responseRoutine("playlists", m_http, func)) {
            if (func -> uid == getUserID()) {
                QThread::msleep(REQUEST_DELAY);
                //////////////////////////////////////////////////////////////
                m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userFolowingsUrl(func -> uid)));

                if (responseRoutine("followings", m_http, func)) {
                    m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userFolowersUrl(func -> uid)));

                    if (responseRoutine("followers", m_http, func)) {
                        QThread::msleep(REQUEST_DELAY);

                        m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::userGroupsUrl(func -> uid)));
                        responseRoutine("groups", m_http, func);
                    }
                }
            }
        }
    }

    delete netManager;
    return func;
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

bool SoundcloudApi::responseRoutine(QString fieldName, QNetworkReply * reply, ApiFuncContainer * func) {
    QByteArray ar = reply -> readAll();
    ar.prepend("{\"response\":"); ar.append("}");
    QJsonObject obj = responseToJson(ar);

    reply -> close();
    delete reply;

    bool hasError = obj.value("response").toObject().contains("errors");

    qDebug() << fieldName << hasError << obj;

    if (hasError)
        errorSend(obj, func -> obj);
    else
        func -> result.insert(fieldName, obj.value("response").toArray());

    return !hasError;
}

void SoundcloudApi::errorSend(QJsonObject & doc, const QObject * obj) {
    QJsonObject error = doc.value("response").toObject().value("errors").toArray().first().toObject();
    int err_code = -1;//error.value("error_code").toInt();
    QString err_msg = error.value("error_message").toString();

    connect(this, SIGNAL(errorReceived(int,QString)), obj, SLOT(errorReceived(int,QString)));
    emit errorReceived(err_code, err_msg);
    disconnect(this, SIGNAL(errorReceived(int,QString)), obj, SLOT(errorReceived(int,QString)));
}
