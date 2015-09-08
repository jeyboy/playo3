#include "web_manager.h"

WebResponse * WebResponse::followByRedirect() {
    qDebug() << "STATUS:" << status();
    QVariant possibleRedirectUrl = redirectUrl();
    if (possibleRedirectUrl.isValid()) {
        QUrl new_url = possibleRedirectUrl.toUrl();
        if (new_url.isRelative())
            new_url = url().resolved(new_url);

        deleteLater();
        return ((WebManager *)manager()) -> requestTo(new_url) -> viaGet() -> followByRedirect();
    }

    return this;
}
QJsonObject WebResponse::toJson(const QString & wrap = QString) {
    QByteArray ar = readAll();
    if (!wrap.isEmpty()) { ar.prepend(QStringLiteral("{\"%1\":").arg(wrap)); ar.append("}"); }
    deleteLater();
    return QJsonDocument::fromJson(ar).object();
}
QPixmap WebResponse::toImage() {
    QImage image;
    image.loadFromData(readAll());
    deleteLater();
    return QPixmap::fromImage(image);
}

//////////////////////////     WEB_REQUEST     /////////////////////////////

WebRequest * WebRequest::withHeaders(const QHash<QString, QString> & headers) {
    for(QHash<QString, QString>::ConstIterator header = headers.cbegin(); header != headers.cend(); header++)
        setRawHeader(header.key().toUtf8(), header.value().toUtf8());

    if (!headers.contains(QStringLiteral("Referer")))
        setRawHeader("Referer", url().path().toUtf8());

    return this;
}

WebResponse * WebRequest::viaGet() {
    return manager -> get(this);
}

WebResponse * WebRequest::viaPost(const QByteArray & data) {
    return manager -> post(this, data);
}

WebResponse * WebRequest::viaForm(const QByteArray & data) {
    setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
    return manager -> post(this, data);
}



//////////////////////////     WEB_MANAGER     /////////////////////////////


QHash<QObject *, WebManager *> WebManager::managers = QHash<QObject *, WebManager *>();
Cookies * WebManager::cookies = new Cookies(QApplication::instance());

WebManager * WebManager::manager() {
    QThread * thread = QThread::currentThread();
    if (!managers.contains(thread)) {
        qDebug() << "!!!!!!!!!!!!!!!!!!!! REGISTRATE MANAGER";
        managers.insert(thread, new WebManager());

        //QApplication::instance() -> thread()

        if (QThread::currentThread() != QApplication::instance() -> thread())
            connect(thread, SIGNAL(finished()), new WebManagerController(), SLOT(disconnectThread()));
    }
    return managers[thread];
}

WebManager::WebManager(QObject * parent, QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode)
    : QNetworkAccessManager(parent) {
    this -> protocol = protocol;
    this -> mode = mode;
    this -> setCookieJar(WebManager::cookies);
}

//QJsonObject WebManager::getJson(const QNetworkRequest & request, bool wrap) {
//    QNetworkReply * reply = getSync(request);
//    QJsonObject res = replyToJson(reply, wrap);
//    reply -> deleteLater();
//    return res;
//}
//QJsonObject WebManager::postJson(const QNetworkRequest & request, const QByteArray & data, bool wrap) {
//    QNetworkReply * reply = postSync(request, data);
//    QJsonObject res = replyToJson(reply, wrap);
//    reply -> deleteLater();
//    return res;
//}

//QNetworkReply * WebManager::postForm(const QUrl & url, bool redirect_follow, QHash<QString, QString> headers) {
//    return postForm(url, url.query(QUrl::FullyEncoded), redirect_follow, headers);
//}

//QNetworkReply * WebManager::postForm(const QUrl & url, const QString & body, bool redirect_follow, QHash<QString, QString> headers) {
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
//    return postSync(request, body.toUtf8(), redirect_follow, headers);
//}

//QPixmap WebManager::openImage(const QUrl & url) {
//    QImage image;
//    QNetworkReply * reply = openUrl(url);
//    image.loadFromData(reply -> readAll());
//    reply -> deleteLater();
//    return QPixmap::fromImage(image);
//}

WebResponse * WebManager::synchronizeRequest(QNetworkReply * m_http) {
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return WebResponse::fromReply(m_http);
}

QNetworkReply * WebManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(mode);
    config.setProtocol(protocol);
    (const_cast<QNetworkRequest &>(req)).setSslConfiguration(config);
    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}
