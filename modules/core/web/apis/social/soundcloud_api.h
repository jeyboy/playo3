#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/apis/service/recaptcha.h"
#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/interfaces/iuser_interaction.h"

#include "soundcloud_request_api.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Api : public ISource, public RequestApi, public IUserInteraction, public Sociable, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { setSearchLimitations(true, true, true, true); }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_sc; }
                inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = qpt_json) { return QUrlQuery(tkn_client_id % val_id_tkn); }

                inline void objectInfoAsync(const QString & uid, Func * func) {
                    ThreadUtils::obj().run((RequestApi *)this, &RequestApi::objectInfo, uid, func);
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = userById(uid);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name, 400);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupById(uid);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name, 400);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }

            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_auth_api_has | sf_auth_site_has | sf_site_offline_credentials_req |
                        sf_content_audio_has |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable | sf_groups_serachable | sf_tags_serachable | sf_genres_serachable |
                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_charteable | sf_recomendable | sf_newable | sf_taggable | sf_genreable |
                        sf_api_auth_mandatory |
                        sf_site_recomendations_auth_only | sf_site_user_content_auth_only
                    );
                }

                bool connectUserApi();
                bool connectUserSite();

                bool hasOfflineCredentials()     { return false; }
                bool takeOfflineCredentials()    { return false; }

                void loadAdditionals(QJsonObject & obj) { Sociable::fromJson(obj); }
                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }

                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        QString full_name = obj.value(tkn_full_name).toString();
                        QString perma = obj.value(tkn_permalink).toString();

                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            full_name.isEmpty() ? obj.value(tkn_username).toString() : full_name,
                            perma,
                            obj.value(tkn_avatar_url).toString()
                        );
                    }
                }

                inline void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            obj.value(QStringLiteral("name")).toString(),
                            obj.value(tkn_permalink).toString(),
                            obj.value(tkn_artwork_url).toString()
                        );
                    }
                }

//                inline bool isRefreshable() { return false; }
                QJsonValue searchProc(const SearchLimit & limitations) {
                    return pRequest(
                        audioSearchUrl(limitations.predicate, limitations.genre, limitations.by_popularity()),
                        call_type_json,
                        rules(limitations.start_offset, limitations.items_limit),
                        0,
                        proc_patch
                    );

//                    return lQuery(
//                        audioSearchUrl(predicate, genre, limitations.by_popularity()),
//                        queryRules(limitations.total_limit),
//                        wrap
//                    );
                }
                inline QString baseUrlStr(const QString & predicate) { return url_base % predicate % val_default_format; }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    return response.value(tkn_response).toArray().size() < arg -> per_request_limit;
                }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & json, int & code, QString & message) {
                    QJsonObject stat_obj = json.value(tkn_response).toObject().value(tkn_errors).toArray().first().toObject();
                    message = stat_obj.value(tkn_error_message).toString();
                    return (code = stat_obj.value(tkn_error_code).toInt()) == 0;
                }                
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
