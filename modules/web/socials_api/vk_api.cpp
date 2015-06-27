#include "vk_api.h"

VkApi * VkApi::self = 0;

VkApi * VkApi::instance() {
    return self;
}

VkApi * VkApi::instance(QObject * parent, QJsonObject obj) {
    if(!self)
        self = new VkApi(parent, obj);
    else
        VkApi::instance() -> fromJson(obj);
    return self;
}

void VkApi::fromJson(QJsonObject hash) {
    TeuAuth::fromJson(hash);
    WebApi::fromJson(hash);
}
QJsonObject VkApi::toJson() {
    QJsonObject root;

    TeuAuth::toJson(root);
    WebApi::toJson(root);

    return root;
}

bool VkApi::isConnected() {
    return !token().isEmpty() && !userID().isEmpty();
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
void VkApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.fragment());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        emit responseReady("reject");
    } else if (query.hasQueryItem("access_token")) {
        setParams(
            query.queryItemValue("access_token"),
            query.queryItemValue("user_id"),
            query.queryItemValue("expires_in")
        );
        emit authorized();
        emit responseReady("accept");
    }
    else emit responseReady("");
}

///////////////////////////////////////////////////////////
/// WALL
///////////////////////////////////////////////////////////

ApiFunc * VkApi::wallMediaRoutine(ApiFunc * func, int offset, int count) {
    QJsonObject doc;
    QVariantList res;

    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
    QNetworkReply * m_http;

    while(true) {
        m_http = netManager -> getSync(QNetworkRequest(
            VkApiPrivate::wallUrl(func -> uid, token(), offset, count)
        ));

        if (!responseRoutine(m_http, func, doc))
            break;

        qDebug() << doc;
        doc = doc.value("response").toObject();

        res.append(doc.value("posts").toArray().toVariantList());

        offset = doc.value("offset").toInt();
        count = doc.value("count").toInt();
        if (offset >= count)
            break;
    }

    func -> result.insert("posts", QJsonArray::fromVariantList(res));
    return func;
}

void VkApi::wallMediaList(const QObject * receiver, const char * respSlot, QString uid, int offset, int count) {
    startApiCall(QtConcurrent::run(this, &VkApi::wallMediaRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), offset, count));
}

///////////////////////////////////////////////////////////
/// FOLDERS LIST
///////////////////////////////////////////////////////////

ApiFunc * VkApi::audioAlbumsRoutine(ApiFunc * func, int offset) {
    QJsonObject doc;
    QVariantList res, temp;
    res.append(func -> result.value("albums").toArray().toVariantList());

    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
    QNetworkReply * m_http;

    while(true) {
        m_http = netManager -> getSync(QNetworkRequest(
            VkApiPrivate::audioAlbumsUrl(func -> uid, token(), offset)
        ));

        if (!responseRoutine(m_http, func, doc))
            break;

        doc = doc.value("response").toObject();

        temp = doc.value("albums").toArray().toVariantList();
        if (temp.isEmpty())
            break;

        temp.append(res);
        res = temp;
//        res.append(doc.value("albums").toArray().toVariantList());
        offset = doc.value("offset").toInt();
        if (doc.value("finished").toBool())
            break;
    }

    func -> result.insert("albums", QJsonArray::fromVariantList(res));

    return func;
}

void VkApi::audioAlbums(const QObject * receiver, const char * respSlot, QString uid) {
    startApiCall(QtConcurrent::run(this, &VkApi::audioAlbumsRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), 0));
}

///////////////////////////////////////////////////////////
/// AUDIO LIST
///////////////////////////////////////////////////////////

ApiFunc * VkApi::audioListRoutine(ApiFunc * func) {
    QNetworkReply * m_http;
    QUrl url = VkApiPrivate::audioInfoUrl(func -> uid, userID(), token());
    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

    m_http = netManager -> getSync(QNetworkRequest(url));
    if (responseRoutine(m_http, func, func -> result)) {
        func -> result = func -> result.value("response").toObject();

        if (!func -> result.value("albums_finished").toBool()) {
            int offset = func -> result.value("albums_offset").toInt();
            audioAlbumsRoutine(func, offset);
        }
    }

    return func;
}

void VkApi::audioList(const QObject * receiver, const char * respSlot, QString uid) {
    startApiCall(QtConcurrent::run(this, &VkApi::audioListRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid))));
}

ApiFunc * VkApi::audioRecomendationRoutine(ApiFunc * func, bool byUser, bool randomize) {
    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

    QUrl url = VkApiPrivate::audioRecomendationUrl(
        func -> uid,
        byUser,
        randomize,
        token()
    );

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
    if (responseRoutine(m_http, func, func -> result)) {
        func -> result = func -> result.value("response").toObject();
    }

    return func;
}
void VkApi::audioRecomendation(const QObject * receiver, const char * respSlot, QString uid, bool byUser, bool randomize) {
    startApiCall(QtConcurrent::run(this, &VkApi::audioRecomendationRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), byUser, randomize));
}

ApiFunc * VkApi::searchAudioRoutine(ApiFunc * func, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular) {
    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

    QUrl url = VkApiPrivate::audioSearchUrl(
        predicate,
        false,
        onlyArtist,
        inOwn,
        mostPopular ? 2 : 0,
        token()
    );

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
    if (responseRoutine(m_http, func, func -> result))
        func -> result = func -> result.value("response").toObject();

    return func;
}
void VkApi::audioSearch(const QObject * receiver, const char * respSlot, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular) {
    startApiCall(QtConcurrent::run(this, &VkApi::searchAudioRoutine, new ApiFunc(receiver, respSlot, adapteUid(uid)), predicate, onlyArtist, inOwn, mostPopular));
}

