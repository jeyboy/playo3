#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/teu_auth.h"
//#include "modules/core/web/interfaces/friendable.h"
//#include "modules/core/web/interfaces/groupable.h"

#include "fourshared_request_api.h"

namespace Core {
    namespace Web {
        namespace Fourshared { // auth is not realised yet
            class Api: public TeuAuth, public RequestApi, public Singleton<Api> {
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

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return !token().isEmpty(); }
        //        QString downloadLink();

            public slots:
                inline void disconnect() { clearParams(); }

            protected:
                inline QString baseUrlStr(const QString & predicate) { return base_url % predicate % ".json"; }

                inline QString offsetKey() const { return offset_key; }
                inline QString limitKey() const { return limit_key; }
                inline int requestLimit() const { return 100; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("files")).toArray().isEmpty(); }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) { return true; } // stub
            private:
                friend class Singleton<Api>;
                inline Api() { }
            };
        }
    }
}

#endif // FOURSHARED_API
