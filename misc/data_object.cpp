#include "data_object.h"

DataObject::DataObject(QUrl & url, uint buffer_length) : bufferLength(buffer_length), error("") {
    obj_url = url;
}

DataObject::~DataObject() {
    closeConnection();
}

QString DataObject::lastError() const { return error; }

void DataObject::download(QUrl savePath, int & progressIndicator) {
    emit start(relation);

    if (!m_http && !openSync())
        return;

    if (!savePath.isLocalFile()) {
        emit onError("File: remote saving is not supported");
        return;
    }

    QFile file(savePath.toLocalFile());

//    QIODevice::Append | QIODevice::Unbuffered

    if (!file.open(QIODevice::WriteOnly)) {
        emit onError("File: cannot open file for writing");
    } else {
        if (!file.resize(m_http -> bytesAvailable())) {
            emit onError("File: free memory on disk is not enough");
        } else {
            QByteArray buffer;
            qint64 pos = 0;
            double limit = m_http -> bytesAvailable();

            while(!m_http -> atEnd()) {
                try {
                    buffer = m_http -> read(bufferLength);
                    pos += buffer.length();
                    file.write(buffer);

                    emit progress(relation, (pos / limit) * 100);
                }

                catch(...) {
                    emit onError(QNetworkReply::UnknownNetworkError);
                }
            }

            file.close();
            emit end(relation, true);
        }
    }
}

void WebObject::onError(QString er) {
    error = er;
    emit end(relation, false);
}

void WebObject::onSslErrors(const QList<QSslError> &errors) {
    qDebug() << "SSL ERROR";
}

void WebObject::onError(QNetworkReply::NetworkError er) {
    switch (er) {
        case QNetworkReply::NoError: { error = ""; }

        case QNetworkReply::ConnectionRefusedError: { error = "Connection: refused"; }
        case QNetworkReply::RemoteHostClosedError: { error = "Connection: host closed"; }
        case QNetworkReply::HostNotFoundError: { error = "Connection: host not found"; }
        case QNetworkReply::TimeoutError: { error = "Connection: timeout"; }
        case QNetworkReply::OperationCanceledError: { error = "Connection: operation canceled"; }
        case QNetworkReply::TemporaryNetworkFailureError: { error = "Connection: network is not accessible"; }
        case QNetworkReply::NetworkSessionFailedError: { error = "Connection: network session is not accessible"; }
        case QNetworkReply::BackgroundRequestNotAllowedError: { error = "Connection: background request not allowed"; }
        case QNetworkReply::UnknownNetworkError: { error = "Unknow error"; }

        case QNetworkReply::ProxyConnectionRefusedError: { error = "Proxy: connection refused"; }
        case QNetworkReply::ProxyConnectionClosedError: { error = "Proxy: connection closed"; }
        case QNetworkReply::ProxyNotFoundError: { error = "Proxy: not found"; }
        case QNetworkReply::ProxyTimeoutError: { error = "Proxy: timeout"; }
        case QNetworkReply::ProxyAuthenticationRequiredError: { error = "Proxy: authentication required"; }
        case QNetworkReply::UnknownProxyError: { error = "Proxy: unknow error"; }

        case QNetworkReply::ContentAccessDenied: { error = "Content: access denied"; }
        case QNetworkReply::ContentOperationNotPermittedError: { error = "Content: operation not permitted"; }
        case QNetworkReply::ContentNotFoundError: { error = "Content: not found"; }
        case QNetworkReply::AuthenticationRequiredError: { error = "Content: authentication required"; }
        case QNetworkReply::ContentReSendError: { error = "Content: resend required"; } //TODO: maybe auto resend ?
        case QNetworkReply::ContentConflictError: { error = "Content: state conflict"; } //TODO: maybe auto resend ?
        case QNetworkReply::ContentGoneError: { error = "Content: is gone"; }
        case QNetworkReply::UnknownContentError: { error = "Content: unknow error"; }

        case QNetworkReply::ProtocolUnknownError: { error = "Protocol: unknow"; }
        case QNetworkReply::ProtocolInvalidOperationError: { error = "Protocol: invalid operation"; }
        case QNetworkReply::ProtocolFailure: { error = "Protocol: failure"; }

        case QNetworkReply::InternalServerError: { error = "Server: internal error"; }
        case QNetworkReply::OperationNotImplementedError: { error = "Server: operation not implemented"; }
        case QNetworkReply::ServiceUnavailableError: { error = "Server: service unavailable"; }
        case QNetworkReply::UnknownServerError: { error = "Server: unknow error"; }
    }

    emit end(relation, false);
}

