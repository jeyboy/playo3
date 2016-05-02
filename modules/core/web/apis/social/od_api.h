#ifndef OD_API_H
#define OD_API_H

#include <qmenu.h>

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
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_od; }
                inline QUrlQuery genDefaultParams() { return QUrlQuery(tkn_jsessionid % token()); }

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

                QToolButton * initButton(QWidget * parent = 0) {
                    if (button == 0) {
                        if (!parent) {
                            qDebug() << "PIPEC" << name();
                            return 0;
                        }
                        button = new QToolButton(parent);
                    } else {
                        button -> setMenu(0);
                        disconnect(button, SIGNAL(clicked()), this, SLOT(openTab()));
                    }

                    if (isConnected()) {
                        button -> setIcon(QIcon(QStringLiteral(":/add_od_on")));
                        button -> setToolTip(QStringLiteral("Od(ok.ru)"));
                        button -> setPopupMode(QToolButton::InstantPopup);

                        QMenu * menu = new QMenu(button);
                        menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
                        menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));
                //        menu -> addAction(QStringLiteral("Open friend/group tab"), this, SLOT(showSoundcloudRelTabDialog()));
                        button -> setMenu(menu);
                    } else {
                        button -> setIcon(QIcon(QStringLiteral(":/add_od")));
                        button -> setToolTip(QStringLiteral("Connect to Od(ok.ru)"));
                        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
                    }

                    return button;
                }

            public slots:
                void openTab();

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

                inline QString offsetKey() const { return tkn_offset; }
                inline QString limitKey() const { return tkn_limit; }
                inline int requestLimit() const { return 100; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) {
                    QJsonObject chunk_obj = response.value(tkn_chunk).toObject();
                    if (chunk_obj.isEmpty()) return false;
                    return chunk_obj.value(tkn_count).toInt() < requestLimit();
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
