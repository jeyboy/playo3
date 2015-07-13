#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>

class WebManager : public QNetworkAccessManager {
    Q_OBJECT
public:
    inline static WebManager * stock() {
        if (!default_manager) default_manager = new WebManager(QApplication::instance());
        return default_manager;
    }
    static bool valid(WebManager *& webManager) {
        bool new_manager = QThread::currentThread() != QApplication::instance() -> thread();
        webManager = new_manager ? new WebManager(/*QThread::currentThread()*/) : stock();
        return new_manager;
    }

    WebManager(QObject * parent = 0, QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

    QNetworkReply * getSync(const QNetworkRequest & request);
    QNetworkReply * postSync(const QNetworkRequest & request, const QByteArray & data);

    QJsonObject getToJson(const QNetworkRequest & request, bool wrap = false);
    QJsonObject postToJson(const QNetworkRequest & request, const QByteArray & data, bool wrap = false);

    QNetworkReply * openUrl(QUrl & url);
    QPixmap openImage(QUrl & url);

    static inline QJsonObject replyToJson(QNetworkReply * reply, bool wrap = false) {
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

    static WebManager * default_manager;
};

#endif // WEB_MANAGER_H
