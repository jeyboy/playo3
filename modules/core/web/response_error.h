#ifndef RESPONSE_ERROR
#define RESPONSE_ERROR

#include <qnetworkreply.h>

namespace Core {
    class ResponseError {
    protected:
        QString ioError(QNetworkReply * reply) {
            switch(reply -> error()) {
                case QNetworkReply::NoError:                            { return QLatin1String("All ok"); }
                case QNetworkReply::ConnectionRefusedError:             { return QLatin1String("Connection: refused"); }
                case QNetworkReply::RemoteHostClosedError:              { return QLatin1String("Connection: host closed"); }
                case QNetworkReply::HostNotFoundError:                  { return QLatin1String("Connection: host not found"); }
                case QNetworkReply::TimeoutError:                       { return QLatin1String("Connection: timeout"); }
                case QNetworkReply::OperationCanceledError:             { return QLatin1String("Connection: operation canceled"); }
                case QNetworkReply::TemporaryNetworkFailureError:       { return QLatin1String("Connection: network is not accessible"); }
                case QNetworkReply::NetworkSessionFailedError:          { return QLatin1String("Connection: network session is not accessible"); }
                case QNetworkReply::BackgroundRequestNotAllowedError:   { return QLatin1String("Connection: background request not allowed"); }
                case QNetworkReply::UnknownNetworkError:                { return QLatin1String("Connection: unknow error"); }

                case QNetworkReply::ProxyConnectionRefusedError:        { return QLatin1String("Proxy: connection refused"); }
                case QNetworkReply::ProxyConnectionClosedError:         { return QLatin1String("Proxy: connection closed"); }
                case QNetworkReply::ProxyNotFoundError:                 { return QLatin1String("Proxy: not found"); }
                case QNetworkReply::ProxyTimeoutError:                  { return QLatin1String("Proxy: timeout"); }
                case QNetworkReply::ProxyAuthenticationRequiredError:   { return QLatin1String("Proxy: authentication required"); }
                case QNetworkReply::UnknownProxyError:                  { return QLatin1String("Proxy: unknow error"); }

                case QNetworkReply::ContentAccessDenied:                { return QLatin1String("Content: access denied"); }
                case QNetworkReply::ContentOperationNotPermittedError:  { return QLatin1String("Content: operation not permitted"); }
                case QNetworkReply::ContentNotFoundError:               { return QLatin1String("Content: not found"); }
                case QNetworkReply::AuthenticationRequiredError:        { return QLatin1String("Content: authentication required"); }
                case QNetworkReply::ContentReSendError:                 { return QLatin1String("Content: resend required"); } //TODO: maybe auto resend ?
                case QNetworkReply::ContentConflictError:               { return QLatin1String("Content: state conflict"); } //TODO: maybe auto resend ?
                case QNetworkReply::ContentGoneError:                   { return QLatin1String("Content: is gone"); }
                case QNetworkReply::UnknownContentError:                { return QLatin1String("Content: unknow error"); }

                case QNetworkReply::ProtocolUnknownError:               { return QLatin1String("Protocol: unknow"); }
                case QNetworkReply::ProtocolInvalidOperationError:      { return QLatin1String("Protocol: invalid operation"); }
                case QNetworkReply::ProtocolFailure:                    { return QLatin1String("Protocol: failure"); }

                case QNetworkReply::InternalServerError:                { return QLatin1String("Server: internal error"); }
                case QNetworkReply::OperationNotImplementedError:       { return QLatin1String("Server: operation not implemented"); }
                case QNetworkReply::ServiceUnavailableError:            { return QLatin1String("Server: service unavailable"); }
                case QNetworkReply::UnknownServerError:                 { return QLatin1String("Server: unknow error"); }
                default: return QLatin1String("Unknow error");
            }
        }
    };
}

#endif // RESPONSE_ERROR
