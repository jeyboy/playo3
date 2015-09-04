#include "web_manager.h"

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

QJsonObject WebManager::getJson(const QNetworkRequest & request, bool wrap) {
    QNetworkReply * reply = getSync(request);
    QJsonObject res = replyToJson(reply, wrap);
    reply -> deleteLater();
    return res;
}
QJsonObject WebManager::postJson(const QNetworkRequest & request, const QByteArray & data, bool wrap) {
    QNetworkReply * reply = postSync(request, data);
    QJsonObject res = replyToJson(reply, wrap);
    reply -> deleteLater();
    return res;
}

QNetworkReply * WebManager::postForm(const QUrl & url, bool redirect_follow, QHash<QString, QString> headers) {
    return postForm(url, url.query(QUrl::FullyEncoded), redirect_follow, headers);
}

QNetworkReply * WebManager::postForm(const QUrl & url, const QString & body, bool redirect_follow, QHash<QString, QString> headers) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
    setHeaders(request, headers);

    if (redirect_follow)
        return proceedReply(postSync(request, body.toUtf8()));
    else
        return postSync(request, body.toUtf8());
}

QPixmap WebManager::openImage(const QUrl & url) {
    QImage image;
    QNetworkReply * reply = openUrl(url);
    image.loadFromData(reply -> readAll());
    reply -> deleteLater();
    return QPixmap::fromImage(image);
}

QNetworkReply * WebManager::synchronizeRequest(QNetworkReply * m_http) {
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return m_http;
}

QNetworkReply * WebManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(mode);
    config.setProtocol(protocol);
    (const_cast<QNetworkRequest &>(req)).setSslConfiguration(config);
    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}
