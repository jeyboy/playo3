#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/web_api.h"
#include "modules/core/web/auth_chemas/teu_auth.h"
#include "modules/core/interfaces/singleton.h"
#include "fourshared_request_api.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Api: public WebApi, public TeuAuth, public RequestApi, public Singleton<Api> {
                Q_OBJECT
            public:
                inline QString name() const { return QStringLiteral("4shared"); }
                inline Web::SubType siteType() { return fourshared_site; }
                inline QUrlQuery genDefaultParams() {
                    return QUrlQuery(
                        QStringLiteral("oauth_consumer_key=") %
                            (token().isEmpty() ? QStringLiteral("22abeb63487b7f6b75051079b7e610b1") : token())
                    );
                }
                QString authUrl();

                void fromJson(QJsonObject hash);
                QJsonObject toJson();

                inline bool isConnected() { return !token().isEmpty(); }
        //        QString downloadLink();

            public slots:
                inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
                void proceedAuthResponse(const QUrl & url);

            protected:
                inline QString baseUrlStr(const QString & predicate) { return base_url % predicate % ".json"; }

                inline QString offsetKey() const { return offset_key; }
                inline QString limitKey() const { return limit_key; }
                inline int requestLimit() const { return 100; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("files")).toArray().isEmpty(); }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) {
        //            QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
        //            message = stat_obj.value(QStringLiteral("error_message")).toString();
        //            return (code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0;
                    return true;
                }
            private:
                friend class Singleton<Api>;
                inline Api() { }
            };
        }
    }
}

#endif // FOURSHARED_API
