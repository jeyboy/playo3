#ifndef CUSTOM_NETWORK_ACCESS_MANAGER_H
#define CUSTOM_NETWORK_ACCESS_MANAGER_H

#include <QtNetwork>

class CustomNetworkAccessManager : public QNetworkAccessManager {
    Q_OBJECT
public:
    CustomNetworkAccessManager(QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);
protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
private:
    QSsl::SslProtocol protocol;
    QSslSocket::PeerVerifyMode mode;
};

#endif // CUSTOM_NETWORK_ACCESS_MANAGER_H
