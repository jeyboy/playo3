#ifndef VK_API_H
#define VK_API_H

// http://vk.com/support

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/interfaces/iuser_interaction.h"

//#include "modules/data_struct/search/search_settings.h"
#include "vk_request_api.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Api : public ISource, public RequestApi, public IUserInteraction, public Singleton<Api>, public Sociable {
                Q_OBJECT
                friend class Singleton<Api>;
                inline Api() { setSearchLimitations(true, true, true, true); }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_vk; }

                void userInfoAsync(const QString & uid, const InfoType & info_type, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::userInfo, uid, info_type, func);
                }

                void audioRecomendationsAsync(const QString & uid, bool byUser, bool randomize, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::audioRecomendations, uid, byUser, randomize, func);
                }

                QString refresh(const QString & audio_uid, const DataMediaType & /*itemMediaType*/) { // TODO: update for video refresh
                    return refresh_postproc(audioInfo(audio_uid).value(tkn_url).toString());
                }

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

            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_recomendable |
                        sf_auth_api_has /*| sf_auth_site_has*/ | sf_auth_mandatory
                    );
                }

                bool connectUserApi();
                bool connectUserSite() { return false; } // TODO: write me

                QString refresh_postproc(const QString & refreshed_url) { return refreshed_url.section('?', 0, 0); }

                QJsonValue popular(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(DMT_AUDIO, audioPopular(true, limits.genre));

                    if (limits.include_video())
                        res.insert(DMT_VIDEO, videoPopular(limits));

                    return res;
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() && limits.by_popularity())
                        return popular(limits);
                    else {
                        QJsonObject res;

                        if (limits.include_audio())
                            res.insert(DMT_AUDIO, audioSearch(limits));

                        if (limits.include_video())
                            res.insert(DMT_VIDEO, videoSearch(limits));

                        return res;
                    }
                }

                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_json) {
                    QUrlQuery query = QUrlQuery();

                    if (stype == qst_json) {
                        query.addQueryItem(tkn_version, val_version);
                        query.addQueryItem(tkn_access_token, apiToken());
                    }

                    return query;
                }

                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void loadAdditionals(QJsonObject & obj) { Sociable::fromJson(obj); }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }

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

                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) { return url_base % predicate; }

                inline bool endReached(QJsonObject & response, QueriableArg * /*arg*/) { return response.value(tkn_finished).toBool(); }
                bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message);

                bool captchaProcessing(QJsonObject & response, QString & url);
            //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }
            };
        }
    }
}

#endif // VK_API_H
