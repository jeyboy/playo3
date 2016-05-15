#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include <qmenu.h>

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/auth/teu_auth.h"
#include "modules/core/web/apis/service/recaptcha.h"
#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

#include "soundcloud_request_api.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Api : public ISource, public RequestApi, public TeuAuth, public Sociable, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_sc; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(tkn_client_id % val_id_tkn); }
                QString authUrl();

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return !token().isEmpty(); }

                void getGroupInfo(QString uid, QJsonObject & object);
                void getUserInfo(QString & uid, QJsonObject & object);

//                QList<Linkable> findFriendById(const QString & uid) {}
//                QList<Linkable> findFriendByName(const QString & name) {}
//                QList<Linkable> findGroupById(const QString & uid) {}
//                QList<Linkable> findGroupByName(const QString & name) {}

                QJsonObject objectInfo(QString & uid);
                inline void objectInfo(QString & uid, Func * func) { ThreadUtils::obj().run(this, &Api::objectInfo, uid, func); }

                QToolButton * initButton(QWidget * parent = 0);
            public slots:
                inline void openTab() { ISource::openTab(userID()); }
//                inline void openRecomendations() { ISource::openRecomendations(userID()); }
                inline void openRelationTab() { ISource::openRelationTab(this); }

                bool connectUser(const ConnectionType & /*conType*/ = connection_restore);
                inline void disconnectUser() {
                    clearParams();
                    clearFriends();
                    clearGroups();
                    initButton();
                }

            protected:
                inline QString refresh(const QString & path) { return path; }
                inline QString baseUrlStr(const QString & predicate) { return url_base % predicate % val_default_format; }

//                inline QString offsetKey() const { return tkn_offset; }
//                inline QString limitKey() const { return tkn_limit; }
//                inline int requestLimit() const { return 200; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(tkn_response).toArray().isEmpty(); }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & response, int & code, QString & message) {
                    QJsonObject stat_obj = response.value(tkn_response).toObject().value(tkn_errors).toArray().first().toObject();
                    message = stat_obj.value(tkn_error_message).toString();
                    return (code = stat_obj.value(tkn_error_code).toInt()) == 0;
                }                
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
