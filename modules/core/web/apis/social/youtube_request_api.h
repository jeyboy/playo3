#ifndef YOUTUBE_REQUEST_API
#define YOUTUBE_REQUEST_API

#include "modules/core/web/interfaces/iapi.h"
#include "youtube_api_keys.h"

#define YOUTUBE_OFFSET_LIMIT 50

namespace Core {
    namespace Web {
        namespace Youtube {
            class RequestApi : public IApi {
            private:
                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_q, predicate); }

//                date – Resources are sorted in reverse chronological order based on the date they were created.
//                rating – Resources are sorted from highest to lowest rating.
//                relevance – Resources are sorted based on their relevance to the search query. This is the default value for this parameter.
//                title – Resources are sorted alphabetically by title.
//                videoCount – Channels are sorted in descending order of their number of uploaded videos.
//                viewCount – Resources are sorted from highest to lowest number of views. For live broadcasts, videos are sorted by number of concurrent viewers while the broadcasts are ongoing.
                inline void setOrder(QUrlQuery & query, const QString & order = QStringLiteral("relevance")) {
                    setParam(query, tkn_order, order);
                }

                inline void setEmbedable(QUrlQuery & query) {
                    setParam(query, tkn_video_embedable, QStringLiteral("true")); // any // true
                    setParam(query, tkn_type, QStringLiteral("video")); // channel // playlist // video
                }


                QueryRules queryRules(int count = YOUTUBE_OFFSET_LIMIT, int offset = 0, int per_request = 99999) {
                    return QueryRules(tkn_items, qMin(per_request, requestLimit()), qMin(count, YOUTUBE_OFFSET_LIMIT), offset);
                }
            public:
                inline virtual ~RequestApi() {}

                /////////////////
                /// AUTH
                ////////////////

//                inline QUrl authTokenUrl() const { return QUrl(url_auth_token); }
//                inline QString confirmAuthUrl(QString access_token) { return url_auth_confirm % access_token; }

//                QByteArray authTokenUrlParams(QString code) {
//                    QUrlQuery query = genDefaultParams();

//                    setParam(query, tkn_client_secret, val_client_tkn);
//                    setParam(query, tkn_grant_type, val_grant_type);
//                    setParam(query, tkn_redirect_uri, url_redirect);
//                    setParam(query, tkn_code, code);

//                    return query.toString(QUrl::FullyEncoded).toUtf8();
//                }

                /////////////////
                /// API
                ////////////////
                QUrl searchUrl(const QString & predicate, const QString & /*genre*/, bool hottest = false) {
                    QUrlQuery query = genDefaultParams();
                    setOrder(query, hottest ? QStringLiteral("rating") : QStringLiteral("relevance"));
                    setEmbedable(query);
                    setParam(query, tkn_part, QStringLiteral("snippet"));

                    if (!predicate.isEmpty())
                        setSearchPredicate(query, predicate);

                    return baseUrl(path_search, query);
                }

                QJsonArray popular(QString & genre) {
                    return lQuery(
                        searchUrl(QString(), genre, true),
                        queryRules(100)
                    );
                }

                QJsonArray search_postprocess(QString & predicate, QString & genre, const SearchLimit & limitations) { //count = 5
                    return lQuery(
                        searchUrl(predicate, genre, limitations.by_popularity()),
                        queryRules(limitations.total_limit)
                    );
                }


//                QUrl groupAudioUrl(QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    setAudioTypesParam(query);
//                    return baseUrl(path_group_tracks.arg(uid), query);
//                }
//                QJsonArray groupAudio(QString & group_id, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(groupAudioUrl(group_id), queryRules(count), wrap); }


//                QUrl groupPlaylistsUrl(QString & uid) { return baseUrl(path_group_playlists.arg(uid), genDefaultParams()); }
//                QJsonArray groupPlaylists(QString & group_id, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(groupPlaylistsUrl(group_id), queryRules(count), wrap); }


//                QUrl audioInfoUrl(QString & audio_uid) { return baseUrl(path_track % audio_uid, genDefaultParams()); }
//                QJsonObject audioInfo(QString audio_uid) { return sQuery(audioInfoUrl(audio_uid)); }


//                QUrl audioUrl(QStringList & audio_uids) {
//                    QUrlQuery query = genDefaultParams();
//                    setIdsFilter(query, audio_uids);
//                    return baseUrl(path_tracks, query);
//                }
//                //"id": 142370360,
//                //"permalink": "sam-smith-stay-with-me",
//                QJsonArray audioInfo(QStringList & audio_uids) { return sQuery(audioUrl(audio_uids), wrap).value(tkn_response).toArray(); }


//                QUrl userAudioUrl(QString & uid) { return baseUrl(path_user_tracks.arg(uid), genDefaultParams()); }
//                QJsonArray userAudio(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(userAudioUrl(uid), queryRules(count), wrap); }


//                QUrl userPlaylistsUrl(QString & uid) { return baseUrl(path_user_playlists.arg(uid), genDefaultParams()); }
//                QJsonArray userPlaylists(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(userPlaylistsUrl(uid), queryRules(count), wrap); }


//                QUrl userFollowingsUrl(QString & uid) { return baseUrl(path_user_followings.arg(uid), genDefaultParams()); }
//                QJsonArray userFollowings(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(userFollowingsUrl(uid), queryRules(count), wrap); }


//                QUrl userFollowersUrl(QString & uid) { return baseUrl(path_user_followers.arg(uid), genDefaultParams()); }
//                QJsonArray userFollowers(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(userFollowersUrl(uid), queryRules(count), wrap); }


//                QUrl userGroupsUrl(QString & uid) { return baseUrl(path_user_groups.arg(uid), genDefaultParams()); }
//                QJsonArray userGroups(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(userGroupsUrl(uid), queryRules(count), wrap); }


//                QUrl userByNameUrl(QString & name) {
//                    QUrlQuery query = genDefaultParams();
//                    setSearchPredicate(query, name);
//                    return baseUrl(path_users, query);
//                }

//                QJsonArray usersByName(QString & name, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(userByNameUrl(name), queryRules(count), wrap); }
//                QJsonArray userById(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//                    return lQuery(baseUrl(path_users % uid, genDefaultParams()), queryRules(count), wrap);
//                }


//                QUrl groupByNameUrl(QString & name) {
//                    QUrlQuery query = genDefaultParams();
//                    setSearchPredicate(query, name);
//                    return baseUrl(path_users, query);
//                }

//                QJsonArray groupByName(QString & name, int count = SOUNDCLOUD_OFFSET_LIMIT) { return lQuery(groupByNameUrl(name), queryRules(count), wrap); }
//                QJsonArray groupById(QString & uid, int count = SOUNDCLOUD_OFFSET_LIMIT) {
//                    return lQuery(baseUrl(path_groups % uid, genDefaultParams()), queryRules(count), wrap);
//                }

//                QUrl playlistByNameUrl(QString & name) {
//                    QUrlQuery query = genDefaultParams();
//                    setSearchPredicate(query, name);
//                    return baseUrl(path_playlists, query);
//                }
//                QJsonArray playlistByName(QString & name, int count = 10, int offset = 0) {
//                    return lQuery(playlistByNameUrl(name), queryRules(count, offset, 2), wrap); // playlists is very weighted for loading - so set limitation to 2 playlists per request
//                }
            };
        }
    }
}

#endif // YOUTUBE_REQUEST_API
