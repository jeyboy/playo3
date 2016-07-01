#ifndef SOUNDCLOUD_REQUEST_SITE
#define SOUNDCLOUD_REQUEST_SITE

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class RequestSite : public virtual IQueriable {
                Headers headers() {
                    return {
                        {QStringLiteral("DNT"), QStringLiteral("1")},
                        {QStringLiteral("Origin"), QStringLiteral("https://soundcloud.com")},
                        {QStringLiteral("Referer"), QStringLiteral("https://soundcloud.com")}
                    };
                }
            protected:
                enum ChartType {
                    new_hot = 1,
                    top_50
                };

                QString idToPathSite(const QString & track_id) {
                    QUrl url(QStringLiteral("https://api.soundcloud.com/i1/tracks/%1/streams").arg(track_id));
                    url.setQuery(genDefaultParams(qst_html));

                    QJsonObject obj = sRequest(
                        url.toString(), call_type_json, 0, proc_json_patch,
                        QStringList() << DEF_JSON_FIELD, call_method_get,
                        headers()
                    );
                    QString res = obj.value(QStringLiteral("http_mp3_128_url")).toString();
                    return res;
                }

                QJsonArray chartSite(const ChartType & chartType, const QString & genre = QStringLiteral("all-music")) {
                    QUrl url(QStringLiteral("https://api-v2.soundcloud.com/charts"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    query.addQueryItem(QStringLiteral("genre"), QStringLiteral("soundcloud:genres:") % genre);
                    query.addQueryItem(QStringLiteral("kind"), chartType == new_hot ? QStringLiteral("trending") : QStringLiteral("top"));
                    // linked_partitioning=1
                    url.setQuery(query);

                    QJsonObject obj = sRequest(
                        url.toString(), call_type_json, 0, proc_json_patch,
                        QStringList() << DEF_JSON_FIELD, call_method_get,
                        headers()
                    );

                    QJsonArray res; // "last_updated":"2016-07-01T05:27:38Z"

                    QJsonArray collection = obj.value(QStringLiteral("collection")).toArray();
                    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++)
                        res.append((*it).toObject().value(QStringLiteral("track")));

                    return res;
                }

//                PolyQueryRules rules(
//                    int offset = 0, int items_limit = SOUNDCLOUD_ITEMS_LIMIT, int pages_limit = SOUNDCLOUD_PAGES_LIMIT,
//                    int per_request = SOUNDCLOUD_PER_REQUEST_LIMIT,
//                    ApiCallIterType call_type = call_iter_type_item)
//                {
//                    return PolyQueryRules(
//                        call_type,
//                        call_iter_method_offset,
//                        qMin(items_limit, SOUNDCLOUD_ITEMS_LIMIT),
//                        qMin(pages_limit, SOUNDCLOUD_PAGES_LIMIT),
//                        tkn_limit,
//                        qMin(qMin(per_request, items_limit), SOUNDCLOUD_PER_REQUEST_LIMIT),
//                        tkn_offset,
//                        offset
//                    );
//                }
//            public:
//                inline virtual ~RequestApi() {}

//                QString authUrl() {
//                    QUrl url(url_connect);

//                    QUrlQuery query = genDefaultParams();
//                    setParam(query, tkn_response_type, val_response_type);
//                    setParam(query, tkn_scope, val_scope);
//                    setParam(query, tkn_redirect_uri, url_redirect);
//                    setParam(query, tkn_display, val_display);

//                    url.setQuery(query);
//                    return url.toString();
//                }

//                /////////////////
//                /// AUTH
//                ////////////////

//                //QString authTokenUrl() const {
//                //    QUrl url("https://api.soundcloud.com/oauth2/token");
//                //    QUrlQuery query = genDefaultParams();


//                //    query.addQueryItem("client_secret", val_client_tkn);
//                //    query.addQueryItem("scope", "non-expiring");

//                ////    query.addQueryItem("grant_type", "password");
//                ////    query.addQueryItem("username", "USERNAME");
//                ////    query.addQueryItem("password", "PASSWORD");

//                //    url.setQuery(query);
//                //    return url.toString();
//                //}
//                inline QUrl authTokenUrl() const { return QUrl(url_auth_token); }
//                inline QString confirmAuthUrl(const QString & access_token) { return url_auth_confirm % access_token; }

//                QByteArray authTokenUrlParams(QString code) {
//                    QUrlQuery query = genDefaultParams();

//                    setParam(query, tkn_client_secret, val_client_tkn);
//                    setParam(query, tkn_grant_type, val_grant_type);
//                    setParam(query, tkn_redirect_uri, url_redirect);
//                    setParam(query, tkn_code, code);

//                    return query.toString(QUrl::FullyEncoded).toUtf8();
//                }

//                /////////////////
//                ///
//                /// API
//                ////////////////

////                GET /tracks/{id}/related


//                // TODO: realize me!!
//                // https://soundcloud.com/charts/top // charts by genres
//                // https://soundcloud.com/discover // recommendations

//                QString audioSearchUrl(const QString & predicate, const QString & genre, bool hottest = false) {
//                    QUrlQuery query = genDefaultParams();
//                    setAudioTypesParam(query);
//                    setOrder(query, hottest);

//                    if (!genre.isEmpty())
//                        setGenreLimitation(query, genre);

//                    if (!predicate.isEmpty())
//                        setSearchPredicate(query, predicate);

//                    return baseUrlStr(qst_json, path_tracks, query);
//                }

//                QJsonValue searchInSets(const SearchLimit & limits) {
//                    QString predicate = predicate.isEmpty() ? limits.genre : limits.predicate;
//                    return playlistByPredicate(predicate, limits.items_limit, limits.start_offset);
//                }

//                QJsonValue popular(const SearchLimit & limitations) {
//                    return pRequest(
//                        audioSearchUrl(QString(), limitations.genre, true),
//                        call_type_json,
//                        rules(),
//                        0,
//                        proc_patch
//                    );
////                    return lQuery(
////                        audioSearchUrl(QString(), genre, true),
////                        queryRules(100),
////                        wrap
////                    );
//                }

//                void getGroupInfo(const QString & uid, QJsonObject & object) {
//                    object.insert(tkn_audio_list, groupAudio(uid));
//                }

//                void getUserInfo(const QString & uid, QJsonObject & object) {
//                    object.insert(tkn_audio_list, userAudio(uid));
//                    object.insert(tkn_playlist, userPlaylists(uid));
//                    QThread::msleep(REQUEST_DELAY);
//                    object.insert(tkn_followings, userFollowings(uid)); // return bad request error
//                    object.insert(tkn_followers, userFollowers(uid));
//                    QThread::msleep(REQUEST_DELAY);
//                    object.insert(tkn_groups, userGroups(uid));
//                }
//                QJsonObject objectInfo(const QString & uid) {
//                    QJsonObject res;

//                    if (uid[0] == '-')
//                        getGroupInfo(uid.mid(1), res);
//                    else
//                        getUserInfo(uid, res);

//                    return res;
//                }


//                QString groupAudioUrl(const QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    setAudioTypesParam(query);
//                    return baseUrlStr(qst_json, path_group_tracks.arg(uid), query);
//                }
//                QJsonArray groupAudio(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        groupAudioUrl(group_id),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(groupAudioUrl(group_id), queryRules(count), wrap);
//                }

//                QString audioInfoUrl(const QString & audio_uid) { return baseUrlStr(qst_json, path_track % audio_uid, genDefaultParams()); }
//                QJsonObject audioInfo(const QString & audio_uid) {
//                    return sRequest(
//                        audioInfoUrl(audio_uid),
//                        call_type_json
//                    );

////                    return sQuery(audioInfoUrl(audio_uid));
//                }


//                QString audioUrl(const QStringList & audio_uids) {
//                    QUrlQuery query = genDefaultParams();
//                    setIdsFilter(query, audio_uids);
//                    return baseUrlStr(qst_json, path_tracks, query);
//                }
//                //"id": 142370360,
//                //"permalink": "sam-smith-stay-with-me",
//                QJsonArray audioInfo(const QStringList & audio_uids) {
//                    return sRequest(
//                        audioUrl(audio_uids),
//                        call_type_json,
//                        0,
//                        proc_patch
//                    ).value(tkn_response).toArray();

////                    return sQuery(audioUrl(audio_uids), wrap).value(tkn_response).toArray();
//                }


//                QString userAudioUrl(const QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_user_tracks.arg(uid), query);
//                }
//                QJsonArray userAudio(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        userAudioUrl(uid),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(userAudioUrl(uid), queryRules(count), wrap);
//                }

//                QString userAudioFavoritesUrl(const QString & user_uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_user_favorites.arg(user_uid), query);
//                }
//                QJsonArray userAudioFavorites(QString & user_uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        userAudioFavoritesUrl(user_uid),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(userAudioUrl(uid), queryRules(count), wrap);
//                }

//                QString userAppendAudioFavoritesUrl(const QString & user_uid, const QString & track_uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_user_favorites.arg(user_uid) % '/' % track_uid, query);
//                }
//                void userAppendAudioFavorites(const QString & user_uid, const QString & track_uid) {
//                    QString res = Manager::prepare() -> putFollowed(
//                        userAppendAudioFavoritesUrl(user_uid, track_uid),
//                        Headers()
//                    ) -> toText();
//                    qDebug() << "FAV" << res;
//                }


//                QString userPlaylistsUrl(const QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_user_playlists.arg(uid), query);
//                }
//                QJsonArray userPlaylists(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        userPlaylistsUrl(uid),
//                        call_type_json,
//                        rules(0, count, SOUNDCLOUD_PAGES_LIMIT, 25),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(userPlaylistsUrl(uid), queryRules(count), wrap);
//                }


//                QString userFollowingsUrl(const QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_user_followings.arg(uid), query);
//                }
//                QJsonArray userFollowings(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        userFollowingsUrl(uid),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(userFollowingsUrl(uid), queryRules(count), wrap);
//                }


//                QString userFollowersUrl(const QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_user_followers.arg(uid), query);
//                }
//                QJsonArray userFollowers(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        userFollowersUrl(uid),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(userFollowersUrl(uid), queryRules(count), wrap);
//                }


//                QString userGroupsUrl(const QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_user_groups.arg(uid), query);
//                }
//                QJsonArray userGroups(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        userGroupsUrl(uid),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(userGroupsUrl(uid), queryRules(count), wrap);
//                }


//                QString usersByNameUrl(const QString & name) {
//                    QUrlQuery query = genDefaultParams();
//                    setSearchPredicate(query, name);
//                    return baseUrlStr(qst_json, path_users, query);
//                }

//                QJsonArray usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        usersByNameUrl(name),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(userByNameUrl(name), queryRules(count), wrap);
//                }
//                QJsonArray userById(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        baseUrlStr(qst_json, path_users % '/' % uid, genDefaultParams()),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );
////                    return lQuery(baseUrl(path_users % uid, genDefaultParams()), queryRules(count), wrap);
//                }


//                QString groupsByNameUrl(const QString & name) {
//                    QUrlQuery query = genDefaultParams();
//                    setSearchPredicate(query, name);
//                    return baseUrlStr(qst_json, path_groups, query);
//                }

//                QJsonArray groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        groupsByNameUrl(name),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(groupByNameUrl(name), queryRules(count), wrap);
//                }

//                QString groupByIdUrl(const QString & uid) {
//                    QUrlQuery query = genDefaultParams();
//                    return baseUrlStr(qst_json, path_groups % '/' % uid, query);
//                }
//                QJsonArray groupById(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
//                    return pRequest(
//                        groupByIdUrl(uid),
//                        call_type_json,
//                        rules(0, count),
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(baseUrl(path_groups % uid, genDefaultParams()), queryRules(count), wrap);
//                }

//                QString playlistByPredicateUrl(QString & name) {
//                    QUrlQuery query = genDefaultParams();
//                    setSearchPredicate(query, name);
//                    return baseUrlStr(qst_json, path_playlists, query);
//                }

//                // predicate is used for search in title - genre - tags - permalinks
//                QJsonArray playlistByPredicate(QString & predicate, int count = 10, int offset = 0) {
//                    return pRequest(
//                        playlistByPredicateUrl(predicate),
//                        call_type_json,
//                        rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET), // playlists is very weighted for loading - so set limitation to 2 playlists per request
//                        0,
//                        proc_patch
//                    );

////                    return lQuery(playlistByNameUrl(name), queryRules(count, offset, 2), wrap);
//                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUEST_SITE
