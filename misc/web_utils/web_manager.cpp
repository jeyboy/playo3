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

QNetworkReply * WebManager::postForm(QUrl url, QHash<QString, QString> headers) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
    if (!headers.contains(QStringLiteral("Referer")))
        headers.insert(QStringLiteral("Referer"), url.path());

    for(QHash<QString, QString>::Iterator header = headers.begin(); header != headers.end(); header++)
        request.setRawHeader(header.key().toUtf8(), header.value().toUtf8());

    return proceedReply(postSync(request, url.query(QUrl::FullyEncoded).toUtf8()));
}

QPixmap WebManager::openImage(QUrl & url) {
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
