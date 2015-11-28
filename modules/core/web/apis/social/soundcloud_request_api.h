#ifndef SOUNDCLOUD_REQUEST_API
#define SOUNDCLOUD_REQUEST_API

#include "modules/core/web/interfaces/iapi.h"
#include "soundcloud_api_keys.h"

#define SOUNDCLOUD_OFFSET_LIMIT 100

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class RequestApi : public IApi {
            private:
                inline void setAudioTypesParam(QUrlQuery & query) { setParam(query, tkn_types, val_audio_types); }

                // add to search
                inline void setAudioTypesParamOriginal(QUrlQuery & query) { setParam(query, tkn_types, val_audio_org_types); }
                inline void setAudioTypesParamRemix(QUrlQuery & query) { setParam(query, tkn_types, val_audio_rmx_types); }

                inline void setSearchPredicate(QUrlQuery & query, QString & predicate) { setParam(query, tkn_q, predicate); }
                inline void setIdsFilter(QUrlQuery & query, QStringList & uids) { setParam(query, tkn_ids, uids.join(',')); }
                inline void setGenreLimitation(QUrlQuery & query, QString & genre) { setParam(query, tkn_genres, genre); }
                inline void setOrder(QUrlQuery & query, bool hottest) { setParam(query, tkn_order, hottest ? val_hotness_order : val_created_at_order); }
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
                inline QString confirmAuthUrl(QString access_token) { return url_auth_confirm % access_token; }

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
                QUrl audioSearchUrl(QString & predicate, QString & genre, bool hottest = false) {
                    QUrlQuery query = genDefaultParams();
                    setAudioTypesParam(query);
                    setOrder(query, hottest);

                    if (!genre.isEmpty())
                        setGenreLimitation(query, genre);

                    if (!predicate.isEmpty())
                        setSearchPredicate(query, predicate);

                    return baseUrl(path_tracks, query);
                }

                QJsonArray search_postprocess(QString & predicate, QString & genre, const SearchLimit & limitations) { //count = 5
                    return lQuery(
                        audioSearchUrl(predicate, genre, limitations.by_popularity()),
                        QueryRules(tkn_response, requestLimit(), qMin(limitations.count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }


                QUrl groupAudioUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setAudioTypesParam(query);
                    return baseUrl(path_group_tracks.arg(uid), query);
                }
                QJsonArray groupAudio(QString & group_id, int count = SOUNDCLOUD_OFFSET_LIMIT) {
                //    group_id = "101";
                    return lQuery(
                        groupAudioUrl(group_id),
                        QueryRules(tkn_response, requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }


                QUrl groupPlaylistsUrl(QString & uid) { return baseUrl(path_group_playlists.arg(uid), genDefaultParams()); }
                QJsonArray groupPlaylists(QString & group_id, int count = SOUNDCLOUD_OFFSET_LIMIT) {
                //    group_id = "101";
                    return lQuery(
                        groupPlaylistsUrl(group_id),
                        QueryRules(tkn_response, requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }


                QUrl audioInfoUrl(QString & audio_uid) { return baseUrl(path_tracks % audio_uid, genDefaultParams()); }
                QJsonObject audioInfo(QString audio_uid) { return sQuery(audioInfoUrl(audio_uid)); }


                QUrl audioUrl(QStringList & audio_uids) {
                    QUrlQuery query = genDefaultParams();
                    setIdsFilter(query, audio_uids);
                    return baseUrl(path_tracks, query);
                }
                //"id": 142370360,
                //"permalink": "sam-smith-stay-with-me",
                QJsonArray audioInfo(QStringList & audio_uids) { return sQuery(audioUrl(audio_uids), wrap).value(tkn_response).toArray(); }


                QUrl userAudioUrl(QString & uid) { return baseUrl(path_user_tracks.arg(uid), genDefaultParams()); }
                QJsonArray userAudio(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
                    return lQuery(
                        userAudioUrl(uid),
                        QueryRules(tkn_response, requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }


                QUrl userPlaylistsUrl(QString & uid) { return baseUrl(path_user_playlists.arg(uid), genDefaultParams()); }
                QJsonArray userPlaylists(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
                    return lQuery(
                        userPlaylistsUrl(uid),
                        QueryRules(tkn_response, requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }


                QUrl userFollowingsUrl(QString & uid) { return baseUrl(path_user_followings.arg(uid), genDefaultParams()); }
                QJsonArray userFollowings(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
                    return lQuery(
                        userFollowingsUrl(uid),
                        QueryRules(tkn_response, requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }


                QUrl userFollowersUrl(QString & uid) { return baseUrl(path_user_followers.arg(uid), genDefaultParams()); }
                QJsonArray userFollowers(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
                    return lQuery(
                        userFollowersUrl(uid),
                        QueryRules(tkn_response, requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }


                QUrl userGroupsUrl(QString & uid) { return baseUrl(path_user_groups.arg(uid), genDefaultParams()); }
                QJsonArray userGroups(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
                    return lQuery(
                        userGroupsUrl(uid),
                        QueryRules(tkn_response, requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                        wrap
                    );
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUEST_API
