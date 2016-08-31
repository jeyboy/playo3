#ifndef WEB_MANAGER_H
#define WEB_MANAGER_H

#include <QtNetwork>
#include <qapplication.h>
#include <qpixmap.h>

#include "modules/core/misc/func.h"
#include "html_parser.h"
#include "web_headers.h"

#define REQUEST_DELAY 260 // ms
#define COOKIES_KEY QStringLiteral("cookies")

#define DEF_JSON_FIELD QStringLiteral("response")
#define USER_AGENT_HEADER_NAME QStringLiteral("User-Agent")

#ifdef Q_OS_WIN
    #define DEFAULT_AGENT QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0") // QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0")
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
            static Response * fromReply(QNetworkReply * reply);

            inline void printHeaders() {
                QList<RawHeaderPair> headers = rawHeaderPairs();

                qDebug() << "------------ HEADERS LIST ----------------";

                for(QList<RawHeaderPair>::ConstIterator it = headers.cbegin(); it != headers.cend(); it++)
                    qDebug() << (*it).first << (*it).second;

                qDebug() << "------------ END OF LIST ----------------";
            }

            inline QByteArray encoding() {
                QString content_type = header(QNetworkRequest::ContentTypeHeader).toString();
                QStringList parts = content_type.split(QStringLiteral("charset="));

                if (parts.length() == 1)
                    return QStringLiteral("utf-8").toUtf8();
                else
                    return parts.last().toUtf8();
            }
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
            QUrlQuery toQuery(bool destroy = true);
            QByteArray toBytes(bool destroy = true);
            QString toText(bool destroy = true);
            QJsonObject toJson(const QString & wrap = QString(), bool destroy = true);
            Html::Document toHtml(bool destroy = true);
            QPixmap toPixmap(bool destroy = true);
            QUrl toUrl(bool destroy = true);
            QUrl toRedirectUrl(bool destroy = true);
            QString toHeader(const QString & header_field, bool destroy = true) {
                QByteArray field_name = header_field.toUtf8();
                if (destroy) deleteLater();

                if (hasRawHeader(field_name))
                    return QString(rawHeader(field_name));

                return QString();
            }
        };

        class Manager;

        class Request : public QNetworkRequest {
        public:
            inline Request(Manager * manager, const QString & url_str) : QNetworkRequest(QUrl(url_str)), manager(manager) {}
            inline Request(Manager * manager, const QUrl & url = QUrl()) : QNetworkRequest(url), manager(manager) {}

            Request withHeaders(const Headers & headers);
            Response * viaGet(bool async = false);
            Response * viaPost(const QString & content_type = QStringLiteral("application/x-www-form-urlencoded"));
            Response * viaPut(const QString & content_type = QStringLiteral("application/x-www-form-urlencoded"));
            Response * viaForm(const QByteArray & data = QByteArray());

            static QByteArray extractParams(QUrl & url) {
                QByteArray params = url.query().toUtf8();
                url.setQuery(QString());
                return params;
            }
        private:
            Manager * manager;
        };

        class Manager : public QNetworkAccessManager {
            Q_OBJECT

            QSsl::SslProtocol protocol;
            QSslSocket::PeerVerifyMode mode;
            int last_code;

            static Cookies * cookies;
            static QHash<QObject *, Manager *> managers;
            friend class ManagerController;
        public:
            static Manager * prepare();

            Manager(QObject * parent = 0, QSsl::SslProtocol protocol = QSsl::TlsV1SslV3, QSslSocket::PeerVerifyMode mode = QSslSocket::VerifyNone);

            void setStatusCode(int code) { last_code = code; }
            int statusCode() { return last_code; }

            static inline void addCookie(const QString & cookie_str) {
                QList<QNetworkCookie> items = QNetworkCookie::parseCookies(cookie_str.toUtf8());
                for(QNetworkCookie & item: items)
                    cookies -> insertCookie(item);
            }

            static void removeCookies(const QUrl & url = QUrl()) {
                const QList<QNetworkCookie> items = url.isEmpty() ? cookies -> allCookies() : cookies -> cookiesForUrl(url);
                for(QList<QNetworkCookie>::ConstIterator cookie = items.cbegin(); cookie != items.cend(); cookie++)
                    cookies -> deleteCookie(*cookie);
            }

            static inline void printCookies() {
//                qDebug() << "COOKIE" << cookies -> allCookies();

                qDebug() << " ----------------------- COOKIES LIST ----------------------------";

                for(QNetworkCookie cookie : cookies -> allCookies())
                    qDebug() << cookie.toRawForm();

                qDebug() << " -----------------------------------------------------------------";
            }
            static inline QString cookiesAsHeaderStr(QUrl url = QUrl(), QHash<QString, bool> acceptable = QHash<QString, bool>()) {
                QString res;
                bool ignore_filter = acceptable.isEmpty();

                const QList<QNetworkCookie> items = url.isEmpty() ? cookies -> allCookies() : cookies -> cookiesForUrl(url);
                for(QList<QNetworkCookie>::ConstIterator cookie = items.cbegin(); cookie != items.cend(); cookie++) {
                    QString name = QString((*cookie).name());
                    if (ignore_filter || acceptable.contains(name))
                        res = res % name % '=' % (*cookie).value() % QStringLiteral("; ");
                }

                return QStringLiteral("Cookie: ") % res;
            }
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

            Response * get(const Request & request, bool async = false) {
                QNetworkReply * m_http = QNetworkAccessManager::get(request);
                return async ? (Response *)m_http : synchronizeRequest(m_http);
            }
            Response * post(const Request & request, const QByteArray & data, bool async = false) {
                QNetworkReply * m_http = QNetworkAccessManager::post(request, data);
                return async ? (Response *)m_http : synchronizeRequest(m_http);
            }

            Response * put(const Request & request, const QByteArray & data, bool async = false) {
                QNetworkReply * m_http = QNetworkAccessManager::put(request, data);
                return async ? (Response *)m_http : synchronizeRequest(m_http);
            }

            inline Request requestTo(const QString & url) {
                Headers headers = Headers::extract(url);
                return Request(this, url).withHeaders(headers);
            }
            inline Request requestTo(const QUrl & url) {
                Headers headers = Headers::extract(url);
                return Request(this, url).withHeaders(headers);
            }

            inline QJsonObject jsonGet(const QUrl & url, const QString & wrap) { return getFollowed(url) -> toJson(wrap); }
            inline QJsonObject jsonGet(const QUrl & url, bool wrap = false) { return getFollowed(url) -> toJson(wrap ? DEF_JSON_FIELD : QString()); }
            inline QJsonObject jsonGet(const QUrl & url, Headers headers, const QString & wrap) { return getFollowed(url, headers) -> toJson(wrap); }
            inline QJsonObject jsonGet(const QUrl & url, Headers headers, bool wrap = false) { return getFollowed(url, headers) -> toJson(wrap ? DEF_JSON_FIELD : QString()); }
            inline QJsonObject jsonPost(const QUrl & url, const QString & wrap) { return postFollowed(url) -> toJson(wrap); }
            inline QJsonObject jsonPost(const QUrl & url, bool wrap = false) { return postFollowed(url) -> toJson(wrap ? DEF_JSON_FIELD : QString()); }
            inline QJsonObject jsonPost(const QUrl & url, Headers headers, const QString & wrap) { return postFollowed(url, headers) -> toJson(wrap); }
            inline QJsonObject jsonPost(const QUrl & url, Headers headers, bool wrap = false) { return postFollowed(url, headers) -> toJson(wrap ? DEF_JSON_FIELD : QString()); }

            inline QPixmap pixmapGet(const QUrl & url) { return getFollowed(url) -> toPixmap(); }
            inline Response * pixmapGetAsync(const QUrl & url, const Func & response) {
                Response * resp = requestTo(url).viaGet(true) -> followByRedirect();
                connect(resp, SIGNAL(finished()), this, SLOT(pixmapRequestFinished()));
                asyncRequests.insert(resp -> url(), response);
                return resp;
            }

            inline Response * getFollowed(const QUrl & url) { return requestTo(url).viaGet() -> followByRedirect(); }
            inline Response * getFollowed(const QUrl & url, const Headers & headers) { return requestTo(url).withHeaders(headers).viaGet() -> followByRedirect(); }
            inline Response * getFollowedAsync(const QUrl & url, const Func & response) {
                Response * resp = requestTo(url).viaGet(true);
                connect(resp, SIGNAL(finished()), this, SLOT(requestFinished()));
                asyncRequests.insert(resp -> url(), response);
                return resp;
            }

            inline Response * postFollowed(const QUrl & url, const QString & content_type = QStringLiteral("application/x-www-form-urlencoded")) {
                return requestTo(url).viaPost(content_type) -> followByRedirect();
            }
            inline Response * postFollowed(const QUrl & url, const Headers & headers, const QString & content_type = QStringLiteral("application/x-www-form-urlencoded")) {
                return requestTo(url).withHeaders(headers).viaPost(content_type) -> followByRedirect();
            }

            inline Response * putFollowed(const QUrl & url, const QString & content_type = QStringLiteral("application/x-www-form-urlencoded")) {
                return requestTo(url).viaPut(content_type) -> followByRedirect();
            }
            inline Response * putFollowed(const QUrl & url, const Headers & headers, const QString & content_type = QStringLiteral("application/x-www-form-urlencoded")) {
                return requestTo(url).withHeaders(headers).viaPut(content_type) -> followByRedirect();
            }

            inline Response * form(const QUrl & url) { return requestTo(url).viaForm(); }
            inline Response * form(const QUrl & url, const Headers & headers) { return requestTo(url).withHeaders(headers).viaForm(); }

            inline Response * formFollowed(const QUrl & url) { return requestTo(url).viaForm() -> followByRedirect(); }
            inline Response * formFollowed(const QUrl & url, const QByteArray & data) { return requestTo(url).viaForm(data) -> followByRedirect(); }
            inline Response * formFollowed(const QUrl & url, const Headers & headers) { return requestTo(url).withHeaders(headers).viaForm() -> followByRedirect(); }
            inline Response * formFollowed(const QUrl & url, const QByteArray & data, const Headers & headers) { return requestTo(url).withHeaders(headers).viaForm(data) -> followByRedirect(); }

        public slots:
            inline void sendGet(QString & url) { getFollowed(url) -> deleteLater(); }
        protected slots:
            inline void requestFinished() {
                Response * source = (Response *)sender();
                Func func = asyncRequests.take(source -> url());
                QUrl new_url = source -> redirectUrl();

                if (!new_url.isEmpty()) {
                    source -> appendHeaders(new_url);
                    getFollowedAsync(new_url, func);
                } else QMetaObject::invokeMethod(func.obj, func.slot, Q_ARG(QIODevice *, source), Q_ARG(void *, func.user_data));
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
