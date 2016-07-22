#ifndef OD_API_H
#define OD_API_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"

#include "od/od_request_api.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Api : public RequestApi, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_od; }
                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_site_def) { return QUrlQuery(tkn_jsessionid % siteToken()); }

                void saveAdditionals(QJsonObject & obj) {
                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_root));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }

                void objectInfo(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, func);
                }

                QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType); // here refresh_page must by eq to track id

                bool connectUserSite() { return formConnection() && takeCredentials(); }
                bool takeCredentials() {
                    if (!siteHash().isEmpty())
                        setSiteToken(grabSID());
                    return sessionIsValid();
                }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }
            protected:
                bool formConnection();

                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) { return url_root % predicate; }

//                inline QString offsetKey() const { return tkn_offset; }
//                inline QString limitKey() const { return tkn_limit; }
//                inline int requestLimit() const { return 100; }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    QJsonObject chunk_obj = response.value(tkn_chunk).toObject();
                    if (chunk_obj.isEmpty()) return false;
                    return chunk_obj.value(tkn_count).toInt() < arg -> per_request_limit/*OD_LIMIT_PER_REQUEST*/;
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
