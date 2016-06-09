#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>

#include "modules/core/misc/func.h"
#include "html_parser.h"

#define REQUEST_DELAY 260 // ms
#define COOKIES_KEY QStringLiteral("cookies")

#define DEF_JSON_FIELD QStringLiteral("response")

#ifdef Q_OS_WIN
    #define DEFAULT_AGENT QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:39.0) Gecko/20100101 Firefox/39.0") // QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0")
#elif Q_OS_MAC // there should be agent for mac
    #define DEFAULT_AGENT QStringLiteral("Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:43.0) Gecko/20100101 Firefox/43.0")
#else
    #define DEFAULT_AGENT QStringLiteral("Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:43.0) Gecko/20100101 Firefox/43.0")
#endif

namespace Core { // requests and response has memory leaks
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
            inline QUrl redirectUrl() {
                QVariant possibleRedirectUrl = attribute(QNetworkRequest::RedirectionTargetAttribute);
                if (possibleRedirectUrl.isValid()) {
                    QUrl new_url = possibleRedirectUrl.toUrl();

                    if (new_url.isRelative())
                        new_url = url().resolved(new_url);

                    return new_url;

                } else return QUrl();
            }
            inline void appendHeaders(QUrl & url) {
                QString urlStr = QStringLiteral("%1\r\nReferer: %2").arg(url.toString(), QString(request().rawHeader("Referer")));
                url = QUrl(urlStr);
            }
            inline QString paramVal(const QString & param) { return QUrlQuery(url()).queryItemValue(param); }

