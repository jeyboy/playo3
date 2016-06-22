#ifndef VK_API_H
#define VK_API_H

// http://vk.com/support

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

//#include "modules/data_struct/search/search_settings.h"
#include "vk_request_api.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Api : public ISource, public RequestApi, public Singleton<Api>, public Sociable {
                Q_OBJECT
                friend class Singleton<Api>;
                inline Api() { setSearchLimitations(true, true, true, true); }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_vk; }

                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(sf_auth_api_has /*| sf_auth_site_has*/ | sf_auth_mandatory);
                }

                inline QUrlQuery genDefaultParams(const QueryParamsType & ptype = qpt_json) {
                    QUrlQuery query = QUrlQuery();

                    if (ptype == qpt_json) {
                        query.addQueryItem(tkn_version, val_version);
                        query.addQueryItem(tkn_access_token, apiToken());
                    }

                    return query;
                }

                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void loadAdditionals(QJsonObject & obj) { Sociable::fromJson(obj); }

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

                QString refresh(const QString & audio_uid, const DataMediaType & /*itemMediaType*/) {
                    return refresh_postproc(audioInfo(audio_uid).value(tkn_url).toString());
                }

                QString refresh_postproc(const QString & refreshed_url) { return refreshed_url.section('?', 0, 0); }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByIdOrPerma(uid);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByIdOrPerma(uid);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }

                bool connectUserApi();
                bool connectUserSite() { return false; } // TODO: write me
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }

            protected:
                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();
                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            QString(obj.value(QStringLiteral("first_name")).toString() % ' ' % obj.value(QStringLiteral("last_name")).toString()),
                            obj.value(tkn_screen_name).toString(),
                            obj.value(tkn_photo).toString()
                        );
                    }
                }

                inline void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();
                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            obj.value(QStringLiteral("name")).toString(),
                            obj.value(tkn_screen_name).toString(),
                            obj.value(tkn_photo).toString()
                        );
                    }
                }

                inline QString baseUrlStr(const QString & predicate) { return url_base % predicate; }

//                inline QString offsetKey() const { return tkn_offset; }
//                inline QString limitKey() const { return tkn_limit; }
//                inline int requestLimit() const { return 200; }
//                inline void iterateOffset(int & offset, QJsonObject & response, QUrl & /*url*/) { offset = response.value(offsetKey()).toInt(); }

                inline bool endReached(QJsonObject & response, QueriableArg * /*arg*/) { return response.value(tkn_finished).toBool(); }
                bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message);

                //QUrl Api::buildUrl(QueriableArg * arg) { //(QUrl tUrl, int offset, int limit, const QJsonObject & /*prev_response*/) {
                //    QString urlStr = tUrl.toString();
                //    urlStr = urlStr.replace(tkn_predef1, QString::number(offset)).replace(tkn_predef2, QString::number(limit));
                //    return QUrl(urlStr);
                //}
                bool captchaProcessing(QJsonObject & response, QString & url);
            //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }
            };
        }
    }
}

#endif // VK_API_H
