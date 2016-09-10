#ifndef VK_REQUESTS
#define VK_REQUESTS

#include "vk_auth.h"
#include "vk_feed.h"
#include "vk_group.h"
#include "vk_playlist.h"
#include "vk_set.h"
#include "vk_track.h"
#include "vk_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Requests : public Sociable, public Auth, public Feed, public Group,
                    public Playlist, public Set, public Track, public User {
            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_primary_source |
                        sf_auth_api_has | sf_auth_site_has |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable | sf_groups_serachable |
                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_packable |
                        sf_recomendable_by_item | sf_recomendable_by_user |
                        /*sf_newable |*/ sf_populable |

                        sf_content_lyrics_has | sf_content_audio_has | sf_content_video_has |
                        sf_content_photo_has | sf_content_news_has |

                        sf_auth_mandatory | sf_prefer_site_object_content | sf_prefer_site_recomendations | sf_prefer_site_packs
                    );
                }

                inline QString boolToStr(const bool & val) { return val ? val_str_true : val_str_false; }

                bool connectUserApi() {
                    QString token, user_id, expiration;

                    if (connectApi(token, user_id, expiration, error)) {
                        setApiToken(token);
                        setApiUserID(user_id);
                        setApiExpiration(expiration);
                        return true;
                    }

                    return false;
                }
                bool connectUserSite() {
                    setSiteUserID(apiUserID()); // use api user id
                    return true; /*return connectSite(error);*/
                }

                QJsonValue popular(const SearchLimit & limits) {
                    QJsonArray res;

                    if (limits.include_audio())
                        res << setByType(set_popular_tracks, limits);

                    if (limits.include_video())
                        res << setByType(set_popular_video, limits);

                    return res;
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() && limits.by_popularity())
                        return popular(limits);
                    else {
                        QJsonArray res;

                        if (limits.include_audio())
                            res << tracksSearch(limits);

                        if (limits.include_video())
                            res << videoSearch(limits);

                        return res;
                    }
                }

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_api: return url_api_base % predicate;
                        case qst_site: return url_site_base % predicate;
                        default: return QString();
                    }
                }

                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api) {
                    QUrlQuery query = QUrlQuery();

                    if (stype == qst_api) {
                        query.addQueryItem(tkn_version, val_version);
                        query.addQueryItem(tkn_access_token, apiToken());
                    }

                    return query;
                }

                void saveAdditionals(QJsonObject & obj) {
                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_api_base));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                    Manager::removeCookies(url_api_base);
                }

                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();
                        QString title = JSON_STR(obj, tkn_title);
                        if (title.isEmpty())
                            title = JSON_STR_CAT(obj, LSTR("first_name"), ' ', LSTR("last_name"));

                        linkables << Linkable(
                            JSON_CSTR(obj, tkn_id),
                            title,
                            JSON_STR(obj, tkn_screen_name),
                            JSON_STR(obj, tkn_photo)
                        );
                    }
                }
                inline void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        QString title = obj.value(tkn_title).toString();
                        if (title.isEmpty())
                            title = obj.value(QStringLiteral("name")).toString();

                        linkables << Linkable(
                            JSON_CSTR(obj, tkn_id),
                            title,
                            JSON_STR(obj, tkn_screen_name),
                            JSON_STR(obj, tkn_photo)
                        );
                    }
                }

                inline bool endReached(QJsonObject & response, QueriableArg * /*arg*/) { return response.value(tkn_finished).toBool(); }
                bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message) {
                    QJsonObject stat_obj = json.value(tkn_error).toObject();
                    message = stat_obj.value(tkn_error_msg).toString();
                    code = stat_obj.value(tkn_error_code).toInt();

                    if (code == 14) {
                        if (Settings::obj().isIgnoreCaptcha())
                            return false;
                        else
                            return captchaProcessing(json, arg -> request_url);
                    } else return code == 0;
                }

                QJsonValue loadSetData(const QUrlQuery & query) {
                    QString id = query.queryItemValue(CMD_ID);

                    switch(query.queryItemValue(CMD_MEDIA_TYPE).toInt()) {
                        case dmt_audio: return tracksByPlaylist(id);
                        case dmt_video: return videoByPlaylist(id);
                        default: qDebug() << "VK LOAD SET UNKNOWN TYPE";
                    }

                    return QJsonArray();
                }

                QJsonValue loadSetData(const QString & attrs) { return loadSetData(QUrlQuery(attrs)); }

                void procSociables(QJsonObject & json) {
                    clearFriends();
                    jsonToUsers(Friendable::linkables, json.value(block_friends).toArray());
                    clearGroups();
                    jsonToGroups(Groupable::linkables, json.value(block_groups).toArray());
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByIdOrPerma(uid).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByIdOrPerma(uid).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
            public:
                Requests() { setSociableLimitations(true, true, true, true); }

                static QString cleanUrl(const QString & refreshed_url) { return refreshed_url.section('?', 0, 0); }

                QString refresh(const QString & item_uid, const DataMediaType & itemMediaType) {
                    switch(itemMediaType) {
                        case dmt_audio: return cleanUrl(trackUrl(item_uid));
                        case dmt_video: return videoUrl(item_uid);
                        default:;
                    }

                    return QString();
                }

//                QJsonValue userTracksGroupsFriends(const QString & user_id) {
//                    return User::sRequest(
//                        User::baseUrlStr(
//                            qst_api, tkn_execute,
//                            {{ tkn_code, query_user_tracks_groups_friends.arg(user_id) }}
//                        ),
//                        call_type_json, 0, proc_json_extract
//                    );
//                }

                QJsonValue userInfo(const QString & user_id) {
                    Permissions perm = permissions(pr_object_content);

                    if (perm > perm_none) {
                        QJsonObject ret = User::sRequest(
                            User::baseUrlStr(
                                qst_api, tkn_execute,
                                {{ tkn_code, query_user_groups_friends(user_id) }}
                            ),
                            call_type_json, 0, proc_json_extract
                        );
                        procSociables(ret);

                        return userMedia(user_id);
                    }

                    return QJsonArray();
                }

                QJsonValue userMedia(const QString & user_id) {
                    Permissions perm = permissions(pr_object_content);

                    switch(perm) {
                        case perm_site: {
                            Response * response = Manager::prepare() -> postFollowed(
                                IQueriable::baseUrlStr(
                                    qst_site, QStringLiteral("al_audio.php"),
                                    {
                                        { QStringLiteral("act"), QStringLiteral("load_silent") },
                                        { QStringLiteral("al"), QStringLiteral("1") },
                                        { QStringLiteral("album_id"), QStringLiteral("-2") },
                                        { QStringLiteral("band"), QStringLiteral("true") },
                                        { QStringLiteral("owner_id"), user_id }
                                    }
                                ),
                                Auth::dntHeader()
                            );

                            QJsonArray tracks_res, items = RESPONSE_TO_JSON_OBJECT(response).value(QStringLiteral("list")).toArray();
                            QHash<QString, QJsonArray> albums;

                            Track::prepareTracks(items, tracks_res, &albums);
                            /////////////////////////////////////

                            QJsonArray playlists_res;

                            for(QHash<QString, QJsonArray>::Iterator album = albums.begin(); album != albums.end(); album++) {
                                playlists_res << QJsonObject {
                                    {tkn_items, album.value()},
                                    {tkn_id, album.key()},
                                    {tkn_title, QString::number(playlists_res.size())} // for real names need send request to get user playlists
                                };
                            }

                            /////////////////////////////////////
                            QJsonArray res = videoByUser(user_id).toArray();

                            res.prepend(prepareBlock(dmt_audio, tracks_res, {{tkn_dir_name, QStringLiteral("Tracks")}}));
                            res.prepend(prepareBlock(dmt_audio_set, playlists_res, {{tkn_dir_name, QStringLiteral("Tracks")}}));

                            return res;
                        break;}

                        case perm_api: {
                            QJsonObject ret = User::sRequest(
                                User::baseUrlStr(
                                    qst_api, tkn_execute,
                                    {{ tkn_code, query_user_tracks_playlists(user_id) }}
                                ),
                                call_type_json, 0, proc_json_extract
                            );

                            QJsonArray res = videoByUser(user_id).toArray();

                            if (!ret.value(tkn_albums_finished).toBool()) {
                                QJsonArray ar = ret.value(block_sets_audio).toArray();
                                tracksPlaylistsByUser(user_id, &ar, ret.value(tkn_albums_offset).toInt());
                                ret.insert(block_sets_audio, ar);
                            }

                            res.prepend(prepareBlock(dmt_audio, ret.value(block_items_audio), {{tkn_dir_name, QStringLiteral("Tracks")}}));
                            res.prepend(prepareBlock(dmt_audio_set, ret.value(block_sets_audio), {{tkn_dir_name, QStringLiteral("Tracks")}}));

                            return ret;
                        break;}

                        default:;
                    }

                    return QJsonArray();
                }

                QJsonValue userRecommendations(const QString & user_id, bool randomize) {
                    return QJsonArray() << Track::userRecommendations(user_id, randomize);
                }

                QJsonValue trackRecommendations(const QString & track_id, bool randomize) {
                    return QJsonArray() << Track::trackRecommendations(track_id, randomize);
                }
            };
        }
    }
}

#endif // VK_REQUESTS