QJsonObject VkApi::audioSearchSync(const QObject * receiver, QString uid, QString predicate, bool onlyArtist, bool inOwn, bool mostPopular) {
    ApiFunc * func = searchAudioRoutine(new ApiFunc(receiver, 0, adapteUid(uid)), predicate, onlyArtist, inOwn, mostPopular);
    QJsonObject res = func -> result;
    delete func;
    return res;
}

QJsonObject VkApi::audioSearchSync(const QObject * receiver, QString predicate, int limitation) {
    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();
    ApiFunc * func = new ApiFunc(receiver, 0, "");

    QUrl url = VkApiPrivate::audioSearchLimitedUrl(
        predicate,
        limitation,
        token()
    );

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
    QJsonObject res;
    if (responseRoutine(m_http, func, res))
        res = res.value("response").toObject();

    delete func;
    return res;
}

ApiFunc * VkApi::audioPopularRoutine(ApiFunc * func, bool onlyEng, int genreId) {
    CustomNetworkAccessManager * netManager = CustomNetworkAccessManager::manager();

    QUrl url = VkApiPrivate::audioPopularUrl(
        onlyEng,
        token(),
        genreId
    );

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
    if (responseRoutine(m_http, func, func -> result))
        func -> result = func -> result.value("response").toObject();

    return func;
}
void VkApi::audioPopular(const QObject * receiver, const char * respSlot, bool onlyEng, int genreId) {
    startApiCall(QtConcurrent::run(this, &VkApi::audioPopularRoutine, new ApiFunc(receiver, respSlot, 0), onlyEng, genreId));
}

QJsonObject VkApi::audioPopularSync(const QObject * receiver, bool onlyEng, int genreId) {
    ApiFunc * func = audioPopularRoutine(new ApiFunc(receiver, 0, 0), onlyEng, genreId);
    QJsonObject res = func -> result;
    delete func;
    return res;
}

QJsonObject VkApi::getAudiosInfo(QStringList audio_uids) {
    QUrl url = VkApiPrivate::audioRefreshUrl(audio_uids, token());

    CustomNetworkAccessManager * netManager;
    bool new_manager = CustomNetworkAccessManager::validManager(netManager);

//    QNetworkReply * reply = netManager -> getSync(QNetworkRequest(url));

//    QJsonObject doc = responseToJson(reply -> readAll());

//    reply -> close();
//    delete reply;

    QJsonObject doc = netManager -> getToJson(QNetworkRequest(url));

    if (new_manager)
        delete netManager;

    return doc;
}

QJsonObject VkApi::getAudioInfo(QString audio_uid) {
    QStringList uids; uids << audio_uid;
    QJsonObject doc = getAudiosInfo(uids);
    return doc.value("response").toArray().first().toObject();
}

QString VkApi::refreshAudioItemUrl(QString audio_uid) {
    return getAudioInfo(audio_uid).value("url").toString();
}

////TODO: has some troubles with ids amount in request
//void VkApi::refreshAudioList(const QObject * receiver, const char * respSlot, QList<QString> uids) { // TODO: rewrite required
//    QUrl url = VkApiPrivate::audioRefreshUrl(uids, getToken());
//    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
////    responses.insert(m_http, responseSlot);
////    collations.insert(m_http, uids);
//    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListResponse()));
//}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

bool VkApi::responseRoutine(QNetworkReply * reply, ApiFunc * func, QJsonObject & doc) {
    doc = CustomNetworkAccessManager::replyToJson(reply);

    QUrl url = reply -> url();
    reply -> deleteLater();

    if (doc.contains("error")) {
        doc = doc.value("error").toObject();
        return errorSend(doc, func, url);
    }

    return true;
}

bool VkApi::errorSend(QJsonObject & error, ApiFunc * func, QUrl url) {
    int err_code = error.value("error_code").toInt();
    QString err_msg = error.value("error_msg").toString();

    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ERROR " << error;

    if (err_code != 14) {
        connect(this, SIGNAL(errorReceived(int,QString)), func -> obj, SLOT(errorReceived(int,QString)));
        emit errorReceived(err_code, err_msg);
        disconnect(this, SIGNAL(errorReceived(int,QString)), func -> obj, SLOT(errorReceived(int,QString)));
        return false;
    }
    else return captchaProcessing(error, func, url);
}

bool VkApi::captchaProcessing(QJsonObject & error, ApiFunc * func, QUrl url) {
    CustomNetworkAccessManager * manager = CustomNetworkAccessManager::manager();

    QUrl image_url(error.value("captcha_img").toString());
    captchaDialog -> setImage(manager -> openImage(image_url));
    emit showCaptcha();

    QString captchaText = captchaDialog -> captchaText();
    if (captchaText.isEmpty())
        return false;

    QUrlQuery query(url.query());
    query.removeQueryItem("captcha_sid");
    query.removeQueryItem("captcha_key");

    query.addQueryItem("captcha_sid", error.value("captcha_sid").toString());
    query.addQueryItem("captcha_key", captchaText);

    url.setQuery(query);

    QNetworkReply * m_http = CustomNetworkAccessManager::manager() -> getSync(QNetworkRequest(url));
    return responseRoutine(m_http, func, error);
}

///////////////////////////////////////////////////////////
/// SLOTS
///////////////////////////////////////////////////////////


//void VkApi::audioCountRequest() {}
//void VkApi::audioSearchRequest() {}
//void VkApi::audioCopyRequest() {}
//void VkApi::audioRemoveRequest() {}

//void VkApi::audioSaveServerRequest() {}
//void VkApi::audioSave() {}

//void VkApi::audioAlbumsRequest() {}
//void VkApi::audioAlbumAddRequest() {}
//void VkApi::audioAlbumEditRequest() {}
//void VkApi::audioAlbumRemoveRequest() {}
//void VkApi::audioAlbumMoveToRequest() {}
