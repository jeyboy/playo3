#ifndef YOUTUBE_REQUEST_API
#define YOUTUBE_REQUEST_API

#include "youtube_api_misc.h"
//#include "modules/core/interfaces/ishareable.h"

#define YOUTUBE_INFO_ITEMS_LIMIT 50
#define YOUTUBE_ITEMS_LIMIT 100

namespace Core {
    namespace Web {
        namespace Youtube {
            class RequestApi : public Misc {
//                Your request can also use the Boolean NOT (-) and OR (|) operators to exclude videos or to find videos that are associated with one of several
//                search terms. For example, to search for videos matching either "boating" or "sailing", set the q parameter value to boating|sailing.
//                Similarly, to search for videos matching either "boating" or "sailing" but not "fishing", set the q parameter value to boating|sailing -fishing.
//                Note that the pipe character must be URL-escaped when it is sent in your API request. The URL-escaped value for the pipe character is %7C.
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

//            {
//             "kind": "youtube#videoCategory",
//             "etag": "\"DsOZ7qVJA4mxdTxZeNzis6uE6ck/nqRIq97-xe5XRZTxbknKFVe5Lmg\"",
//             "id": "10",
//             "snippet": {
//              "channelId": "UCBR8-60-B28hp2BmDPdntcQ",
//              "title": "Music",
//              "assignable": true
//             }
//            },
                void setMusicVideoCategory(QUrlQuery & query) {
                    setParam(query, QStringLiteral("videoCategoryId"), 10);
                }

                inline void setEmbedable(QUrlQuery & query) {
                    setParam(query, tkn_video_embedable, QStringLiteral("true")); // any // true
                    setParam(query, tkn_type, QStringLiteral("video")); // channel // playlist // video
                }


//                QueryRules queryRules(int count = YOUTUBE_OFFSET_LIMIT, int offset = 0, int per_request = 99999) {
//                    return QueryRules(tkn_items, qMin(per_request, requestLimit()), qMin(count, YOUTUBE_OFFSET_LIMIT), offset);
//                }

                PolyQueryRules rules(int offset = 0, int items_limit = YOUTUBE_ITEMS_LIMIT, int pages_count = DEFAULT_REQUESTS_LIMIT, ApiCallIterType call_type = call_iter_type_item) {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_token,
                        qMin(items_limit, YOUTUBE_ITEMS_LIMIT),
                        qMin(pages_count, DEFAULT_REQUESTS_LIMIT),
                        offset
                    );
                }

                void proceedDurationResult(QStringList & ids, QJsonArray & arr) {
                    pRequest(
                        videosUrl(ids),
                        call_type_json,
                        rules(0, YOUTUBE_INFO_ITEMS_LIMIT),
                        proc_none,
                        &arr
                    );

//                    lQuery(
//                        videosUrl(ids),
//                        queryRules(50),
//                        res
//                    );
                    ids.clear();
                }

                void initDuration(QJsonArray & arr) {
                    QStringList ids;
                    QJsonArray res;

                    for(QJsonArray::Iterator item = arr.begin(); item != arr.end(); item++)
                        if (ids.length() == YOUTUBE_INFO_ITEMS_LIMIT)
                            proceedDurationResult(ids, res);
                        else
                            ids << (*item).toObject().value(QStringLiteral("id")).toObject().value(QStringLiteral("videoId")).toString();

                    if (!ids.isEmpty())
                        proceedDurationResult(ids, res);

                    arr = res;
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
//                videoDuration 	string
//                The videoDuration parameter filters video search results based on their duration. If you specify a value for this parameter, you must also set the type parameter's value to video.

//                Acceptable values are:

//                    any – Do not filter video search results based on their duration. This is the default value.
//                    long – Only include videos longer than 20 minutes.
//                    medium – Only include videos that are between four and 20 minutes long (inclusive).
//                    short – Only include videos that are less than four minutes long.

                //encodeStr
                QString searchUrl(const QString & predicate, const QString & /*genre*/, bool hottest = false, const QString & relatedVideoId = QString()) {
                    QUrlQuery query = genDefaultParams();
                    setOrder(query, hottest ? QStringLiteral("rating") : QStringLiteral("relevance"));
                    setEmbedable(query);
                    setParam(query, tkn_part, QStringLiteral("snippet"));
                    setParam(query, QStringLiteral("fields"), QStringLiteral("items(id,snippet),nextPageToken,pageInfo"));
                    setParam(query, QStringLiteral("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT); // 50
//                    setParam(query, QStringLiteral("safeSearch"), QStringLiteral("none"));
                    setMusicVideoCategory(query);

                    if (!predicate.isEmpty())
                        setSearchPredicate(query, predicate);
                    else if (!relatedVideoId.isEmpty())
                        setParam(query, QStringLiteral("relatedToVideoId"), relatedVideoId);

                    return baseUrlStr(path_search, query);
                }

                QString videosUrl(const QStringList & ids = QStringList()) {
                    QUrlQuery query = genDefaultParams();
                    if (!ids.isEmpty())
                        setParam(query, QStringLiteral("id"), ids.join(','));
                    else
                        setParam(query, QStringLiteral("chart"), QStringLiteral("mostPopular"));

                    setParam(query, tkn_part, QStringLiteral("snippet,contentDetails"));
                    setParam(query, QStringLiteral("fields"), QStringLiteral("items(contentDetails,fileDetails,id,localizations,player,snippet),nextPageToken,pageInfo"));
                    setParam(query, QStringLiteral("maxResults"), YOUTUBE_INFO_ITEMS_LIMIT); // 50
                    setParam(query, QStringLiteral("regionCode"), QStringLiteral("ua"));
                    setMusicVideoCategory(query);

                    return baseUrlStr(QStringLiteral("videos"), query);
                }

                QJsonArray popular(const SearchLimit & /*limits*/) {
                    return pRequest(
                        videosUrl(),
                        call_type_json,
                        rules()
                    );

//                    return lQuery(
//                        videosUrl(),
//                        queryRules(100)
//                    );
                }

                QJsonArray search_proc(const SearchLimit & limits) { //count = 5
//                    QJsonArray res = lQuery(
//                        searchUrl(limits.predicate, limits.genre, limits.by_popularity()),
//                        queryRules(limits.items_limit)
//                    );


                    QJsonArray res = pRequest(
                        searchUrl(limits.predicate, limits.genre, limits.by_popularity()),
                        call_type_json,
                        rules(0, limits.items_limit)
                    );

                    initDuration(res);
                    return res;
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
//                QJsonObject audioInfo(const QString & audio_uid) { return sQuery(audioInfoUrl(audio_uid)); }


//                QUrl audioUrl(QStringList & audio_uids) {
//                    QUrlQuery query = genDefaultParams();
//                    setIdsFilter(query, audio_uids);
//                    return baseUrl(path_tracks, query);
//                }
//                //"id": 142370360,
//                //"permalink": "sam-smith-stay-with-me",
//                QJsonArray audioInfo(const QStringList & audio_uids) { return sQuery(audioUrl(audio_uids), wrap).value(tkn_response).toArray(); }


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
