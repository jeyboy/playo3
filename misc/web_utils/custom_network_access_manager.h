#ifndef CUSTOM_NETWORK_ACCESS_MANAGER_H
#define CUSTOM_NETWORK_ACCESS_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>

class CustomNetworkAccessManager : public QNetworkAccessManager {
    Q_OBJECT
public:
    static CustomNetworkAccessManager * manager();
    static bool validManager(CustomNetworkAccessManager *& webManager) {
        bool new_manager = QThread::currentThread() != QApplication::instance() -> thread();
        webManager = new_manager ? new CustomNetworkAccessManager(QApplication::instance()) : manager();
        return new_manager;
    }

    CustomNetworkAccessManager(QObject * parent = 0, QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

    QNetworkReply * getSync(const QNetworkRequest & request);
    QNetworkReply * postSync(const QNetworkRequest & request, const QByteArray & data);

    QJsonObject getToJson(const QNetworkRequest & request, bool wrap = false);
    QJsonObject postToJson(const QNetworkRequest & request, const QByteArray & data, bool wrap = false);

    QNetworkReply * openUrl(QUrl & url);
    QPixmap openImage(QUrl & url);

    inline QJsonObject replyToJson(QNetworkReply * reply, bool wrap = false) {
        QByteArray ar = reply -> readAll();
        if (wrap) { ar.prepend("{\"response\":"); ar.append("}"); }
        return QJsonDocument::fromJson(ar).object();
    }
protected:
    void synchronizeRequest(QNetworkReply * m_http);
    QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
private:
    QSsl::SslProtocol protocol;
    QSslSocket::PeerVerifyMode mode;

    static CustomNetworkAccessManager * default_manager;
};

#endif // CUSTOM_NETWORK_ACCESS_MANAGER_H
