#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H


#include "modules/core/web/interfaces/web_api.h"
#include "modules/core/web/interfaces/teu_auth.h"
#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/apis/service/recaptcha.h"

#include "soundcloud_request_api.h"
#include "soundcloud_api_keys.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Api : public WebApi, public TeuAuth, public RequestApi, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return QStringLiteral("Soundcloud"); }
                inline SubType siteType() { return sc_site; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("client_id=8f84790a84f5a5acd1c92e850b5a91b7")); }
                QString authUrl();

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return !token().isEmpty(); }

                void getGroupInfo(QString uid, QJsonObject & object);
                void getUserInfo(QString & uid, QJsonObject & object);

                QJsonObject objectInfo(QString & uid);
                inline void objectInfo(QString & uid, Func * func) {
                    ThreadUtils::obj().run(this, &Api::objectInfo, uid, func);
                }
            public slots:
                inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
                bool connection();

            protected:
                inline QString refresh(QString path) { return path; }
                inline QString baseUrlStr(const QString & predicate) { return base_url % predicate % ".json"; }

                inline QString offsetKey() const { return offset_key; }
                inline QString limitKey() const { return limit_key; }
                inline int requestLimit() const { return 200; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("response")).toArray().isEmpty(); }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & response, int & code, QString & message) {
                    QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
                    message = stat_obj.value(QStringLiteral("error_message")).toString();
                    return (code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0;
                }                
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
