#ifndef OD_API_H
#define OD_API_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"
#include "modules/core/web/interfaces/sociable.h"

#include "od_request_api.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Api : public RequestApi, public Singleton<Api>, public Sociable {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return QStringLiteral("Od"); }
                inline QString uidStr(const QString & user_id) const { return tkn_alpha % name() % user_id; }
                inline Web::SubType siteType() { return od_site; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("jsessionid=") % token()); }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() {
                    if (!additional().isEmpty())
                        setParams(grabSID(), userID(), additional());

                    if (sessionIsValid()) // maybe use grabSID() ?
                        return true;

                    return false;
                }

                void objectInfo(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, func);
                }

                QString refresh(const QString & refresh_page); // here refresh_page must by eq to track id

            public slots:
                bool connection(bool onlyAuto = false);
                inline void disconnect() {
                    clearParams();
                    clearFriends();
                    clearGroups();
                }
            protected:
                bool hashConnection(bool onlyAuto);
                bool formConnection();

                inline QString baseUrlStr(const QString & predicate) { return base_url % predicate; }

                inline QString offsetKey() const { return offset_key; }
                inline QString limitKey() const { return limit_key; }
                inline int requestLimit() const { return 100; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) {
                    QJsonObject chunk_obj = response.value(QStringLiteral("chunk")).toObject();
                    if (chunk_obj.isEmpty()) return false;
                    return chunk_obj.value(QStringLiteral("count")).toInt() < requestLimit();
                }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) {
        //            QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
        //            message = stat_obj.value(QStringLiteral("error_message")).toString();
                    return true/*(code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0*/;
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
