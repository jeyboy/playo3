#include "web_manager.h"

#include "web_connection.h"

namespace Core {
    namespace Web {
        Response * Response::fromReply(QNetworkReply * reply) {
            Response * resp = (Response *)reply;
            ((Manager *)reply -> manager()) -> setStatusCode(resp -> status());
            return resp;
        }

        Response * Response::followByRedirect(QHash<QUrl, bool> prev_urls) {
            QUrl new_url = redirectUrl();
            if (!new_url.isEmpty()) {
                if (prev_urls.contains(new_url)) return this;
                else prev_urls.insert(new_url, true);

                appendHeaders(new_url);
                deleteLater();
                return ((Manager *)manager()) -> requestTo(new_url).viaGet() -> followByRedirect(prev_urls);
            }

            return this;
        }

        QUrlQuery Response::toQuery(bool destroy) {
            QByteArray ar = readAll();
            if (destroy) deleteLater();
            return QUrlQuery(QUrl::fromPercentEncoding(ar));
        }

        QByteArray Response::toBytes(bool destroy) {
            QByteArray ar = readAll();
            printHeaders();
            if (destroy) deleteLater();
            return ar;
        }

        QString Response::toText(bool destroy) {
            QByteArray ar = readAll();
            QByteArray enc = encoding();
            qDebug() << "ENC" << enc;
            QTextCodec * codec = QTextCodec::codecForName(enc);

            if (destroy) deleteLater();

            return codec -> toUnicode(ar);
        }
        QJsonObject Response::toJson(const QString & wrap, bool destroy) { //TODO: enc not used yet
            if (error()) qDebug() << "IOERROR" << error() << url();

            QByteArray ar = readAll();

            QByteArray header = rawHeader("Content-Type");

            if (header.startsWith("application/json") || header.startsWith("application/javascript")) {
//                ar.replace('\'', '"'); // ' wraps responds to errors on parsing // need to replace ' with "
                if (!wrap.isEmpty()) { ar.prepend(QStringLiteral("{\"%1\":").arg(wrap).toUtf8()); ar.append("}"); }
                if (destroy) deleteLater();
                QJsonParseError err;
                QJsonObject ret =  QJsonDocument::fromJson(ar, &err).object();
                if (err.error == NoError) return ret;
            }

            qCritical() << "NOT JSON" << rawHeader("Content-Type");
            return QJsonObject {{JSON_ERR_FIELD, QString(ar)}};
        }
        QPixmap Response::toPixmap(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error() << url();
            QPixmap image;
            image.loadFromData(readAll());
            if (destroy) deleteLater();
            return image;
        }

        QUrl Response::toUrl(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error() << url();
            QUrl uri = url();
            if (destroy) deleteLater();
            return uri;
        }

        Html::Document Response::toHtml(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error() << url();
            QByteArray enc = encoding();
            qDebug() << "ENC" << enc;

            Html::Document doc(this, UnicodeDecoding::toCharsetType(enc));
            if (destroy) deleteLater();
            return doc;
        }

        QUrl Response::toRedirectUrl(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error() << url();
            QUrl uri = redirectUrl();
            if (destroy) deleteLater();
            return uri;
        }

        //////////////////////////     WEB_REQUEST     /////////////////////////////

        Request Request::withHeaders(const Headers & headers) {
            for(Headers::ConstIterator header = headers.cbegin(); header != headers.cend(); header++)
                setRawHeader(header.key().toUtf8(), header.value().toUtf8());

            // in some cases referer broke the link - od is one of it
//            if (!headers.contains(QStringLiteral("Referer")))
//                setRawHeader("Referer", url().path().toUtf8());

            if (!headers.contains(USER_AGENT_HEADER_NAME))
                setRawHeader(USER_AGENT_HEADER_NAME.toUtf8(), DEFAULT_AGENT.toUtf8());

            return *this;
        }

        Response * Request::viaGet(bool async) { return manager -> get(*this, async); }

        Response * Request::viaPost(const QString & content_type) {
            QUrl curl = url();
            QByteArray payload = manager -> isExtractParamsToPayload() ? extractParams(curl) : QByteArray();
            setUrl(curl);

            setHeader(QNetworkRequest::ContentTypeHeader, content_type);
            return manager -> post(*this, payload);
        }

