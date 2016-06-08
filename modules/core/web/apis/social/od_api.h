#ifndef OD_API_H
#define OD_API_H

//#include <qmenu.h>

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/thread_utils.h"
#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

#include "od_request_api.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Api : public RequestApi, public ISource, public Singleton<Api>, public Sociable {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_od; }
                inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = json) { return QUrlQuery(tkn_jsessionid % token()); }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool connectionRequired() { return true; }
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

                QToolButton * initButton(QWidget * parent = 0);
            public slots:
                inline void openTab() { ISource::openTab(userID()); }
//                inline void openRecomendations() { ISource::openRecomendations(userID()); }
//                inline void openRelationTab() { ISource::openRelationTab(this); }

                bool connectUser(const ConnectionType & /*conType*/ = connection_restore);//connect_user(bool onlyAuto = false);
                inline void disconnectUser() {
                    clearParams();
                    clearFriends();
                    clearGroups();

                    if (button)
                        initButton();
                }
            protected:
                bool hashConnection(bool onlyAuto);
                bool formConnection();

                inline QString baseUrlStr(const QString & predicate) { return url_root % predicate; }

//                inline QString offsetKey() const { return tkn_offset; }
//                inline QString limitKey() const { return tkn_limit; }
//                inline int requestLimit() const { return 100; }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    QJsonObject chunk_obj = response.value(tkn_chunk).toObject();
                    if (chunk_obj.isEmpty()) return false;
                    return chunk_obj.value(tkn_count).toInt() < arg -> per_request_limit/*OD_LIMIT_PER_REQUEST*/;
                }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return true; }
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
