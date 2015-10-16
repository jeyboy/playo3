#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>

#include "modules/core/misc/logger.h"
#include "modules/core/web/utils/html_parser.h"

namespace Core {
    namespace Web {
        class ManagerController;
        class Cookies : public QNetworkCookieJar {
        public:
            inline explicit Cookies(QObject * parent = 0) : QNetworkCookieJar(parent) {}
            inline QList<QNetworkCookie> allCookies() const { return QNetworkCookieJar::allCookies(); }
        };

        class Response : public QNetworkReply {
        public:
            static inline Response * fromReply(QNetworkReply * reply) { return (Response *)reply; }

            inline bool hasErrors() { return error() != NoError; }
            inline int status() { return attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); }
            inline QVariant redirectUrl() { return attribute(QNetworkRequest::RedirectionTargetAttribute); }
            inline QString paramVal(const QString & param) { return QUrlQuery(url()).queryItemValue(param); }

            Response * followByRedirect();
            QJsonObject toJson(const QString & wrap = QString());
            QPixmap toImage();
            Html::Document toHtml();
        };

        class Manager;

        class Request : public QNetworkRequest {
        public:
            inline Request(Manager * manager, const QString & url_str) : QNetworkRequest(QUrl(url_str)), manager(manager) {}
            inline Request(Manager * manager, const QUrl & url = QUrl()) : QNetworkRequest(url), manager(manager) {}

            Request * withHeaders(const QHash<QString, QString> & headers);
            Response * viaGet();
            Response * viaPost(const QByteArray & data = QByteArray());
            Response * viaForm(const QByteArray & data = QByteArray());
        private:
            Manager * manager;
        };

        class Manager : public QNetworkAccessManager {
            Q_OBJECT
        public:
            static Manager * prepare();

            Manager(QObject * parent = 0, QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

            static inline void printCookies() { qDebug() << "COOKIE" << cookies -> allCookies(); }
            static inline QString cookie(const QString & name, QUrl url = QUrl()) {
                const QList<QNetworkCookie> items = url.isEmpty() ? cookies -> allCookies() : cookies -> cookiesForUrl(url);
                for(QList<QNetworkCookie>::ConstIterator cookie = items.cbegin(); cookie != items.cend(); cookie++)
                    if ((*cookie).name() == name)
                        return QString((*cookie).value());

                return QString();
            }

            static inline QString paramVal(const QUrl & url, const QString & param) { return QUrlQuery(url).queryItemValue(param); }

            Response * get(const Request & request) { return synchronizeRequest(QNetworkAccessManager::get((const QNetworkRequest &)request)); }
            Response * post(const Request & request, const QByteArray & data) { return synchronizeRequest(QNetworkAccessManager::post((const QNetworkRequest &)request, data)); }

            inline Request * requestTo(const QString & url) { return new Request(this, url); }
            inline Request * requestTo(const QUrl & url) { return new Request(this, url); }

            inline QPixmap getImage(const QUrl & url) { return followedGet(url) -> toImage(); }
            inline QJsonObject getJson(const QUrl & url, const QString & wrap) { return followedGet(url) -> toJson(wrap); }
            inline QJsonObject getJson(const QUrl & url, bool wrap = false) { return followedGet(url) -> toJson(wrap ? QStringLiteral("response") : QString()); }
            inline QJsonObject postJson(const QUrl & url, bool wrap = false) { return followedPost(url) -> toJson(wrap ? QStringLiteral("response") : QString()); }
            inline QJsonObject postJson(const QUrl & url, QHash<QString, QString> headers, bool wrap = false) { return followedPost(url, headers) -> toJson(wrap ? QStringLiteral("response") : QString()); }

            inline Response * followedGet(const QUrl & url) { return requestTo(url) -> viaGet() -> followByRedirect(); }
            inline Response * followedGet(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaGet() -> followByRedirect(); }

            inline Response * followedPost(const QUrl & url) { return requestTo(url) -> viaPost() -> followByRedirect(); }
            inline Response * followedPost(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaPost() -> followByRedirect(); }

            inline Response * unfollowedForm(const QUrl & url) { return requestTo(url) -> viaForm(); }
            inline Response * unfollowedForm(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaForm(); }
            inline Response * followedForm(const QUrl & url) { return requestTo(url) -> viaForm() -> followByRedirect(); }
            inline Response * followedForm(const QUrl & url, const QByteArray & data) { return requestTo(url) -> viaForm(data) -> followByRedirect(); }
            inline Response * followedForm(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url) -> withHeaders(headers) -> viaForm() -> followByRedirect(); }

        public slots:
            inline void sendGet(QString & url) { followedGet(url) -> deleteLater(); }
        protected:
            Response * synchronizeRequest(QNetworkReply * m_http);
            QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
        private:
            QSsl::SslProtocol protocol;
            QSslSocket::PeerVerifyMode mode;

            static Cookies * cookies;
            static QHash<QObject *, Manager *> managers;
            friend class ManagerController;
        };

        class ManagerController : public QObject {
            Q_OBJECT
        protected slots:
            inline void disconnectThread() {
                qDebug() << "!!!!!!!!!!!!!!!!!!!! UNREGISTRATE MANAGER";
                Manager * tmanager = Manager::managers.take(sender());
                emit Logger::obj().write(QStringLiteral("Manager"), QStringLiteral("disconnection"));
                if (tmanager) tmanager -> deleteLater();
                deleteLater();
            }
        };
    }
}

#endif // WEB_MANAGER_H