        Response * Request::viaPut(const QByteArray & data, const QString & content_type) {
            QByteArray payload = data;

            if (payload.isEmpty()) {
                QUrl curl = url();
                payload = manager -> isExtractParamsToPayload() ? extractParams(curl) : QByteArray();
                setUrl(curl);
            }

//            QUrl curl = url();
//            QByteArray payload = manager -> isExtractParamsToPayload() ? extractParams(curl) : QByteArray();
//            setUrl(curl);

            setHeader(QNetworkRequest::ContentTypeHeader, content_type);
            return manager -> put(*this, payload);
        }

        Response * Request::viaForm(const QByteArray & data) {
            setHeader(QNetworkRequest::ContentTypeHeader, FORM_URLENCODE);

            QByteArray payload = data;

            if (payload.isEmpty()) {
                QUrl curl = url();
                payload = manager -> isExtractParamsToPayload() ? extractParams(curl) : QByteArray();
                setUrl(curl);
            }

//            QUrl curl = url();
//            QByteArray payload = manager -> isExtractParamsToPayload() ? extractParams(curl) : QByteArray();
//            setUrl(curl);

//            if (!data.isEmpty()) {
//                if (!payload.isEmpty()) {
//                    payload = payload + '&' + data;
//                } else payload = data;
//            }

            return manager -> post(*this, payload);
        }

        //////////////////////////     WEB_MANAGER     /////////////////////////////

        QHash<QObject *, Manager *> Manager::managers = QHash<QObject *, Manager *>();
        Cookies * Manager::cookies = new Cookies(QApplication::instance());

        void Manager::loadCookies(const QJsonObject & store) {
            QJsonArray arr = store.value(COOKIES_KEY).toArray();
            for(QJsonValue const & cookie: arr) {
                QList<QNetworkCookie> items = QNetworkCookie::parseCookies(cookie.toString().toUtf8());
                for(QNetworkCookie & item: items)
                    cookies -> insertCookie(item);
            }
        }

        void Manager::saveCookies(QJsonObject & store, const QUrl & url) {
            QList<QNetworkCookie> cookiesList = url.isEmpty() ? cookies -> allCookies() : cookies -> cookiesForUrl(url);
            QJsonArray cookiesArray;
            for(auto const & cookie: cookiesList)
                cookiesArray << QJsonValue::fromVariant(cookie.toRawForm());

            store.insert(COOKIES_KEY, cookiesArray);
        }

        Manager * Manager::prepare() {
            QThread * thread = QThread::currentThread();
            if (!managers.contains(thread)) {
                qDebug() << "!!!!!!!!!!!!!!!!!!!! REGISTRATE MANAGER";
                managers.insert(thread, new Manager());

                if (thread != QApplication::instance() -> thread())
                    connect(thread, SIGNAL(finished()), new ManagerController(), SLOT(disconnectThread()));
            }
            return managers[thread];
        }

        Manager::Manager(QObject * parent, QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode)
            : QNetworkAccessManager(parent), extract_params_to_payload(true) {
            this -> protocol = protocol;
            this -> mode = mode;
            this -> setCookieJar(Manager::cookies);
        }

        Response * Manager::synchronizeRequest(QNetworkReply * m_http) {
//            QTimer timer;
//            timer.setSingleShot(true);

            QEventLoop loop;
//            connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
            connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
//            timer.start(10000);   // 10 secs. timeout
            loop.exec();

            //    if(timer.isActive()) {
            //        timer.stop();
            //        if(m_http -> error() > 0) {
            //          ... // handle error
            //        }
            //        else {
            //          int v = m_http -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            //          if (v >= 200 && v < 300) {  // Success
            //            ...
            //          }
            //        }
            //    } else {
            //       // timeout
            //       disconnect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));

            //       m_http -> abort();
            //    }

            if (m_http -> error() == Response::HostNotFoundError)
                WebConnection::obj().check();

            return Response::fromReply(m_http);
        }

        QNetworkReply * Manager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
            QSslConfiguration config = req.sslConfiguration();
            config.setPeerVerifyMode(mode);
            config.setProtocol(protocol);
            (const_cast<QNetworkRequest &>(req)).setSslConfiguration(config);
            return QNetworkAccessManager::createRequest(op, req, outgoingData);
        }
    }
}
