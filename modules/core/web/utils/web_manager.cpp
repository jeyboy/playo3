#include "web_manager.h"

namespace Core {
    namespace Web {
        Response * Response::followByRedirect(QHash<QUrl, bool> prev_urls) {
            QUrl new_url = redirectUrl();
            if (!new_url.isEmpty()) {
                if (prev_urls.contains(new_url)) return this;
                else prev_urls.insert(new_url, true);

                appendHeaders(new_url);
                deleteLater();
                return ((Manager *)manager()) -> requestTo(new_url) -> viaGet() -> followByRedirect(prev_urls);
            }

            return this;
        }

        QString Response::toText(bool destroy) {
            QByteArray ar = readAll();
            if (destroy) deleteLater();
            return QString(ar);
        }
        QJsonObject Response::toJson(const QString & wrap, bool destroy) {
            if (error()) qDebug() << "IOERROR" << error();
            QByteArray ar = readAll();
            if (!wrap.isEmpty()) { ar.prepend(QStringLiteral("{\"%1\":").arg(wrap).toUtf8()); ar.append("}"); }
            if (destroy) deleteLater();
            return QJsonDocument::fromJson(ar).object();
        }
        QPixmap Response::toImage(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error();
            QImage image;
            image.loadFromData(readAll());
            if (destroy) deleteLater();
            return QPixmap::fromImage(image);
        }

        Html::Document Response::toHtml(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error();
            Html::Document doc(this);
            if (destroy) deleteLater();
            return doc;
        }

        QUrl Response::toUrl(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error();
            QUrl uri = url();
            if (destroy) deleteLater();
            return uri;
        }

        QUrl Response::toRedirectUrl(bool destroy) {
            if (error()) qDebug() << "IOERROR" << error();
            QUrl uri = redirectUrl();
            if (destroy) deleteLater();
            return uri;
        }

        //////////////////////////     WEB_REQUEST     /////////////////////////////

        Request * Request::withHeaders(const QHash<QString, QString> & headers) {
            for(QHash<QString, QString>::ConstIterator header = headers.cbegin(); header != headers.cend(); header++)
                setRawHeader(header.key().toUtf8(), header.value().toUtf8());

            if (!headers.contains(QStringLiteral("Referer")))
                setRawHeader("Referer", url().path().toUtf8());

            if (!headers.contains(QStringLiteral("User-Agent")))
                setRawHeader("User-Agent", DEFAULT_AGENT.toUtf8());

            return this;
        }

        Response * Request::viaGet(bool async) { return manager -> get(*this, async); }

        Response * Request::viaPost(const QByteArray & data) { return manager -> post(*this, data); }

        Response * Request::viaForm(const QByteArray & data) {
            setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
            return manager -> post(*this, data);
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
            : QNetworkAccessManager(parent) {
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
