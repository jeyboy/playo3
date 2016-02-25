#ifndef VK_API_H
#define VK_API_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/teu_auth.h"
#include "modules/core/web/interfaces/sociable.h"

//#include "modules/data_struct/search/search_settings.h"
#include "vk_request_api.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Api : public TeuAuth, public RequestApi, public Singleton<Api>, public Sociable {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() {}
            public:
                inline QString name() const { return val_name; }
                inline Web::SubType siteType() { return site_vk; }
                inline QUrlQuery genDefaultParams() {
                    QUrlQuery query = QUrlQuery();

                    query.addQueryItem(tkn_version, val_version);
                    query.addQueryItem(tkn_access_token, token());
                    query.addQueryItem(QStringLiteral("test_mode"), "1");

                    return query;
                }

                inline QString authUrl() { return RequestApi::authUrl(); }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return !token().isEmpty() && !userID().isEmpty(); }

                void userInfo(QString & uid, bool fullInfo, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, fullInfo, func);
                }
            //    void wallAudio(QString & uid, Func func) {
            //        registerAsync(
            //            QtConcurrent::run((RequestApi *)this, &RequestApi::wallAudio, uid), func
            //        );
            //    }
                void audioRecomendations(QString & uid, bool byUser, bool randomize, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::audioRecomendations, uid, byUser, randomize, func);
                }

            public slots:
                bool connection();
                inline void disconnect() {
                    clearParams();
                    clearFriends();
                    clearGroups();
                }
            protected:
                inline QString baseUrlStr(const QString & predicate) { return url_base % predicate; }

                inline QString offsetKey() const { return tkn_offset; }
                inline QString limitKey() const { return tkn_limit; }
                inline int requestLimit() const { return 200; }
                inline void iterateOffset(int & offset, QJsonObject & response, QUrl & /*url*/) { offset = response.value(offsetKey()).toInt(); }

                inline QJsonObject & extractBody(QJsonObject & response) { return (response = response.value(tkn_response).toObject()); }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(tkn_finished).toBool(); }
                bool extractStatus(QUrl & url, QUrl & /*responseUrl*/, QJsonObject & response, int & code, QString & message);

                QUrl buildUrl(QUrl tUrl, int offset, int limit, const QJsonObject & prev_response);
                bool captchaProcessing(QJsonObject & response, QUrl & url);
            //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }
            };
        }
    }
}

#endif // VK_API_H
