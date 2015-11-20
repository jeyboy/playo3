#ifndef OD_API_H
#define OD_API_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"
#include "modules/core/web/interfaces/friendable.h"
#include "modules/core/web/interfaces/groupable.h"

#include "od_request_api.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Api : public RequestApi, public Singleton<Api>, public Friendable, public Groupable {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return QStringLiteral("Od"); }
                inline Web::SubType siteType() { return od_site; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("jsessionid=") % token()); }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return !token().isEmpty(); }

                void objectInfo(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, func);
                }

                inline QString refresh(QString refresh_page) { // here refresh_page must by eq to track id
                    QJsonObject obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
                    if (hasError(obj)) {
                        connection(true);
                        obj = Manager::prepare() -> getJson(playAudioUrl(refresh_page));
                        qDebug() << "RECONECTION";
                    }
                    QUrl url(obj.value(QStringLiteral("play")).toString());
                    QUrlQuery query = QUrlQuery(url.query());
                    query.addQueryItem(QStringLiteral("clientHash"), calcMagicNumber(query.queryItemValue(QStringLiteral("md5"))));
                    url.setQuery(query);
                    return url.toString();
                }

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
