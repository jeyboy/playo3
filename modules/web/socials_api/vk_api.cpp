#include "vk_api.h"

VkApi * VkApi::self = 0;

VkApi * VkApi::instance() {
    if(!self)
        self = new VkApi();
    return self;
}

VkApi * VkApi::instance(QJsonObject obj) {
    if(!self)
        self = new VkApi(obj);
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
    return !getToken().isEmpty() && !getUserID().isEmpty();
}

///////////////////////////////////////////////////////////
/// AUTH
///////////////////////////////////////////////////////////
QString VkApi::authUrl() const {
    return VkApiPrivate::authUrl();
}

QString VkApi::proceedAuthResponse(const QUrl & url) {
    QUrlQuery query(url.fragment());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        return "reject";
    } else if (query.hasQueryItem("access_token")) {
        setParams(
                  query.queryItemValue("access_token"),
                  query.queryItemValue("user_id"),
                  query.queryItemValue("expires_in")
                  );
        return "accept";
    }

    return "";
}

///////////////////////////////////////////////////////////
/// WALL
///////////////////////////////////////////////////////////

ApiFuncContainer * VkApi::wallMediaRoutine(ApiFuncContainer * func, int offset, int count) {
    QJsonObject doc;
    QVariantList res;

    CustomNetworkAccessManager * netManager = createManager();
    QNetworkReply * m_http;

    while(true) {
        m_http = netManager -> getSync(QNetworkRequest(
            VkApiPrivate::wallUrl(func -> uid, getToken(), offset, count)
        ));

        if (!responseRoutine(m_http, func, doc))
            break;

        doc = doc.value("response").toObject();

        res.append(doc.value("posts").toArray().toVariantList());

        offset = doc.value("offset").toInt();
        count = doc.value("count").toInt();
        if (offset >= count)
            break;
    }

    func -> result.insert("posts", QJsonArray::fromVariantList(res));
    delete netManager;
    return func;
}

void VkApi::wallMediaList(const QObject * receiver, const char * respSlot, QString uid, int offset, int count) {
    startApiCall(QtConcurrent::run(this, &VkApi::wallMediaRoutine, new ApiFuncContainer(receiver, respSlot, adapteUid(uid)), offset, count));
}

///////////////////////////////////////////////////////////
/// FOLDERS LIST
///////////////////////////////////////////////////////////

ApiFuncContainer * VkApi::audioAlbumsRoutine(ApiFuncContainer * func, int offset) {
    QJsonObject doc;
    QVariantList res, temp;
    res.append(func -> result.value("albums").toArray().toVariantList());

    CustomNetworkAccessManager * netManager = createManager();
    QNetworkReply * m_http;

    while(true) {
        m_http = netManager -> getSync(QNetworkRequest(
            VkApiPrivate::audioAlbumsUrl(func -> uid, getToken(), offset)
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

        QThread::sleep(1);
    }

    func -> result.insert("albums", QJsonArray::fromVariantList(res));
    delete netManager;

    return func;
}

void VkApi::audioAlbums(const QObject * receiver, const char * respSlot, QString uid) {
    startApiCall(QtConcurrent::run(this, &VkApi::audioAlbumsRoutine, new ApiFuncContainer(receiver, respSlot, adapteUid(uid)), 0));
}

///////////////////////////////////////////////////////////
/// AUDIO LIST
///////////////////////////////////////////////////////////

ApiFuncContainer * VkApi::audioListRoutine(ApiFuncContainer * func) {
    QNetworkReply * m_http;
    QUrl url = VkApiPrivate::audioInfoUrl(func -> uid, getUserID(), getToken());
    CustomNetworkAccessManager * netManager = createManager();

    m_http = netManager -> getSync(QNetworkRequest(url));
    if (responseRoutine(m_http, func, func -> result)) {
        func -> result = func -> result.value("response").toObject();

        if (!func -> result.value("albums_finished").toBool()) {
            int offset = func -> result.value("albums_offset").toInt();
            audioAlbumsRoutine(func, offset);
        }
    }

    delete netManager;
    return func;
}

void VkApi::audioList(const QObject * receiver, const char * respSlot, QString uid) {
    startApiCall(QtConcurrent::run(this, &VkApi::audioListRoutine, new ApiFuncContainer(receiver, respSlot, adapteUid(uid))));
}

//TODO: has some troubles with ids amount in request
void VkApi::refreshAudioList(const QObject * receiver, const char * respSlot, QList<QString> uids) { // TODO: rewrite required
    QUrl url = VkApiPrivate::audioRefreshUrl(uids, getToken());
    QNetworkReply * m_http = manager() -> get(QNetworkRequest(url));
//    responses.insert(m_http, responseSlot);
//    collations.insert(m_http, uids);
    QObject::connect(m_http, SIGNAL(finished()), this, SLOT(audioListResponse()));
}

///////////////////////////////////////////////////////////
/// PROTECTED
///////////////////////////////////////////////////////////

void VkApi::startApiCall(QFuture<ApiFuncContainer *> feature) {
    QFutureWatcher<ApiFuncContainer *> * initiator = new QFutureWatcher<ApiFuncContainer *>();
    connect(initiator, SIGNAL(finished()), this, SLOT(apiCallFinished()));
    initiator -> setFuture(feature);
}

void VkApi::apiCallFinished() {
    QFutureWatcher<ApiFuncContainer *> * initiator = (QFutureWatcher<ApiFuncContainer *> *)sender();
    ApiFuncContainer * func = initiator -> result();
    connect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
    emit routineFinished(func -> result);
    disconnect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
}

bool VkApi::responseRoutine(QNetworkReply * reply, ApiFuncContainer * func, QJsonObject & doc) {
    doc = responseToJson(reply -> readAll());

    QUrl url = reply -> url();
    reply -> close();
    delete reply;

    if (doc.contains("error")) {
        doc = doc.value("error").toObject();
        return errorSend(doc, func, url);
    }

    return true;
}

bool VkApi::errorSend(QJsonObject & error, ApiFuncContainer * func, QUrl url) {
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

bool VkApi::captchaProcessing(QJsonObject & error, ApiFuncContainer * func, QUrl url) {
    captchaDialog -> setImage(this, error.value("captcha_img").toString());
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

    QNetworkReply * m_http = netManager -> getSync(QNetworkRequest(url));
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
