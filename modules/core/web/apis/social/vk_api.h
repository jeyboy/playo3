#ifndef VK_API_H
#define VK_API_H

#include <qmenu.h>

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/auth/teu_auth.h"
#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

//#include "modules/data_struct/search/search_settings.h"
#include "vk_request_api.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Api : public ISource, public TeuAuth, public RequestApi, public Singleton<Api>, public Sociable {
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

                QToolButton * initButton(QWidget * parent = 0);

                QString refresh(const QString & audio_uid) {
                    return refresh_postproc(audioInfo(audio_uid).value(tkn_url).toString());
                }

                QString refresh_postproc(const QString & refreshed_url) { return refreshed_url.section('?', 0, 0); }

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

                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(tkn_finished).toBool(); }
                bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message);

                //QUrl Api::buildUrl(QueriableArg * arg) { //(QUrl tUrl, int offset, int limit, const QJsonObject & /*prev_response*/) {
                //    QString urlStr = tUrl.toString();
                //    urlStr = urlStr.replace(tkn_predef1, QString::number(offset)).replace(tkn_predef2, QString::number(limit));
                //    return QUrl(urlStr);
                //}
                bool captchaProcessing(QJsonObject & response, QUrl & url);
            //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }
            };
        }
    }
}

#endif // VK_API_H
