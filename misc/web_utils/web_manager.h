#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>
#include "misc/logger.h"

class WebManagerController;
class Cookies : public QNetworkCookieJar {
public:
    inline explicit Cookies(QObject * parent = 0) : QNetworkCookieJar(parent) {}
    inline QList<QNetworkCookie> allCookies() const { return QNetworkCookieJar::allCookies(); }
};

class WebManager : public QNetworkAccessManager {
    Q_OBJECT
public:
    static WebManager * manager();

    WebManager(QObject * parent = 0, QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

    static inline void printCookies() { qDebug() << "COOKIE" << cookies -> allCookies(); }
    static inline QString cookie(const QString & name, QUrl url = QUrl()) {
        const QList<QNetworkCookie> items = url.isEmpty() ? cookies -> allCookies() : cookies -> cookiesForUrl(url);
        for(QList<QNetworkCookie>::ConstIterator cookie = items.cbegin(); cookie != items.cend(); cookie++)
            if ((*cookie).name() == name)
                return QString((*cookie).value());

        return QString();
    }

    static inline int status(QNetworkReply * reply) { return reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); }
    static inline QVariant redirectUrl(QNetworkReply * reply) { return reply -> attribute(QNetworkRequest::RedirectionTargetAttribute); }

    inline void setHeaders(QNetworkRequest & request, QHash<QString, QString> headers) {
        if (!headers.contains(QStringLiteral("Referer")))
            headers.insert(QStringLiteral("Referer"), request.url().path());

        for(QHash<QString, QString>::Iterator header = headers.begin(); header != headers.end(); header++)
            request.setRawHeader(header.key().toUtf8(), header.value().toUtf8());
    }

    inline QNetworkReply * getSync(QString path, bool redirect_follow = true, QHash<QString, QString> headers = QHash<QString, QString>()) { return getSync(QUrl(path), redirect_follow, headers); }
    inline QNetworkReply * getSync(QUrl url, bool redirect_follow = true, QHash<QString, QString> headers = QHash<QString, QString>()) { return getSync(QNetworkRequest(url), redirect_follow, headers); }
    inline QNetworkReply * getSync(QNetworkRequest request, bool redirect_follow = true, QHash<QString, QString> headers = QHash<QString, QString>()) {
        setHeaders(request, headers);

        if (redirect_follow)
            return proceedReply(synchronizeRequest(WebManager::get(request)));
        else
            return synchronizeRequest(WebManager::get(request));
    }
    inline QNetworkReply * postSync(QNetworkRequest request, const QByteArray & data, bool redirect_follow = true, QHash<QString, QString> headers = QHash<QString, QString>()) {
        setHeaders(request, headers);

        if (redirect_follow)
            return proceedReply(synchronizeRequest(WebManager::post(request, data)));
        else
            return synchronizeRequest(WebManager::post(request, data));
    }
    QNetworkReply * postForm(const QUrl & url, bool redirect_follow = true, QHash<QString, QString> headers = QHash<QString, QString>());
    QNetworkReply * postForm(const QUrl & url, const QString & body, bool redirect_follow = true, QHash<QString, QString> headers = QHash<QString, QString>());

    inline QJsonObject getJson(const QString & url, bool wrap = false) { return getJson(QUrl(url), wrap); }
    inline QJsonObject getJson(const QUrl & url, bool wrap = false) { return getJson(QNetworkRequest(url), wrap); }
    QJsonObject getJson(const QNetworkRequest & request, bool wrap = false);
    QJsonObject postJson(const QNetworkRequest & request, const QByteArray & data, bool wrap = false);

    inline QNetworkReply * openUrl(const QUrl & url) { return proceedReply(getSync(QNetworkRequest(url))); }
    QPixmap openImage(const QUrl & url);

    static inline QJsonObject replyToJson(QNetworkReply * reply, bool wrap = false) {
        QByteArray ar = reply -> readAll();
        if (wrap) { ar.prepend("{\"response\":"); ar.append("}"); }
        return QJsonDocument::fromJson(ar).object();
    }
    QNetworkReply * proceedReply(QNetworkReply * m_http) { // TODO: need to prevent from url cicling
        qDebug() << "STATUS:" << status(m_http);
        QVariant possibleRedirectUrl = redirectUrl(m_http);
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

    static Cookies * cookies;
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
