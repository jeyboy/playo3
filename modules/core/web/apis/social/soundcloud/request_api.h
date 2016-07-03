#ifndef SOUNDCLOUD_REQUEST_API
#define SOUNDCLOUD_REQUEST_API

#include "defines.h"

#include "auth.h"
#include "group.h"
#include "playlist.h"
#include "set.h"
#include "track.h"
#include "user.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class RequestApi : public ApiAuth, public ApiGroup, public ApiPlaylist,
                    public ApiSet, public ApiTrack, public ApiUser {
            public:
                inline virtual ~RequestApi() {}


                QJsonValue searchInSets(const SearchLimit & limits) {
                    QString predicate = predicate.isEmpty() ? limits.genre : limits.predicate;
                    return playlistByPredicate(predicate, limits.items_limit, limits.start_offset);
                }


                QJsonValue popular(const SearchLimit & limitations) {
                    return pRequest(
                        audioSearchUrl(QString(), limitations.genre, true),
                        call_type_json,
                        rules(),
                        0,
                        proc_json_patch
                    );
//                    return lQuery(
//                        audioSearchUrl(QString(), genre, true),
//                        queryRules(100),
//                        wrap
//                    );
                }

                void getGroupInfo(const QString & uid, QJsonObject & object) {
                    object.insert(tkn_audio_list, groupAudio(uid));
                }

                void getUserInfo(const QString & uid, QJsonObject & object) {
                    object.insert(tkn_audio_list, userAudio(uid));
                    object.insert(tkn_playlist, userPlaylists(uid));
                    QThread::msleep(REQUEST_DELAY);
                    object.insert(tkn_followings, userFollowings(uid)); // return bad request error
                    object.insert(tkn_followers, userFollowers(uid));
                    QThread::msleep(REQUEST_DELAY);
                    object.insert(tkn_groups, userGroups(uid));
                }
                QJsonObject objectInfo(const QString & uid) {
                    QJsonObject res;

                    if (uid[0] == '-')
                        getGroupInfo(uid.mid(1), res);
                    else
                        getUserInfo(uid, res);

                    return res;
                }



                QString userFollowingsUrl(const QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    return baseUrlStr(qst_json, path_user_followings.arg(uid), query);
                }
                QJsonArray userFollowings(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userFollowingsUrl(uid),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_patch
                    );

//                    return lQuery(userFollowingsUrl(uid), queryRules(count), wrap);
                }


                QString userFollowersUrl(const QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    return baseUrlStr(qst_json, path_user_followers.arg(uid), query);
                }
                QJsonArray userFollowers(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userFollowersUrl(uid),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_patch
                    );

//                    return lQuery(userFollowersUrl(uid), queryRules(count), wrap);
                }


                QString userGroupsUrl(const QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    return baseUrlStr(qst_json, path_user_groups.arg(uid), query);
                }
                QJsonArray userGroups(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        userGroupsUrl(uid),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_patch
                    );

//                    return lQuery(userGroupsUrl(uid), queryRules(count), wrap);
                }


                QString usersByNameUrl(const QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setSearchPredicate(query, name);
                    return baseUrlStr(qst_json, path_users, query);
                }

                QJsonArray usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        usersByNameUrl(name),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_patch
                    );

//                    return lQuery(userByNameUrl(name), queryRules(count), wrap);
                }
                QJsonArray userById(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        baseUrlStr(qst_json, path_users % '/' % uid, genDefaultParams()),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_patch
                    );
//                    return lQuery(baseUrl(path_users % uid, genDefaultParams()), queryRules(count), wrap);
                }


                QString groupsByNameUrl(const QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setSearchPredicate(query, name);
                    return baseUrlStr(qst_json, path_groups, query);
                }

                QJsonArray groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        groupsByNameUrl(name),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_patch
                    );

//                    return lQuery(groupByNameUrl(name), queryRules(count), wrap);
                }

                QString groupByIdUrl(const QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    return baseUrlStr(qst_json, path_groups % '/' % uid, query);
                }
                QJsonArray groupById(const QString & uid, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    return pRequest(
                        groupByIdUrl(uid),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_patch
                    );

//                    return lQuery(baseUrl(path_groups % uid, genDefaultParams()), queryRules(count), wrap);
                }

                QString playlistByPredicateUrl(const QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setSearchPredicate(query, name);
                    return baseUrlStr(qst_json, path_playlists, query);
                }

                // predicate is used for search in title - genre - tags - permalinks
                QJsonArray playlistByPredicate(const QString & predicate, int count = 10, int offset = 0) {
                    return pRequest(
                        playlistByPredicateUrl(predicate),
                        call_type_json,
                        rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET), // playlists is very weighted for loading - so set limitation to 2 playlists per request
                        0,
                        proc_json_patch
                    );

//                    return lQuery(playlistByNameUrl(name), queryRules(count, offset, 2), wrap);
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUEST_API
