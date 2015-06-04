#include "custom_network_access_manager.h"

CustomNetworkAccessManager * CustomNetworkAccessManager::default_manager = 0;

CustomNetworkAccessManager * CustomNetworkAccessManager::manager() {
    if (!default_manager) default_manager = new CustomNetworkAccessManager(QApplication::instance());
    return default_manager;
}

CustomNetworkAccessManager::CustomNetworkAccessManager(QObject * parent, QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode)
    : QNetworkAccessManager(parent) {
    this -> protocol = protocol;
    this -> mode = mode;
}

QNetworkReply * CustomNetworkAccessManager::getSync(const QNetworkRequest & request) {
    QNetworkReply * ret = CustomNetworkAccessManager::get(request);
    synchronizeRequest(ret);
    return ret;
}

QNetworkReply * CustomNetworkAccessManager::postSync(const QNetworkRequest & request, const QByteArray & data) {
    QNetworkReply * ret = CustomNetworkAccessManager::post(request, data);
    synchronizeRequest(ret);
    return ret;
}

QJsonObject CustomNetworkAccessManager::getToJson(const QNetworkRequest & request) {
    QNetworkReply * reply = getSync(request);
    QJsonObject res = replyToJson(reply);
    reply -> deleteLater();
    return res;
}
QJsonObject CustomNetworkAccessManager::postToJson(const QNetworkRequest & request, const QByteArray & data) {
    QNetworkReply * reply = postSync(request, data);
    QJsonObject res = replyToJson(reply);
    reply -> deleteLater();
    return res;
}

QNetworkReply * CustomNetworkAccessManager::openUrl(QUrl & url) { // TODO: need to prevent from url cicling
    QNetworkReply * m_http = getSync(QNetworkRequest(url));

    QVariant possibleRedirectUrl = m_http -> attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (possibleRedirectUrl.isValid()) {
        QUrl url = possibleRedirectUrl.toUrl();
        m_http -> deleteLater();
        return openUrl(url);
    }

    return m_http;
}

QPixmap CustomNetworkAccessManager::openImage(QUrl & url) {
    QImage image;
    QNetworkReply * reply = openUrl(url);
    image.loadFromData(reply -> readAll());
    reply -> deleteLater();
    return QPixmap::fromImage(image);
}

void CustomNetworkAccessManager::synchronizeRequest(QNetworkReply * m_http) {
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

QNetworkReply * CustomNetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(mode);
    config.setProtocol(protocol);
    (const_cast<QNetworkRequest &>(req)).setSslConfiguration(config);
    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}
