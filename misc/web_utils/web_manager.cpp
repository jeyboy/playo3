#include "web_manager.h"

QHash<QObject *, WebManager *> WebManager::managers = QHash<QObject *, WebManager *>();

WebManager * WebManager::manager() {
    QThread * thread = QThread::currentThread();
    if (!managers.contains(thread)) {
        qDebug() << "!!!!!!!!!!!!!!!!!!!! REGISTRATE MANAGER";
        managers.insert(thread, new WebManager());

        //QApplication::instance() -> thread()

        if (QThread::currentThread() != QApplication::instance() -> thread())
            connect(thread, SIGNAL(finished()), new WebManagerController(), SLOT(disconnectThread()));
    }
    return managers[thread];
}

WebManager::WebManager(QObject * parent, QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode)
    : QNetworkAccessManager(parent) {
    this -> protocol = protocol;
    this -> mode = mode;
}

QJsonObject WebManager::getJson(const QNetworkRequest & request, bool wrap) {
    QNetworkReply * reply = getSync(request);
    QJsonObject res = replyToJson(reply, wrap);
    reply -> deleteLater();
    return res;
}
QJsonObject WebManager::postJson(const QNetworkRequest & request, const QByteArray & data, bool wrap) {
    QNetworkReply * reply = postSync(request, data);
    QJsonObject res = replyToJson(reply, wrap);
    reply -> deleteLater();
    return res;
}

QNetworkReply * WebManager::openUrl(QUrl & url) { // TODO: need to prevent from url cicling
    QNetworkReply * m_http = getSync(QNetworkRequest(url));

    QVariant possibleRedirectUrl = m_http -> attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (possibleRedirectUrl.isValid()) {
        QUrl new_url = possibleRedirectUrl.toUrl();
        if (new_url.isRelative()) {
            new_url = url.resolved(new_url); // need to check this
        }

        m_http -> deleteLater();
        return openUrl(new_url);
    }

    return m_http;
}

QPixmap WebManager::openImage(QUrl & url) {
    QImage image;
    QNetworkReply * reply = openUrl(url);
    image.loadFromData(reply -> readAll());
    reply -> deleteLater();
    return QPixmap::fromImage(image);
}

QNetworkReply * WebManager::synchronizeRequest(QNetworkReply * m_http) {
    QEventLoop loop;
    connect(m_http, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return m_http;
}

QNetworkReply * WebManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData) {
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(mode);
    config.setProtocol(protocol);
    (const_cast<QNetworkRequest &>(req)).setSslConfiguration(config);
    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}
