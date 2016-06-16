#ifndef YANDEX
#define YANDEX

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"
//#include "modules/core/web/interfaces/sociable.h"
#include "modules/core/interfaces/isource.h"

#include "yandex_request_api.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Api : public RequestApi, public ISource, public Singleton<Api>/*, public Sociable*/ {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() {
                    // need to add checking presents of cookies
                    Manager::prepare() -> getFollowed(QUrl(url_root));
                }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_yandex; }
                inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = qpt_json) { return QUrlQuery(); }

                void fromJson(const QJsonObject & /*hash*/) {}
                void toJson(QJsonObject & /*hash*/) {}

                inline bool isConnected() {
//                    if (!additional().isEmpty())
//                        setParams(grabSID(), userID(), additional());

//                    return sessionIsValid();

                    return true;
                }

//                void objectInfo(const QString & uid, Func * func) {
//                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, func);
//                }

                QString refresh(const QString & refresh_page) { // here refresh_page must by eq to track id
                    QJsonObject obj = Manager::prepare() -> jsonGet(trackDownloadingUrl(refresh_page));
                    obj = Manager::prepare() -> jsonGet(QString(obj.value(QStringLiteral("src")).toString() % QStringLiteral("&format=json")));
                    QString path = obj.value(QStringLiteral("path")).toString();
                    return QStringLiteral("https://%1/get-mp3/%2/%3%4?track-id=%5&play=false&").arg(
                        obj.value(QStringLiteral("host")).toString(), calcKey(path, obj.value(QStringLiteral("s")).toString()),
                        obj.value(QStringLiteral("ts")).toString(), path, refresh_page.split(':').first()
                    );
                }

            public slots:
                bool connectUser(const ConnectionType & /*conType*/ = connection_restore) { return true; }
                inline void disconnectUser() {}
            protected:
//                bool hashConnection(bool onlyAuto);
//                bool formConnection();

                inline QString baseUrlStr(const QString & predicate) { return url_root % predicate; }

//                inline QString offsetKey() const { return QString(); /*tkn_offset;*/ }
//                inline QString limitKey() const { return QString(); /*tkn_limit;*/ }
//                inline int requestLimit() const { return 100; }

                inline bool endReached(QJsonObject & /*response*/, QueriableArg * /*arg*/) { return true; }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return true; }
            };
        }
    }
}

#endif // YANDEX
