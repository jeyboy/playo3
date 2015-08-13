#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>
#include "misc/logger.h"

class WebManagerController;

class WebManager : public QNetworkAccessManager {
    Q_OBJECT
public:
    static WebManager * manager();

    WebManager(QObject * parent = 0, QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

    inline QNetworkReply * getSync(QString path) { return getSync(QUrl(path)); }
    inline QNetworkReply * getSync(QUrl url) { return getSync(QNetworkRequest(url)); }
    inline QNetworkReply * getSync(const QNetworkRequest & request) { return synchronizeRequest(WebManager::get(request)); }
    inline QNetworkReply * postSync(const QNetworkRequest & request, const QByteArray & data) { return synchronizeRequest(WebManager::post(request, data)); }

    inline QJsonObject getJson(QString url, bool wrap = false) { return getJson(QUrl(url), wrap); }
    inline QJsonObject getJson(QUrl url, bool wrap = false) { return getJson(QNetworkRequest(url), wrap); }
    QJsonObject getJson(const QNetworkRequest & request, bool wrap = false);
    QJsonObject postJson(const QNetworkRequest & request, const QByteArray & data, bool wrap = false);

    QNetworkReply * openUrl(QUrl & url);
    QPixmap openImage(QUrl & url);

    static inline QJsonObject replyToJson(QNetworkReply * reply, bool wrap = false) {
        QByteArray ar = reply -> readAll();
        if (wrap) { ar.prepend("{\"response\":"); ar.append("}"); }
        return QJsonDocument::fromJson(ar).object();
    }
protected:
    QNetworkReply * synchronizeRequest(QNetworkReply * m_http);
    QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
private:
    QSsl::SslProtocol protocol;
    QSslSocket::PeerVerifyMode mode;

    static QHash<QObject *, WebManager *> managers;
    friend class WebManagerController;
};

class WebManagerController : public QObject {
    Q_OBJECT
protected slots:
    inline void disconnectThread() {
        WebManager * tmanager = WebManager::managers.take(sender());
        qDebug() << "!!!!!!!!!!!!!!!!!!!! UNTE MANAGER" << tmanager;
        Logger::instance() -> writeToStream(QStringLiteral("WebManager"), QStringLiteral("disconnection"));
        if (tmanager) tmanager -> deleteLater();
        deleteLater();
    }
};


#endif // WEB_MANAGER_H
