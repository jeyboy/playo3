#ifndef YANDEX
#define YANDEX

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"
//#include "modules/core/web/interfaces/sociable.h"

#include "yandex_request_api.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Api : public RequestApi, public Singleton<Api>/*, public Sociable*/ {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() {
                    // need to add checking presents of cookies
                    Manager::prepare() -> followedGet(QUrl(url_root));
                }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() { return dt_site_yandex; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(); }

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
                    QJsonObject obj = Manager::prepare() -> getJson(trackDownloadingUrl(refresh_page));
                    obj = Manager::prepare() -> getJson(QString(obj.value(QStringLiteral("src")).toString() % QStringLiteral("&format=json")));
                    QString path = obj.value(QStringLiteral("path")).toString();
                    return QStringLiteral("https://%1/get-mp3/%2/%3%4?track-id=%5&play=false&").arg(
                        obj.value(QStringLiteral("host")).toString(), calcKey(path, obj.value(QStringLiteral("s")).toString()),
                        obj.value(QStringLiteral("ts")).toString(), path, refresh_page.split(':').first()
                    );
                }

            public slots:
                bool connection(bool /*onlyAuto*/ = false) { return true; }
                inline void disconnect() {}
            protected:
//                bool hashConnection(bool onlyAuto);
//                bool formConnection();

                inline QString baseUrlStr(const QString & predicate) { return url_root % predicate; }

                inline QString offsetKey() const { return QString(); /*tkn_offset;*/ }
                inline QString limitKey() const { return QString(); /*tkn_limit;*/ }
                inline int requestLimit() const { return 100; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & /*response*/, int /*offset*/) {
//                    QJsonObject chunk_obj = response.value(tkn_chunk).toObject();
//                    if (chunk_obj.isEmpty()) return false;
//                    return chunk_obj.value(tkn_count).toInt() < requestLimit();
                    return true;
                }
                inline bool extractStatus(QUrl & /*url*/, QUrl & responseUrl, QJsonObject & response, int & /*code*/, QString & /*message*/) {
                    if (response.isEmpty()) {
                        if (responseUrl.toString().startsWith(QStringLiteral("https://music.yandex.ua/showcaptcha")))
                            Logger::obj().write("Yandex API", "CAPTCHA"); // proceed captcha
                    }
        //            QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
        //            message = stat_obj.value(QStringLiteral("error_message")).toString();
                    return true/*(code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0*/;
                }
            };
        }
    }
}

#endif // YANDEX
