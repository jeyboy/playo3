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
    QNetworkReply * postForm(QUrl url);

    inline QJsonObject getJson(QString url, bool wrap = false) { return getJson(QUrl(url), wrap); }
    inline QJsonObject getJson(QUrl url, bool wrap = false) { return getJson(QNetworkRequest(url), wrap); }
    QJsonObject getJson(const QNetworkRequest & request, bool wrap = false);
    QJsonObject postJson(const QNetworkRequest & request, const QByteArray & data, bool wrap = false);

    inline QNetworkReply * openUrl(QUrl & url) { return proceedReply(getSync(QNetworkRequest(url))); }
    QPixmap openImage(QUrl & url);

    static inline QJsonObject replyToJson(QNetworkReply * reply, bool wrap = false) {
        QByteArray ar = reply -> readAll();
        if (wrap) { ar.prepend("{\"response\":"); ar.append("}"); }
        return QJsonDocument::fromJson(ar).object();
    }
    QNetworkReply * proceedReply(QNetworkReply * m_http) { // TODO: need to prevent from url cicling
        QVariant possibleRedirectUrl = m_http -> attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (possibleRedirectUrl.isValid()) {
            QUrl new_url = possibleRedirectUrl.toUrl();
            if (new_url.isRelative())
                new_url = m_http -> url().resolved(new_url); // need to check this

            m_http -> deleteLater();
            return openUrl(new_url);
        }

        return m_http;
    }
protected:
    QNetworkReply * synchronizeRequest(QNetworkReply * m_http);
    QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
private:
    QSsl::SslProtocol protocol;
    QSslSocket::PeerVerifyMode mode;

    static QNetworkCookieJar * cookies;
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
