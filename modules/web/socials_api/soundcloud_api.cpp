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
        QJsonObject doc = CustomNetworkAccessManager::manager() -> postToJson(request, SoundcloudApiPrivate::authTokenUrlParams(query.queryItemValue("code")));

        if (doc.contains("access_token")) {
            QString newToken = doc.value("access_token").toString();

            QNetworkRequest request(SoundcloudApiPrivate::confirmAuthUrl(newToken));
            doc = CustomNetworkAccessManager::manager() -> getToJson(request);

            setParams(newToken, QString::number(doc.value("id").toInt()), "");
            emit authorized();
            return "accept";
        }

        return "reject";
    }

    return "";
}

///////////////////////////////////////////////////////////
/// AUDIO LIST
///////////////////////////////////////////////////////////

//"id": 142370360,
//"permalink": "sam-smith-stay-with-me",

QJsonObject SoundcloudApi::getAudiosInfo(QStringList audio_uids) {
    return CustomNetworkAccessManager::manager() -> getToJson(QNetworkRequest(SoundcloudApiPrivate::audiosUrl(audio_uids)), true);
}

QJsonObject SoundcloudApi::getAudioInfo(QString audio_uid) {
    return CustomNetworkAccessManager::manager() -> getToJson(QNetworkRequest(SoundcloudApiPrivate::audioUrl(audio_uid)), true);
//    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::audioUrl(audio_uid)));

//    QJsonObject obj = responseToJson(m_http -> readAll());

//    m_http -> close();
//    delete m_http;

//    return obj;
}

void SoundcloudApi::getGroupInfo(const QObject * receiver, const char * respSlot, QString uid) {
    startApiCall(QtConcurrent::run(this, &SoundcloudApi::getGroupInfoRoutine, new ApiFuncContainer(receiver, respSlot, uid)));
}

ApiFuncContainer * SoundcloudApi::getGroupInfoRoutine(ApiFuncContainer * func) {
//    uid = "101";
    QNetworkReply * m_http = CustomNetworkAccessManager::manager() -> getSync(QNetworkRequest(SoundcloudApiPrivate::groupAudiosUrl(func -> uid)));

    if (responseRoutine("audio_list", m_http, func)) {
        m_http = CustomNetworkAccessManager::manager() -> getSync(QNetworkRequest(SoundcloudApiPrivate::groupPlaylistsUrl(func -> uid)));
        responseRoutine("playlists", m_http, func);
    }

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

    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

    func -> uid = /*"183";*/ func -> uid == "0" ? getUserID() : func -> uid;

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

void SoundcloudApi::searchAudio(const QObject * receiver, const char * respSlot, QString predicate, QString genre, bool popular) {
    startApiCall(QtConcurrent::run(this, &SoundcloudApi::searchAudioRoutine, new ApiFuncContainer(receiver, respSlot, QString()), predicate, genre, popular));
}

QJsonObject SoundcloudApi::searchAudioSync(const QObject * receiver, QString predicate, QString genre, bool popular) {
    ApiFuncContainer * func = searchAudioRoutine(new ApiFuncContainer(receiver, 0, QString()), predicate, genre, popular);
    QJsonObject res = func -> result;
    delete func;
    return res;
}

ApiFuncContainer * SoundcloudApi::searchAudioRoutine(ApiFuncContainer * func, QString & predicate, QString & genre, bool popular) {
    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
    QNetworkReply * m_http;


    for(int i = 0; i < 5; i++) {
        m_http = netManager -> getSync(QNetworkRequest(SoundcloudApiPrivate::audiosSearchUrl(predicate, genre, popular, i * SoundcloudApiPrivate::amountLimit())));
        responseRoutine("audio_list", m_http, func);
    }

    delete netManager;
    return func;
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

bool SoundcloudApi::responseRoutine(QString fieldName, QNetworkReply * reply, ApiFuncContainer * func) {
    QJsonObject obj = CustomNetworkAccessManager::replyToJson(reply, true);

    reply -> deleteLater();

    bool hasError = obj.value("response").toObject().contains("errors");

    qDebug() << fieldName << hasError << obj;

    if (hasError)
        errorSend(obj, func -> obj);
    else {
        if (func -> result.contains(fieldName)) { // rewrite on array of json arrays
            QVariantList n_ar = obj.value("response").toArray().toVariantList();

            if (!n_ar.isEmpty()) {
                QVariantList ar = func -> result.value(fieldName).toArray().toVariantList();
                ar.append(n_ar);

                func -> result.insert(fieldName, QJsonArray::fromVariantList(ar));
            }
        } else
            func -> result.insert(fieldName, obj.value("response").toArray());
    }

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
