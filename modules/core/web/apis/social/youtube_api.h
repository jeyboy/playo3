#ifndef YOUTUBE_API_H
#define YOUTUBE_API_H

#include "modules/core/interfaces/singleton.h"
//#include "modules/core/web/interfaces/teu_auth.h"
//#include "modules/core/web/apis/service/recaptcha.h"

//#include "modules/core/web/interfaces/sociable.h"

#include "youtube_request_api.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Api : /*public TeuAuth,*/ public RequestApi, public Singleton<Api>/*, public Sociable*/ {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline SubType siteType() { return site_youtube; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(); }
                QString authUrl();

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return true; /*!token().isEmpty();*/ }

            public slots:
                bool connection();
                inline void disconnect() {
//                    clearParams();
//                    clearFriends();
//                    clearGroups();
                }

            protected:
                inline QString refresh(const QString & path) { return path; }
                inline QString baseUrlStr(const QString & predicate) { return url_base % predicate % val_default_format; }

                inline QString offsetKey() const { return tkn_offset; }
                inline QString limitKey() const { return tkn_limit; }
                inline int requestLimit() const { return 200; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(tkn_response).toArray().isEmpty(); }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) {
//                    QJsonObject stat_obj = response.value(tkn_response).toObject().value(tkn_errors).toArray().first().toObject();
//                    message = stat_obj.value(tkn_error_message).toString();
//                    return (code = stat_obj.value(tkn_error_code).toInt()) == 0;
                    return true;
                }                
            };
        }
    }
}

#endif // YOUTUBE_API_H
