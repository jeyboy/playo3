#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>

#include "modules/core/misc/logger.h"
#include "modules/core/misc/web_utils/html_parser.h"

namespace Core {
    class WebManagerController;
    class Cookies : public QNetworkCookieJar {
    public:
        inline explicit Cookies(QObject * parent = 0) : QNetworkCookieJar(parent) {}
        inline QList<QNetworkCookie> allCookies() const { return QNetworkCookieJar::allCookies(); }
    };

    class WebResponse : public QNetworkReply {
    public:
        static inline WebResponse * fromReply(QNetworkReply * reply) { return (WebResponse *)reply; }

        inline int status() { return attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); }
        inline QVariant redirectUrl() { return attribute(QNetworkRequest::RedirectionTargetAttribute); }
        inline QString paramVal(const QString & param) { return QUrlQuery(url()).queryItemValue(param); }

        WebResponse * followByRedirect();
        QJsonObject toJson(const QString & wrap = QString());
        QPixmap toImage();
        Html::Document toHtml();
    };

    class WebManager;

    class WebRequest : public QNetworkRequest {
    public:
        inline WebRequest(WebManager * manager, const QString & url_str) : QNetworkRequest(QUrl(url_str)), manager(manager) {}
        inline WebRequest(WebManager * manager, const QUrl & url = QUrl()) : QNetworkRequest(url), manager(manager) {}

        WebRequest * withHeaders(const QHash<QString, QString> & headers);
        WebResponse * viaGet();
        WebResponse * viaPost(const QByteArray & data = QByteArray());
        WebResponse * viaForm(const QByteArray & data = QByteArray());
    private:
        WebManager * manager;
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

        static inline QString paramVal(const QUrl & url, const QString & param) { return QUrlQuery(url).queryItemValue(param); }

        WebResponse * get(const WebRequest & request) { return synchronizeRequest(QNetworkAccessManager::get((const QNetworkRequest &)request)); }
        WebResponse * post(const WebRequest & request, const QByteArray & data) { return synchronizeRequest(QNetworkAccessManager::post((const QNetworkRequest &)request, data)); }

        inline WebRequest * requestTo(const QString & url) { return new WebRequest(this, url); }
        inline WebRequest * requestTo(const QUrl & url) { return new WebRequest(this, url); }

        inline QPixmap getImage(const QUrl & url) { return followedGet(url) -> toImage(); }
        inline QJsonObject getJson(const QUrl & url, const QString & wrap) { return followedGet(url) -> toJson(wrap); }
        inline QJsonObject getJson(const QUrl & url, bool wrap = false) { return followedGet(url) -> toJson(wrap ? QStringLiteral("response") : QString()); }
        inline QJsonObject postJson(const QUrl & url, bool wrap = false) { return followedPost(url) -> toJson(wrap ? QStringLiteral("response") : QString()); }
        inline QJsonObject postJson(const QUrl & url, QHash<QString, QString> headers, bool wrap = false) { return followedPost(url, headers) -> toJson(wrap ? QStringLiteral("response") : QString()); }

        inline WebResponse * followedGet(const QUrl & url) { return requestTo(url) -> viaGet() -> followByRedirect(); }
        inline WebResponse * followedGet(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaGet() -> followByRedirect(); }

        inline WebResponse * followedPost(const QUrl & url) { return requestTo(url) -> viaPost() -> followByRedirect(); }
        inline WebResponse * followedPost(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaPost() -> followByRedirect(); }

        inline WebResponse * unfollowedForm(const QUrl & url) { return requestTo(url) -> viaForm(); }
        inline WebResponse * unfollowedForm(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaForm(); }
        inline WebResponse * followedForm(const QUrl & url) { return requestTo(url) -> viaForm() -> followByRedirect(); }
        inline WebResponse * followedForm(const QUrl & url, const QByteArray & data) { return requestTo(url) -> viaForm(data) -> followByRedirect(); }
        inline WebResponse * followedForm(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaForm() -> followByRedirect(); }

    public slots:
        inline void sendGet(QString & url) { followedGet(url) -> deleteLater(); }
    protected:
        WebResponse * synchronizeRequest(QNetworkReply * m_http);
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
            qDebug() << "!!!!!!!!!!!!!!!!!!!! UNREGISTRATE MANAGER";
            WebManager * tmanager = WebManager::managers.take(sender());
            emit Logger::instance() -> write(QStringLiteral("WebManager"), QStringLiteral("disconnection"));
            if (tmanager) tmanager -> deleteLater();
            deleteLater();
        }
    };
}

#endif // WEB_MANAGER_H