            Response * followByRedirect(QHash<QUrl, bool> prev_urls = QHash<QUrl, bool>());
            QString toText(bool destroy = true);
            QJsonObject toJson(const QString & wrap = QString(), bool destroy = true);
            Html::Document toHtml(bool destroy = true);
            QPixmap toPixmap(bool destroy = true);
            QUrl toUrl(bool destroy = true);
            QUrl toRedirectUrl(bool destroy = true);
        };

        class Manager;

        class Request : public QNetworkRequest {
        public:
            inline Request(Manager * manager, const QString & url_str) : QNetworkRequest(QUrl(url_str)), manager(manager) {}
            inline Request(Manager * manager, const QUrl & url = QUrl()) : QNetworkRequest(url), manager(manager) {}

            Request withHeaders(const QHash<QString, QString> & headers);
            Response * viaGet(bool async = false);
            Response * viaPost(const QByteArray & data = QByteArray());
            Response * viaForm(const QByteArray & data = QByteArray());
        private:
            Manager * manager;
        };

        class Manager : public QNetworkAccessManager {
            Q_OBJECT

            QSsl::SslProtocol protocol;
            QSslSocket::PeerVerifyMode mode;

            static Cookies * cookies;
            static QHash<QObject *, Manager *> managers;
            friend class ManagerController;
        public:
            static Manager * prepare();

            Manager(QObject * parent = 0, QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

            static inline void addCookie(const QString & cookie_str) {
                QList<QNetworkCookie> items = QNetworkCookie::parseCookies(cookie_str.toUtf8());
                for(QNetworkCookie & item: items)
                    cookies -> insertCookie(item);
            }
            static inline void printCookies() { qDebug() << "COOKIE" << cookies -> allCookies(); }
            static inline QString cookie(const QString & name, QUrl url = QUrl()) {
                const QList<QNetworkCookie> items = url.isEmpty() ? cookies -> allCookies() : cookies -> cookiesForUrl(url);
                for(QList<QNetworkCookie>::ConstIterator cookie = items.cbegin(); cookie != items.cend(); cookie++)
                    if ((*cookie).name() == name)
                        return QString((*cookie).value());

                return QString();
            }
            static void loadCookies(const QJsonObject & store);
            static void saveCookies(QJsonObject & store, const QUrl & url = QUrl());

            static inline QString paramVal(const QUrl & url, const QString & param) { return QUrlQuery(url).queryItemValue(param); }

            static inline QHash<QString, QString> extractHeaders(QString & url) {
                QStringList heads = url.split(QRegularExpression("%0D%0A|\\r\\n"), QString::SkipEmptyParts);
                QHash<QString, QString> res;
                if (heads.size() > 1) {
                    url = heads.takeFirst();

                    while(!heads.isEmpty()) {
                        QStringList parts =  heads.takeLast().split(QStringLiteral(": "), QString::SkipEmptyParts);
                        res.insert(parts.first(), parts.last());
                    }
                }

                return res;
            }
            static inline QHash<QString, QString> extractHeaders(const QUrl & url) {
                QString urlStr = url.toString();
                QHash<QString, QString> headers = extractHeaders(urlStr);
                if (!headers.isEmpty()) const_cast<QUrl &>(url) = QUrl(urlStr);
                return headers;
            }

            Response * get(const Request & request, bool async = false) {
                QNetworkReply * m_http = QNetworkAccessManager::get(request);
                return async ? (Response *)m_http : synchronizeRequest(m_http);
            }
            Response * post(const Request & request, const QByteArray & data, bool async = false) {
                QNetworkReply * m_http = QNetworkAccessManager::post(request, data);
                return async ? (Response *)m_http : synchronizeRequest(m_http);
            }

            inline Request requestTo(const QString & url) {
                QHash<QString, QString> headers = extractHeaders(const_cast<QString &>(url));
                return Request(this, url).withHeaders(headers);
            }
            inline Request requestTo(const QUrl & url) {
                QHash<QString, QString> headers = extractHeaders(url);
                return Request(this, url).withHeaders(headers);
            }

            inline QPixmap getPixmap(const QUrl & url) { return followedGet(url) -> toPixmap(); }
            inline QJsonObject getJson(const QUrl & url, const QString & wrap) { return followedGet(url) -> toJson(wrap); }
            inline QJsonObject getJson(const QUrl & url, bool wrap = false) { return followedGet(url) -> toJson(wrap ? DEF_JSON_FIELD : QString()); }
            inline QJsonObject postJson(const QUrl & url, bool wrap = false) { return followedPost(url) -> toJson(wrap ? DEF_JSON_FIELD : QString()); }
            inline QJsonObject postJson(const QUrl & url, QHash<QString, QString> headers, bool wrap = false) { return followedPost(url, headers) -> toJson(wrap ? DEF_JSON_FIELD : QString()); }

            inline Response * getPixmapAsync(const QUrl & url, const Func & response) {
                Response * resp = requestTo(url).viaGet(true)-> followByRedirect();
                asyncRequests.insert(resp -> url(), response);
                connect(resp, SIGNAL(finished()), this, SLOT(pixmapRequestFinished()));
                return resp;
            }

//            inline Response * unfollowedGet(const QUrl & url) { return requestTo(url).viaGet(); }
            inline Response * followedGet(const QUrl & url) { return requestTo(url).viaGet() -> followByRedirect(); }
            inline Response * followedGet(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url).withHeaders(headers).viaGet() -> followByRedirect(); }
            inline Response * followedGetAsync(const QUrl & url, const Func & response) {
                Response * resp = requestTo(url).viaGet(true);
                asyncRequests.insert(resp -> url(), response);
                connect(resp, SIGNAL(finished()), this, SLOT(requestFinished()));
                return resp;
            }

            inline Response * followedPost(const QUrl & url) { return requestTo(url).viaPost() -> followByRedirect(); }
            inline Response * followedPost(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url).withHeaders(headers).viaPost() -> followByRedirect(); }

            inline Response * unfollowedForm(const QUrl & url) { return requestTo(url).viaForm(); }
            inline Response * unfollowedForm(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url).withHeaders(headers).viaForm(); }
            inline Response * followedForm(const QUrl & url) { return requestTo(url).viaForm() -> followByRedirect(); }
            inline Response * followedForm(const QUrl & url, const QByteArray & data) { return requestTo(url).viaForm(data) -> followByRedirect(); }
            inline Response * followedForm(const QUrl & url, QHash<QString, QString> headers) { return requestTo(url).withHeaders(headers).viaForm() -> followByRedirect(); }

        public slots:
            inline void sendGet(QString & url) { followedGet(url) -> deleteLater(); }
        protected slots:
            inline void requestFinished() {
                Response * source = (Response *)sender();
                Func func = asyncRequests.take(source -> url());
                QUrl new_url = source -> redirectUrl();

                if (!new_url.isEmpty()) {
                    source -> appendHeaders(new_url);
                    followedGetAsync(new_url, func);
                } else QMetaObject::invokeMethod(func.obj, func.slot, Q_ARG(Response *, source), Q_ARG(void *, func.user_data));
            }

            inline void pixmapRequestFinished() {
                Response * source = (Response *)sender();
                Func func = asyncRequests.take(source -> url());
                QMetaObject::invokeMethod(func.obj, func.slot, Q_ARG(QPixmap, source -> toPixmap()));
            }
        protected:
            QHash<QUrl, Func> asyncRequests;
            Response * synchronizeRequest(QNetworkReply * m_http);
            QNetworkReply * createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);
        };

        class ManagerController : public QObject {
            Q_OBJECT
        protected slots:
            inline void disconnectThread() {
                qDebug() << "!!!!!!!!!!!!!!!!!!!! UNREGISTRATE MANAGER";
                Manager * tmanager = Manager::managers.take(sender());
                if (tmanager) tmanager -> deleteLater();
                deleteLater();
            }
        };
    }
}

#endif // WEB_MANAGER_H
