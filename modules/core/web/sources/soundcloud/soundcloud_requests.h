#ifndef SOUNDCLOUD_REQUESTS
#define SOUNDCLOUD_REQUESTS

#include "soundcloud_auth.h"
#include "soundcloud_group.h"
#include "soundcloud_playlist.h"
#include "soundcloud_set.h"
#include "soundcloud_audio.h"
#include "soundcloud_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Requests : public Sociable, public Auth, public Group,
                     public Playlist, public Set, public Audio, public User
            {
            protected:
                //                inline SourceFlags defaultFlags() {
                //                    return (SourceFlags)(
                //                        sf_auth_api_has | /*sf_auth_site_has |*/ |
                //                        sf_items_serachable | sf_sets_serachable | sf_users_serachable |
                //                        sf_groups_serachable | sf_by_tags_serachable | sf_by_genres_serachable |
                //                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_packable |
                //                        sf_recommendable_by_item | sf_newable | sf_taggable | sf_genreable |
                //                        sf_recommendable_by_user | sf_api_auth_mandatory |
                //                        sf_site_object_content_auth_only
                //                    );
                //                }

                Requests() {
                    setSociableLimitations(true, true, false, false/*true, true*/);

                    flags = {
                        {sf_endpoint, (SourceFlags)(
                            sf_offline_credentials_req | sf_is_primary | sf_audio | sf_user/*sf_sociable*/ |
                            sf_feed | sf_playlist | sf_compilation | sf_site | sf_api | sf_api_connectable)
                        },

//                        {sf_feed,                   sf_site},

                        {sf_feed_by_user,               sf_both_auth},
//                        {sf_feed_by_group,          sf_site_auth_only},

                        {sf_is_shareable,               sf_site_not_api_auth},

                        {sf_search,                     sf_site_not_api_auth},

                        {sf_compilation,                sf_site_not_api_auth},

//                        {sf_popular_audio,          sf_site_auth_only},
                        {sf_like_audio,                 sf_both_auth},
                        {sf_audio_by_id,                sf_site_not_api_auth},
                        {sf_audio_by_title,             sf_site_not_api_auth},
                        {sf_audio_by_genre,             sf_site_not_api_auth},
                        {sf_audio_by_tag,               sf_site_not_api_auth},
                        {sf_audio_by_group,             sf_site_not_api_auth},
                        {sf_audio_by_like,              sf_site_not_api_auth},
                        {sf_audio_by_user,              sf_site_not_api_auth},
                        {sf_audio_recs_by_user,         sf_site},
                        {sf_audio_recs_by_audio,        sf_site_not_api_auth},

//                        {sf_audio_playlist_by_id,   sf_site_auth_only},
                        {sf_audio_playlist_by_title,    sf_site_not_api_auth},
                        {sf_audio_playlist_by_tag,      sf_site},
                        {sf_audio_playlist_by_audio,    sf_site_not_api_auth},
                        {sf_audio_playlist_by_user,     sf_site_not_api_auth},

                        {sf_user_sociable,              sf_site_not_api_auth},
//                        {sf_group_sociable,             sf_site_not_api_auth},

                        {sf_user_by_id,                 sf_site_not_api_auth},
                        {sf_user_by_title,              sf_site_not_api_auth},
                        {sf_user_by_likes,              sf_site_not_api_auth},
                        {sf_user_by_audio_repost,       sf_site_not_api_auth},

//                        {sf_group_by_user,              sf_site_not_api_auth},
//                        {sf_group_by_title,             sf_site_not_api_auth},
//                        {sf_group_by_id,                sf_site_not_api_auth},
//                        {sf_group_by_audio,             sf_site}
                    };
                }
                inline virtual ~Requests() {}

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    if (!isConnected()) takeOfflineCredentials();

                    switch(stype) {
                        case qst_api: return url_api_base % predicate % val_default_format;
                        case qst_site: return url_api_base % predicate;
                        case qst_site_alt1: return url_api_base2 % predicate;
                        default: return QString();
                    }
                }

                inline bool endReached(QJsonObject & /*response*/, QueriableArg * arg) {
                    return arg -> amount_of_appends == 0 || arg -> amount_of_appends < arg -> per_request_limit;
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
                    bool res = connectApi(new_token, user_id, error);

                    if (res) {
                        setApiToken(new_token);
                        setApiUserID(user_id);
                    }

                    return res;
                }
                bool connectUserSite() { return connectSite(); } // not realized yet

                bool hasOfflineCredentials()     { return !siteToken().isEmpty(); }
                bool takeOfflineCredentials();

                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
