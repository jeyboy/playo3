#ifndef RESPONSE_ERROR
#define RESPONSE_ERROR

#include <qnetworkreply.h>

namespace Core {
    class ResponseError {
    protected:
        QString ioError(QNetworkReply * reply) {
            switch(reply -> error()) {
                case QNetworkReply::NoError:                            { return QStringLiteral("All ok"); }
                case QNetworkReply::ConnectionRefusedError:             { return QStringLiteral("Connection: refused"); }
                case QNetworkReply::RemoteHostClosedError:              { return QStringLiteral("Connection: host closed"); }
                case QNetworkReply::HostNotFoundError:                  { return QStringLiteral("Connection: host not found"); }
                case QNetworkReply::TimeoutError:                       { return QStringLiteral("Connection: timeout"); }
                case QNetworkReply::OperationCanceledError:             { return QStringLiteral("Connection: operation canceled"); }
                case QNetworkReply::TemporaryNetworkFailureError:       { return QStringLiteral("Connection: network is not accessible"); }
                case QNetworkReply::NetworkSessionFailedError:          { return QStringLiteral("Connection: network session is not accessible"); }
                case QNetworkReply::BackgroundRequestNotAllowedError:   { return QStringLiteral("Connection: background request not allowed"); }
                case QNetworkReply::UnknownNetworkError:                { return QStringLiteral("Connection: unknow error"); }

                case QNetworkReply::ProxyConnectionRefusedError:        { return QStringLiteral("Proxy: connection refused"); }
                case QNetworkReply::ProxyConnectionClosedError:         { return QStringLiteral("Proxy: connection closed"); }
                case QNetworkReply::ProxyNotFoundError:                 { return QStringLiteral("Proxy: not found"); }
                case QNetworkReply::ProxyTimeoutError:                  { return QStringLiteral("Proxy: timeout"); }
                case QNetworkReply::ProxyAuthenticationRequiredError:   { return QStringLiteral("Proxy: authentication required"); }
                case QNetworkReply::UnknownProxyError:                  { return QStringLiteral("Proxy: unknow error"); }

                case QNetworkReply::ContentAccessDenied:                { return QStringLiteral("Content: access denied"); }
                case QNetworkReply::ContentOperationNotPermittedError:  { return QStringLiteral("Content: operation not permitted"); }
                case QNetworkReply::ContentNotFoundError:               { return QStringLiteral("Content: not found"); }
                case QNetworkReply::AuthenticationRequiredError:        { return QStringLiteral("Content: authentication required"); }
                case QNetworkReply::ContentReSendError:                 { return QStringLiteral("Content: resend required"); } //TODO: maybe auto resend ?
                case QNetworkReply::ContentConflictError:               { return QStringLiteral("Content: state conflict"); } //TODO: maybe auto resend ?
                case QNetworkReply::ContentGoneError:                   { return QStringLiteral("Content: is gone"); }
                case QNetworkReply::UnknownContentError:                { return QStringLiteral("Content: unknow error"); }

                case QNetworkReply::ProtocolUnknownError:               { return QStringLiteral("Protocol: unknow"); }
                case QNetworkReply::ProtocolInvalidOperationError:      { return QStringLiteral("Protocol: invalid operation"); }
                case QNetworkReply::ProtocolFailure:                    { return QStringLiteral("Protocol: failure"); }

                case QNetworkReply::InternalServerError:                { return QStringLiteral("Server: internal error"); }
                case QNetworkReply::OperationNotImplementedError:       { return QStringLiteral("Server: operation not implemented"); }
                case QNetworkReply::ServiceUnavailableError:            { return QStringLiteral("Server: service unavailable"); }
                case QNetworkReply::UnknownServerError:                 { return QStringLiteral("Server: unknow error"); }
                default: return QStringLiteral("Unknow error");
            }
        }
    };
}

#endif // RESPONSE_ERROR
