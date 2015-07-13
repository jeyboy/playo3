#include "web_manager.h"

WebManager * WebManager::default_manager = 0;

WebManager::WebManager(QObject * parent, QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode)
    : QNetworkAccessManager(parent) {
    this -> protocol = protocol;
    this -> mode = mode;
}

QNetworkReply * WebManager::getSync(const QNetworkRequest & request) {
    QNetworkReply * ret = WebManager::get(request);
    synchronizeRequest(ret);
    return ret;
}

QNetworkReply * WebManager::postSync(const QNetworkRequest & request, const QByteArray & data) {
    QNetworkReply * ret = WebManager::post(request, data);
    synchronizeRequest(ret);
    return ret;
}

QJsonObject WebManager::getToJson(const QNetworkRequest & request, bool wrap) {
    QNetworkReply * reply = getSync(request);
    QJsonObject res = replyToJson(reply, wrap);
    reply -> deleteLater();
    return res;
}
QJsonObject WebManager::postToJson(const QNetworkRequest & request, const QByteArray & data, bool wrap) {
    QNetworkReply * reply = postSync(request, data);
    QJsonObject res = replyToJson(reply, wrap);
    reply -> deleteLater();
    return res;
}

QNetworkReply * WebManager::openUrl(QUrl & url) { // TODO: need to prevent from url cicling
    QNetworkReply * m_http = getSync(QNetworkRequest(url));

    QVariant possibleRedirectUrl = m_http -> attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (possibleRedirectUrl.isValid()) {
        QUrl url = possibleRedirectUrl.toUrl();
        m_http -> deleteLater();
        return openUrl(url);
    }

    return m_http;
}

QPixmap WebManager::openImage(QUrl & url) {
    QImage image;
    QNetworkReply * reply = openUrl(url);
    image.loadFromData(reply -> readAll());
    reply -> deleteLater();
    return QPixmap::fromImage(image);
}

void WebManager::synchronizeRequest(QNetworkReply * m_http) {
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

QNetworkReply * WebManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(mode);
    config.setProtocol(protocol);
    (const_cast<QNetworkRequest &>(req)).setSslConfiguration(config);
    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}
