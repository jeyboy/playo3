#ifndef SOUNDCLOUD_REQUESTS
#define SOUNDCLOUD_REQUESTS

#include "api/requests.h"
#include "site/requests.h"

#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Requests : public ISource, public Sociable, public Api::Requests, public Site::Requests, public Singleton<Requests> {
                friend class Singleton<Requests>;
            protected:
                inline virtual ~Requests() {}

                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_auth_api_has | sf_auth_site_has | sf_site_offline_credentials_req |
                        sf_content_audio_has |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable |
                        sf_groups_serachable | sf_tags_serachable | sf_genres_serachable |
                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_charteable |
                        sf_recomendable_by_item | sf_newable | sf_taggable | sf_genreable |
                        sf_api_auth_mandatory |
                        sf_site_recomendations_auth_only | sf_site_user_content_auth_only
                    );
                }

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_api_def: return url_api_base % predicate % val_default_format;
                        case qst_site_def: return url_api_base % predicate;
                        case qst_site_alt1: return url_api_base2 % predicate;
                        default: return QString();
                    }
                }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    return response.value(tkn_response).toArray().size() < arg -> per_request_limit;
                }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & json, int & code, QString & message) {
                    QJsonObject stat_obj = json.value(tkn_response).toObject().value(tkn_errors).toArray().first().toObject();
                    message = stat_obj.value(tkn_error_message).toString();
                    return (code = stat_obj.value(tkn_error_code).toInt()) == 0;
                }

                void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr);
                void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr);

                bool connectUserApi() {
                    QString new_token, user_id;
                    bool res = Api::Requests::connectUser(new_token, user_id, error);

                    if (res) {
                        setApiToken(new_token);
                        setApiUserID(user_id);
                    }

                    return res;
                }

                bool connectUserSite() { return Site::Requests::connectUser(); } // not realized yet

                bool hasOfflineCredentials()     { return !siteToken().isEmpty(); }
                bool takeOfflineCredentials();

                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    takeOfflineCredentials();
                }
                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }

            public:
                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api_def) {
                    switch(stype) {
                        case qst_api_def:
                            return QUrlQuery(tkn_client_id % val_id_tkn);
                        case qst_site_def:
                        case qst_site_alt1:
                            return QUrlQuery(tkn_client_id % siteToken() % QStringLiteral("app_version=") % siteHash());
                        default: return QUrlQuery();
                    }
                }

                //                inline QHash<QString, QString> customsList() { return QHash<QString, QString>(); }
                //                inline QJsonValue procCustom(const QString & /*custom_params*/) { return QJsonArray(); }


                QJsonObject groupInfo(const QString & group_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::groupInfo(group_id);
                        case perm_api: return Api::Requests::groupInfo(group_id);
                        default: Logger::obj().write("Soundcloud", "group info is not accessable", true);
                    }
                }
                QJsonValue groupsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::groupsByTrack(track_id, count, offset);
                        case perm_api: return Api::Requests::groupsByTrack(track_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by track is not accessable", true);
                    }
                }
                QJsonValue groupsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::groupsByUser(user_id, count, offset);
                        case perm_api: return Api::Requests::groupsByUser(user_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::groupsByName(name, count, offset);
                        case perm_api: return Api::Requests::groupsByName(name, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }

                QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::playlistsByTag(tag, count, offset);
                        case perm_api: return Api::Requests::playlistsByTag(tag, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::playlistsByTrack(track_id, count, offset);
                        case perm_api: return Api::Requests::playlistsByTrack(track_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue playlistsByPredicate(const QString & predicate, int count = 10, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::playlistsByPredicate(predicate, count, offset);
                        case perm_api: return Api::Requests::playlistsByPredicate(predicate, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::playlistsByUser(user_id, count, offset);
                        case perm_api: return Api::Requests::playlistsByUser(user_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }

                QJsonValue setByType(const SetType & chartType, const QString & genre = QString()) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::setByType(chartType, genre);
                        case perm_api: return Api::Requests::setByType(chartType, genre);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }

                QString trackUrl(const QString & track_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::trackUrl(track_id);
                        case perm_api: return Api::Requests::trackUrl(track_id);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                bool likeTrack(const QString & user_id, const QString & track_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::likeTrack(user_id, track_id);
                        case perm_api: return Api::Requests::likeTrack(user_id, track_id);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonObject trackInfo(const QString & track_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::trackInfo(track_id);
                        case perm_api: return Api::Requests::trackInfo(track_id);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue tracksInfo(const QStringList & track_ids) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::tracksInfo(track_ids);
                        case perm_api: return Api::Requests::tracksInfo(track_ids);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue trackRelations(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::trackRelations(track_id, count, offset);
                        case perm_api: return Api::Requests::trackRelations(track_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue tracksSearch(const SearchLimit & limitations) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::tracksSearch(limitations);
                        case perm_api: return Api::Requests::tracksSearch(limitations);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }

                QJsonValue tracksByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::tracksByTag(tag, count, offset);
                        case perm_api: return Api::Requests::tracksByTag(tag, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue tracksByGroup(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::tracksByGroup(group_id, count, offset);
                        case perm_api: return Api::Requests::tracksByGroup(group_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue tracksByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::tracksByUser(user_id, count, offset);
                        case perm_api: return Api::Requests::tracksByUser(user_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue tracksByUserLikes(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::tracksByUserLikes(user_id, count, offset);
                        case perm_api: return Api::Requests::tracksByUserLikes(user_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }

                QJsonValue usersById(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::usersById(user_id, count, offset);
                        case perm_api: return Api::Requests::usersById(user_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::usersByName(name, count, offset);
                        case perm_api: return Api::Requests::usersByName(name, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue usersByTrackLikes(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::usersByTrackLikes(track_id, count, offset);
                        case perm_api: return Api::Requests::usersByTrackLikes(track_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue usersByTrackReposting(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::usersByTrackReposting(track_id, count, offset);
                        case perm_api: return Api::Requests::usersByTrackReposting(track_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue userFollowings(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::userFollowings(user_id, count, offset);
                        case perm_api: return Api::Requests::userFollowings(user_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
                QJsonValue userFollowers(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site: return Site::Requests::userFollowers(user_id, count, offset);
                        case perm_api: return Api::Requests::userFollowers(user_id, count, offset);
                        default: Logger::obj().write("Soundcloud", "group by user is not accessable", true);
                    }
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUESTS
