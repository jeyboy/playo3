#include "web_manager.h"

namespace Core {
    namespace Web {
        Response * Response::followByRedirect() {
            QVariant possibleRedirectUrl = redirectUrl();
            if (possibleRedirectUrl.isValid()) {
                QUrl new_url = possibleRedirectUrl.toUrl();
                if (new_url.isRelative())
                    new_url = url().resolved(new_url);

                deleteLater();
                return ((Manager *)manager()) -> requestTo(new_url) -> viaGet() -> followByRedirect();
            }

            return this;
        }
        QJsonObject Response::toJson(const QString & wrap) {
            qDebug() << "IOERROR" << error();
            QByteArray ar = readAll();
            if (!wrap.isEmpty()) { ar.prepend(QStringLiteral("{\"%1\":").arg(wrap).toUtf8()); ar.append("}"); }
            deleteLater();
            return QJsonDocument::fromJson(ar).object();
        }
        QPixmap Response::toImage() {
            qDebug() << "IOERROR" << error();
            QImage image;
            image.loadFromData(readAll());
            deleteLater();
            return QPixmap::fromImage(image);
        }

        //////////////////////////     WEB_REQUEST     /////////////////////////////

        Request * Request::withHeaders(const QHash<QString, QString> & headers) {
            for(QHash<QString, QString>::ConstIterator header = headers.cbegin(); header != headers.cend(); header++)
                setRawHeader(header.key().toUtf8(), header.value().toUtf8());

            if (!headers.contains(QStringLiteral("Referer")))
                setRawHeader("Referer", url().path().toUtf8());

            return this;
        }

        Response * Request::viaGet() {
            return manager -> get(*this);
        }

        Response * Request::viaPost(const QByteArray & data) {
            return manager -> post(*this, data);
        }

        Response * Request::viaForm(const QByteArray & data) {
            setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
            return manager -> post(*this, data);
        }



        //////////////////////////     WEB_MANAGER     /////////////////////////////


        QHash<QObject *, Manager *> Manager::managers = QHash<QObject *, Manager *>();
        Cookies * Manager::cookies = new Cookies(QApplication::instance());

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
            QEventLoop loop;
            connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
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
