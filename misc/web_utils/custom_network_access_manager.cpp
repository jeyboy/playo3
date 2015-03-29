#include "custom_network_access_manager.h"

CustomNetworkAccessManager::CustomNetworkAccessManager(QObject * parent, QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode)
    : QNetworkAccessManager(parent) {
    this -> protocol = protocol;
    this -> mode = mode;
}

QNetworkReply * CustomNetworkAccessManager::openUrl(QUrl & url) { // TODO: need to prevent from url cicling
    QNetworkReply * m_http = get(QNetworkRequest(url));
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QVariant possibleRedirectUrl = m_http -> attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (possibleRedirectUrl.isValid()) {
        QUrl url = possibleRedirectUrl.toUrl();
        delete m_http;
        return openUrl(url);
    }
    else return m_http;
}

void CustomNetworkAccessManager::synchronizeRequest(QNetworkReply * m_http) {
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

QNetworkReply * CustomNetworkAccessManager::createRequest(Operation op, const QNetworkRequest &req, QIODevice * outgoingData) {
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(mode);
    config.setProtocol(protocol);
    QNetworkRequest request(req);
    request.setSslConfiguration(config);
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}