//                    takeOfflineCredentials();
                }
                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                    takeOfflineCredentials();
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonObject friends_res = usersById(uid).toObject();
                    jsonToUsers(linkables, EXTRACT_ITEMS(friends_res));

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonObject friends_res = usersByName(name, 0, 400).toObject();
                    jsonToUsers(linkables, EXTRACT_ITEMS(friends_res));

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonObject groups_res = groupsById(uid).toObject();
                    jsonToGroups(linkables, EXTRACT_ITEMS(groups_res));

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonObject groups_res = groupsByName(name, 0, 400).toObject();
                    jsonToGroups(linkables, EXTRACT_ITEMS(groups_res));

                    return linkables;
                }
            public:
                // standart step for all search requests is 10

                // mixed search with autocomplete - users / groups/ tracks
                //'https://api-v2.soundcloud.com/search/autocomplete?q=s&queries_limit=0&results_limit=10&limit=10&offset=0&linked_partitioning=1'

                // mixed search - users / groups/ tracks
                // curl 'https://api-v2.soundcloud.com/search?q=dubstep&sc_a_id=184de48a-5c8d-4ef7-908a-b4602d2a667d&facet=model&user_id=301086-912545-767108-258358&client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1466770454' -H 'Host: api-v2.soundcloud.com' -H 'DNT: 1' -H 'Referer: https://soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive' -H 'Pragma: no-cache' -H 'Cache-Control: no-cache'

                QString refresh(const QString & track_id, const DataMediaType & /*itemMediaType*/) {
                    return audioUrl(track_id);
                }

                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api) {
                    switch(stype) {
                        case qst_api:
                            return QUrlQuery(tkn_client_id % val_id_tkn);
                        case qst_site:
                        case qst_site_alt1:
                            return QUrlQuery(tkn_client_id % siteToken() % LSTR("&app_version=") % siteHash());
                        default: return QUrlQuery();
                    }
                }

                QJsonValue audioRecommendations(const QString & track_id, int offset = 0, int amount = SOUNDCLOUD_ITEMS_LIMIT) {
                    return QJsonArray() << Audio::audioRecommendations(track_id, offset, amount);
                }

                QJsonValue userAudioRecommendationsAsync(const QString & user_id, int offset = 0, int amount = SOUNDCLOUD_ITEMS_LIMIT) {
                    return QJsonArray() << User::userAudioRecommendations(user_id, offset, amount);
                }

                QJsonValue objectInfo(const QString & oid) {
                    if (oid[0] == '-')
                        return QJsonArray() << groupInfo(oid.mid(1));
                    else
                        return userInfo(oid);
                }

                QJsonValue groupInfo(const QString & group_id) {
                    return audioByGroup(group_id);
                }

                QJsonValue userInfo(const QString & user_id) {
                    QJsonArray blocks;

                    QJsonObject json = audioByUser(user_id).toObject();
                    json.insert(tkn_dir_name, LSTR("Tracks"));
                    blocks << json;

                    json = audioByUserLikes(user_id).toObject();
                    json.insert(tkn_dir_name, LSTR("Likes"));
                    blocks << json;

                    json = playlistsByUser(user_id).toObject();
                    json.insert(tkn_dir_name, LSTR("Playlists"));
                    blocks << json;

                    if (user_id == userID()) { // ignore socials for not current user
                        clearFriends();
                        QThread::msleep(REQUEST_DELAY);
                        jsonToUsers(Friendable::linkables, EXTRACT_ITEMS(userFollowings(user_id).toObject()));
                        jsonToUsers(Friendable::linkables, EXTRACT_ITEMS(userFollowers(user_id).toObject()));
                        clearGroups();
                        QThread::msleep(REQUEST_DELAY);
                        jsonToGroups(Groupable::linkables, EXTRACT_ITEMS(groupsByUser(user_id).toObject()));
                    }

                    return blocks;
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUESTS
