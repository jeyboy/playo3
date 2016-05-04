#ifndef VK_API_H
#define VK_API_H

#include <qmenu.h>

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
                inline DataSubType siteType() const { return dt_site_vk; }

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

                void userInfo(QString & uid, InfoType info_type, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, info_type, func);
                }
            //    void wallAudio(QString & uid, Func func) {
            //        registerAsync(
            //            QtConcurrent::run((RequestApi *)this, &RequestApi::wallAudio, uid), func
            //        );
            //    }
                void audioRecomendations(QString & uid, bool byUser, bool randomize, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::audioRecomendations, uid, byUser, randomize, func);
                }

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
                        button -> setIcon(QIcon(QStringLiteral(":/add_vk_on")));
                        button -> setPopupMode(QToolButton::InstantPopup);
                        button -> setToolTip(QStringLiteral("VKontakte(vk.com)"));

                        QMenu * menu = new QMenu(button);
                        menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
                        menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));
                        menu -> addAction(QStringLiteral("Open friend/group tab"), this, SLOT(openRelationTab()));
                        menu -> addAction(QStringLiteral("Open recommendations"), this, SLOT(openRecomendations()));
                        button -> setMenu(menu);
                    } else {
                        button -> setIcon(QIcon(QStringLiteral(":/add_vk")));
                        button -> setToolTip(QStringLiteral("Connect to VKontakte(vk.com)"));
                        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
                    }

                    return button;
                }

            public slots:
                inline void openTab() { ISource::openTab(userID()); }
                inline void openRecomendations() { ISource::openRecomendations(userID()); }
                inline void openRelationTab() { ISource::openRelationTab(this); }

                bool connectUser(const ConnectionType & /*conType*/ = connection_restore);
                inline void disconnectUser() {
                    clearParams();
                    clearFriends();
                    clearGroups();
                    initButton();
                }
            protected:
                inline QString baseUrlStr(const QString & predicate) { return url_base % predicate; }

                inline QString offsetKey() const { return tkn_offset; }
                inline QString limitKey() const { return tkn_limit; }
                inline int requestLimit() const { return 200; }
                inline void iterateOffset(int & offset, QJsonObject & response, QUrl & /*url*/) { offset = response.value(offsetKey()).toInt(); }

                inline QJsonObject & extractBody(QJsonObject & response) { return (response = response.value(tkn_response).toObject()); }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(tkn_finished).toBool(); }
                bool extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message);

                QUrl buildUrl(QUrl tUrl, int offset, int limit, const QJsonObject & prev_response);
                bool captchaProcessing(QJsonObject & response, QUrl & url);
            //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }
            };
        }
    }
}

#endif // VK_API_H
