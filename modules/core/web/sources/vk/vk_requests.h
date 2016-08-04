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
#include "modules/core/interfaces/isource.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Requests : public ISource, public Sociable, public Auth, public Feed, public Group,
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

                Permissions permissions(const PermitRequest & req_perm = pr_search_media) { return ISource::permissions(req_perm); }

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
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(block_items_audio, setByType(set_popular_tracks, PPACK(Auth::boolToStr(false), limits.genre)));

//                    if (limits.include_video())
//                        res.insert(block_items_video, videoPopular(limits));

                    return res;
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() && limits.by_popularity())
                        return popular(limits);
                    else {
                        QJsonObject res;

                        if (limits.include_audio())
                            res.insert(block_items_audio, tracksSearch(limits));

                        if (limits.include_video())
                            res.insert(block_items_video, videoSearch(limits));

                        return res;
                    }
                }

                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api_def) {
                    QUrlQuery query = QUrlQuery();

                    if (stype == qst_api_def) {
                        query.addQueryItem(tkn_version, val_version);
                        query.addQueryItem(tkn_access_token, apiToken());
                    }

                    return query;
                }

                void saveAdditionals(QJsonObject & obj) {
                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_base));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                    Manager::removeCookies(url_base);
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

                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) {
                    return url_base % predicate;
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

                inline QJsonValue loadPlaylist(const QVariantMap & attrs) {
                    switch(attrs[tkn_media_type].toInt()) {
                        case dmt_audio: return tracksByPlaylist(attrs[tkn_grab_refresh].toString());
                        case dmt_video: return videoByPlaylist(attrs[tkn_grab_refresh].toString());
                        default: qDebug() << "VK LOAD SET UNKNOWN TYPE";
                    }

                    return QJsonArray();
                }

                void procSociables(QJsonObject & json) {
                    clearFriends();
                    jsonToUsers(Friendable::linkables, json.take(block_friends).toArray());
                    clearGroups();
                    jsonToGroups(Groupable::linkables, json.take(block_groups).toArray());
                }
            public:
                Requests() { setSociableLimitations(true, true, true, true); }

                static QString cleanUrl(const QString & refreshed_url) { return refreshed_url.section('?', 0, 0); }

//                QJsonValue userTracksGroupsFriends(const QString & user_id) {
//                    return User::sRequest(
//                        User::baseUrlStr(
//                            qst_api_def, tkn_execute,
//                            {{ tkn_code, query_user_tracks_groups_friends.arg(user_id) }}
//                        ),
//                        call_type_json, 0, proc_json_extract
//                    );
//                }

                QJsonValue userInfo(const QString & user_id) {
                    Permissions perm = permissions(pr_object_content);

                    switch(perm) {
                        case perm_site: {
                            QJsonObject ret = User::sRequest(
                                User::baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {{ tkn_code, query_user_groups_friends(user_id) }}
                                ),
                                call_type_json, 0, proc_json_extract
                            );

                            QJsonObject media_ret = userMedia(user_id).toObject();

                            ret.insert(block_items_audio, media_ret.value(block_items_audio).toArray());
                            ret.insert(block_sets_audio, media_ret.value(block_sets_audio).toArray());

                            ret.insert(block_items_video, media_ret.value(block_items_video).toArray());
                            ret.insert(block_sets_video, media_ret.value(block_sets_video).toArray());

                            procSociables(ret);

                            return ret;
                        }

                        case perm_api: {
                            QJsonObject ret = User::sRequest(
                                User::baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {{ tkn_code, query_user_tracks_playlists_groups_friends(user_id) }}
                                ),
                                call_type_json, 0, proc_json_extract
                            );

                            if (!ret.value(tkn_albums_finished).toBool()) {
                                QJsonArray ar = ret.value(block_sets_audio).toArray();
                                tracksPlaylistsByUser(user_id, &ar, ret.value(tkn_albums_offset).toInt());
                                ret.insert(block_sets_audio, ar);
                            }

                            procSociables(ret);

                            return ret;
                        }

                        default: ;
                    }

                    return QJsonObject();
                }

                QJsonValue userMedia(const QString & user_id) {
                    Permissions perm = permissions(pr_object_content);

                    switch(perm) {
                        case perm_site: {
                            bool is_group = user_id[0] == '-';

                            Response * response = Manager::prepare() -> postFollowed( // TODO: improve me
                                QStringLiteral("http://vk.com/audio?act=load_audios_silent&al=1&gid=%1&id=%2&please_dont_ddos=2").arg(is_group ? user_id.mid(1) : QStringLiteral("0"), is_group ? QStringLiteral("0") : user_id),
                                Auth::dntHeader().unite({{ QStringLiteral("Referer"), QStringLiteral("http://vk.com/audios") % user_id }})
                            );

                            QString data = response -> toText();

                            QStringList parts = data.split(QStringLiteral("<!>"));

                            if (parts.length() < 7) {
                                Logger::obj().write("VK", "LOAD AUDIO", true);
                                return QJsonObject();
                            } else {
                                QHash<QString, QJsonArray> albums;

                                QJsonObject info_obj = QJsonDocument::fromJson(parts[6].toUtf8()).object();
                                QJsonObject playlists_info = info_obj.value(QStringLiteral("albums")).toObject();
                                QJsonArray playlists_res;

                                for(QJsonObject::Iterator playlist = playlists_info.begin(); playlist != playlists_info.end(); playlist++) {
                                    playlists_res << playlist.value();
                                    albums.insert(playlist.key(), QJsonArray());
                                }

                                ///////////////////////////////////

                                QJsonObject audio_info_obj = QJsonDocument::fromJson(parts[5].replace('\'', '"').toUtf8()).object();

                                QJsonArray tracks_arr = audio_info_obj.value(QStringLiteral("all")).toArray();
                                QJsonArray tracks_res;
                                for(QJsonArray::Iterator track_arr = tracks_arr.begin(); track_arr != tracks_arr.end(); track_arr++) {
                                    QJsonArray track = (*track_arr).toArray();
                                    QJsonObject track_obj;

                                    track_obj.insert(tkn_owner_id, track[0].toString().toInt());
                                    track_obj.insert(tkn_id, track[1].toString().toInt());
                                    track_obj.insert(tkn_url, track[2].toString());
//                                    track[3].toString().toInt() // bitrate
//                                    track[7].toString().toInt() // ?lyrics_id // '0' if empty

                                    track_obj.insert(tkn_artist, UnicodeDecoding::decodeHtmlEntites(track[5].toString()));
                                    track_obj.insert(tkn_title, UnicodeDecoding::decodeHtmlEntites(track[6].toString()));
                                    track_obj.insert(tkn_duration, Duration::toMillis(track[4].toString()) / 1000);
//                                    track_obj.insert(tkn_genre_id, ); // not presented

                                    if (track[8].toString().toInt() > 0) // album_id // '0' if empty
                                        albums[track[8].toString()] << track_obj;

                                    tracks_res << track_obj;
                                }

                                /////////////////////////////////////

                                QJsonArray uplaylists_res;
                                for(QJsonArray::Iterator playlist_it = playlists_res.begin(); playlist_it != playlists_res.end(); playlist_it++) {
                                    QJsonObject playlist_obj = (*playlist_it).toObject();

                                    playlist_obj.insert(tkn_items, albums[playlist_obj.value(tkn_id).toString()]);
                                    uplaylists_res << playlist_obj;
                                }

                                /////////////////////////////////////                               
                                QJsonObject res = videoByUser(user_id).toObject();

                                res.insert(block_items_audio, tracks_res);
                                res.insert(block_sets_audio, uplaylists_res);

                                return res;
                            }
                        break;}
                        case perm_api: {
                            QJsonObject ret = User::sRequest(
                                User::baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {{ tkn_code, query_user_tracks_playlists(user_id) }}
                                ),
                                call_type_json, 0, proc_json_extract
                            );

                            QJsonObject video_res = videoByUser(user_id).toObject();

                            ret.insert(block_items_video, video_res.value(block_items_video).toArray());
                            ret.insert(block_sets_video, video_res.value(block_sets_video).toArray());

                            if (!ret.value(tkn_albums_finished).toBool()) {
                                QJsonArray ar = ret.value(block_sets_audio).toArray();
                                tracksPlaylistsByUser(user_id, &ar, ret.value(tkn_albums_offset).toInt());
                                ret.insert(block_sets_audio, ar);
                            }
                            return ret;
                        }

                        default:;
                    }

                    return QJsonObject();
                }
            };
        }
    }
}

#endif // VK_REQUESTS
