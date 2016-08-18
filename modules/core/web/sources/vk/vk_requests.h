#ifndef VK_REQUESTS
#define VK_REQUESTS

#include "vk_auth.h"
#include "vk_feed.h"
#include "vk_group.h"
#include "vk_playlist.h"
#include "vk_set.h"
#include "vk_track.h"
#include "vk_video.h"
#include "vk_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Requests : public Sociable, public Auth, public Feed, public Group,
                    public Playlist, public Set, public Track, public Video, public User {
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

                        sf_auth_mandatory | sf_prefer_site_object_content
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
                        QString title = obj.value(tkn_title).toString();
                        if (title.isEmpty())
                            title = QString(obj.value(QStringLiteral("first_name")).toString() % ' ' % obj.value(QStringLiteral("last_name")).toString());

                        linkables << Linkable(
                            idToStr(obj.value(tkn_id)),
                            title,
                            obj.value(tkn_screen_name).toString(),
                            obj.value(tkn_photo).toString()
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
                            idToStr(obj.value(tkn_id)),
                            title,
                            obj.value(tkn_screen_name).toString(),
                            obj.value(tkn_photo).toString()
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
            public:
                Requests() { setSociableLimitations(true, true, true, true); }

                static QString cleanUrl(const QString & refreshed_url) { return refreshed_url.section('?', 0, 0); }

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
                                Auth::dntHeader().unite({{ QStringLiteral("Referer"), QStringLiteral("http://vk.com/audios") % user_id }})
                            );

                            QString data = response -> toText();

                            QStringList parts = data.split(QStringLiteral("<!json>"));

                            if (parts.length() == 1) {
                                Logger::obj().write("VK", "LOAD AUDIO", true);
                                return QJsonObject();
                            } else {
                                QJsonObject info_obj = QJsonDocument::fromJson(parts.last().toUtf8()).object();

                                if (!info_obj.value(QStringLiteral("isComplete")).toBool())
                                    qDebug() << "VK HAS MORE RECORDS";

                                QHash<QString, QJsonArray> albums;
                                QJsonArray tracks_res, items = info_obj.value(QStringLiteral("list")).toArray();

                                for(QJsonArray::Iterator item = items.begin(); item != items.end(); item++) {
                                    QJsonArray track = (*item).toArray();
                                    QJsonObject track_obj;

                                    track_obj.insert(tkn_owner_id, track[1].toString().toInt());
                                    track_obj.insert(tkn_id, track[0].toString().toInt());

                                    QString url = track[2].toString();
                                    if (!url.isEmpty())
                                        track_obj.insert(tkn_url, url);

////                                    track[9].toString().toInt() // ?lyrics_id // '0' if empty

                                    track_obj.insert(tkn_artist, UnicodeDecoding::decodeHtmlEntites(track[4].toString()));
                                    track_obj.insert(tkn_title, UnicodeDecoding::decodeHtmlEntites(track[3].toString()));
                                    track_obj.insert(tkn_duration, track[5].toInt());
////                                    track_obj.insert(tkn_genre_id, ); // not presented

                                    QString album_id = ISource::idToStr(track[6]);
                                    if (album_id.toInt() > 0)// album_id // '0' if empty
                                        albums[album_id] << track_obj;

                                    tracks_res << track_obj;

                                    // 0 - id
                                    // 1 - owner_id
                                    // 2 - url
                                    // 3 - title
                                    // 4 - artist
                                    // 5 - duration
                                    // 6 - album_id
                                    // 7 -
                                    // 8 -
                                    // 9 - lyrics
                                    // 10 -
                                    // 11 -
                                    // 12 -
                                    // 13 - some token
                                }

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

                                res.prepend(prepareBlock(dmt_audio, tracks_res));
                                res.prepend(prepareBlock(dmt_audio_set, playlists_res));

                                return res;
                            }
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

                            res.prepend(prepareBlock(dmt_audio, ret.value(block_items_audio)));
                            res.prepend(prepareBlock(dmt_audio_set, ret.value(block_sets_audio)));

                            return ret;
                        break;}

                        default:;
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // VK_REQUESTS
