#ifndef SOUNDCLOUD_REQUEST_API
#define SOUNDCLOUD_REQUEST_API

#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/interfaces/ishareable.h"
#include "modules/core/web/interfaces/auth/teu_auth.h"
#include "soundcloud_api_keys.h"

#define SOUNDCLOUD_PAGES_LIMIT 25
#define SOUNDCLOUD_ITEMS_LIMIT 10000
#define SOUNDCLOUD_PER_REQUEST_LIMIT 200

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class RequestApi : public TeuAuth, public IApi, public IShareable {
            private:
                inline void setAudioTypesParam(QUrlQuery & query) { setParam(query, tkn_types, val_audio_types); }

                // add to search
                inline void setAudioTypesParamOriginal(QUrlQuery & query) { setParam(query, tkn_types, val_audio_org_types); }
                inline void setAudioTypesParamRemix(QUrlQuery & query) { setParam(query, tkn_types, val_audio_rmx_types); }

                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_q, predicate); }
                inline void setIdsFilter(QUrlQuery & query, const QStringList & uids) { setParam(query, tkn_ids, uids.join(',')); }
                inline void setGenreLimitation(QUrlQuery & query, const QString & genre) { setParam(query, tkn_genres, genre); }
                inline void setOrder(QUrlQuery & query, bool hottest) { setParam(query, tkn_order, hottest ? val_hotness_order : val_created_at_order); }
                inline void setPagination(QUrlQuery & query, int per_request = SOUNDCLOUD_PER_REQUEST_LIMIT) {
                    setParam(query, tkn_offset, OFFSET_TEMPLATE);
                    setParam(query, tkn_limit, per_request);
                }

                PolyQueryRules rules(int items_limit = SOUNDCLOUD_ITEMS_LIMIT, int pages_count = SOUNDCLOUD_PAGES_LIMIT, int offset = 0, ApiCallIterType call_type = call_iter_type_page) {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, SOUNDCLOUD_ITEMS_LIMIT),
                        qMin(pages_count, SOUNDCLOUD_PAGES_LIMIT),
                        offset
                    );
                }
            public:
                inline virtual ~RequestApi() {}

                /////////////////
                /// AUTH
                ////////////////

                //QString authTokenUrl() const {
                //    QUrl url("https://api.soundcloud.com/oauth2/token");
                //    QUrlQuery query = genDefaultParams();


                //    query.addQueryItem("client_secret", val_client_tkn);
                //    query.addQueryItem("scope", "non-expiring");

                ////    query.addQueryItem("grant_type", "password");
                ////    query.addQueryItem("username", "USERNAME");
                ////    query.addQueryItem("password", "PASSWORD");

                //    url.setQuery(query);
                //    return url.toString();
                //}
                inline QUrl authTokenUrl() const { return QUrl(url_auth_token); }
                inline QString confirmAuthUrl(const QString & access_token) { return url_auth_confirm % access_token; }

                QByteArray authTokenUrlParams(QString code) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_client_secret, val_client_tkn);
                    setParam(query, tkn_grant_type, val_grant_type);
                    setParam(query, tkn_redirect_uri, url_redirect);
                    setParam(query, tkn_code, code);

                    return query.toString(QUrl::FullyEncoded).toUtf8();
                }

                /////////////////
                /// API
                ////////////////
                QString audioSearchUrl(const QString & predicate, const QString & genre, bool hottest = false) {
                    QUrlQuery query = genDefaultParams();
                    setAudioTypesParam(query);
                    setOrder(query, hottest);
                    setPagination(query);

                    if (!genre.isEmpty())
                        setGenreLimitation(query, genre);

                    if (!predicate.isEmpty())
                        setSearchPredicate(query, predicate);

                    return baseUrlStr(path_tracks, query);
                }

                QJsonArray popular(const SearchLimit & limitations) {
                    return pRequest(
                        audioSearchUrl(QString(), limitations.genre, true),
                        call_type_json,
                        rules(),
                        proc_json_wrap
                    );
//                    return lQuery(
//                        audioSearchUrl(QString(), genre, true),
//                        queryRules(100),
//                        wrap
//                    );
                }

                QJsonArray search_proc(const SearchLimit & limitations) {
                    return pRequest(
                        audioSearchUrl(limitations.predicate, limitations.genre, limitations.by_popularity()),
                        call_type_json,
                        rules(limitations.items_limit),
                        proc_json_wrap
                    );

//                    return lQuery(
//                        audioSearchUrl(predicate, genre, limitations.by_popularity()),
//                        queryRules(limitations.total_limit),
//                        wrap
//                    );
                }


                QString groupAudioUrl(const QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setAudioTypesParam(query);
                    setPagination(query);
                    return baseUrlStr(path_group_tracks.arg(uid), query);
                }
                QJsonArray groupAudio(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        groupAudioUrl(group_id),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(groupAudioUrl(group_id), queryRules(count), wrap);
                }


                QString groupPlaylistsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    return baseUrlStr(path_group_playlists.arg(uid), query);
                }
                QJsonArray groupPlaylists(QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        groupPlaylistsUrl(group_id),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );
                }


                QString audioInfoUrl(const QString & audio_uid) { return baseUrlStr(path_track % audio_uid, genDefaultParams()); }
                QJsonObject audioInfo(const QString & audio_uid) {
                    return sRequest(
                        audioInfoUrl(audio_uid),
                        call_type_json
                    );

//                    return sQuery(audioInfoUrl(audio_uid));
                }


                QString audioUrl(const QStringList & audio_uids) {
                    QUrlQuery query = genDefaultParams();
                    setIdsFilter(query, audio_uids);
                    return baseUrlStr(path_tracks, query);
                }
                //"id": 142370360,
                //"permalink": "sam-smith-stay-with-me",
                QJsonArray audioInfo(const QStringList & audio_uids) {
                    return sRequest(
                        audioUrl(audio_uids),
                        call_type_json,
                        proc_json_wrap
                    ).value(tkn_response).toArray();

//                    return sQuery(audioUrl(audio_uids), wrap).value(tkn_response).toArray();
                }


                QString userAudioUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    return baseUrlStr(path_user_tracks.arg(uid), query);
                }
                QJsonArray userAudio(QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userAudioUrl(uid),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(userAudioUrl(uid), queryRules(count), wrap);
                }


                QString userPlaylistsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    return baseUrlStr(path_user_playlists.arg(uid), query);
                }
                QJsonArray userPlaylists(QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userPlaylistsUrl(uid),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(userPlaylistsUrl(uid), queryRules(count), wrap);
                }


                QString userFollowingsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    return baseUrlStr(path_user_followings.arg(uid), query);
                }
                QJsonArray userFollowings(QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userFollowingsUrl(uid),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(userFollowingsUrl(uid), queryRules(count), wrap);
                }


                QString userFollowersUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    return baseUrlStr(path_user_followers.arg(uid), query);
                }
                QJsonArray userFollowers(QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userFollowersUrl(uid),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(userFollowersUrl(uid), queryRules(count), wrap);
                }


                QString userGroupsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    return baseUrlStr(path_user_groups.arg(uid), query);
                }
                QJsonArray userGroups(QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userGroupsUrl(uid),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(userGroupsUrl(uid), queryRules(count), wrap);
                }


                QString userByNameUrl(QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    setSearchPredicate(query, name);
                    return baseUrlStr(path_users, query);
                }

                QJsonArray usersByName(QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userByNameUrl(name),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(userByNameUrl(name), queryRules(count), wrap);
                }
                QJsonArray userById(QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        baseUrlStr(path_users % uid, genDefaultParams()),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );
//                    return lQuery(baseUrl(path_users % uid, genDefaultParams()), queryRules(count), wrap);
                }


                QString groupByNameUrl(QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    setSearchPredicate(query, name);
                    return baseUrlStr(path_users, query);
                }

                QJsonArray groupByName(QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        groupByNameUrl(name),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(groupByNameUrl(name), queryRules(count), wrap);
                }

                QString groupByIdUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query);
                    return baseUrlStr(path_groups % uid, query);
                }
                QJsonArray groupById(QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        groupByIdUrl(uid),
                        call_type_json,
                        rules(count),
                        proc_json_wrap
                    );

//                    return lQuery(baseUrl(path_groups % uid, genDefaultParams()), queryRules(count), wrap);
                }

                QString playlistByNameUrl(QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setPagination(query, 2); // playlists is very weighted for loading - so set limitation to 2 playlists per request
                    setSearchPredicate(query, name);
                    return baseUrlStr(path_playlists, query);
                }
                QJsonArray playlistByName(QString & name, int count = 10, int offset = 0) {
                    return pRequest(
                        playlistByNameUrl(name),
                        call_type_json,
                        rules(count, SOUNDCLOUD_PAGES_LIMIT, offset),
                        proc_json_wrap
                    );

//                    return lQuery(playlistByNameUrl(name), queryRules(count, offset, 2), wrap);
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUEST_API
