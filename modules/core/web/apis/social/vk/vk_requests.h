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
                        sf_recomendable_by_item | sf_recomendable_by_user |
                        sf_auth_api_has /*| sf_auth_site_has*/ | sf_auth_mandatory
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
                bool connectUserSite() { return false; } // TODO: write me

                QString refresh_postproc(const QString & refreshed_url) { return refreshed_url.section('?', 0, 0); }

                QJsonValue popular(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(DMT_AUDIO, setByType(popular_tracks, PPACK(Auth::boolToStr(false), limits.genre)));

//                    if (limits.include_video())
//                        res.insert(DMT_VIDEO, videoPopular(limits));

                    return res;
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() && limits.by_popularity())
                        return popular(limits);
                    else {
                        QJsonObject res;

                        if (limits.include_audio())
                            res.insert(DMT_AUDIO, tracksSearch(limits));

                        if (limits.include_video())
                            res.insert(DMT_VIDEO, videoSearch(limits));

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

                void saveAdditionals(QJsonObject & obj) { Sociable::toJson(obj); }
                void loadAdditionals(QJsonObject & obj) { Sociable::fromJson(obj); }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                }

                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();
                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            QString(obj.value(QStringLiteral("first_name")).toString() % ' ' % obj.value(QStringLiteral("last_name")).toString()),
                            obj.value(tkn_screen_name).toString(),
                            obj.value(tkn_photo).toString()
                        );
                    }
                }

                inline void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();
                        linkables << Linkable(
                            QString::number(obj.value(tkn_id).toInt()),
                            obj.value(QStringLiteral("name")).toString(),
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

            //    inline QString adapteUid(QString & uid) { return uid == "0" ? userID() : uid; }

            public:
                Requests() { setSociableLimitations(true, true, true, true); }

                QJsonValue userTracksGroupsFriends(const QString & user_id) {
                    return User::sRequest(
                        User::baseUrlStr(
                            qst_api_def, tkn_execute,
                            {{ tkn_code, query_user_tracks_groups_friends.arg(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );
                }

                QJsonObject userInfo(const QString & user_id) {
                    QJsonObject ret = User::sRequest(
                        User::baseUrlStr(
                            qst_api_def, tkn_execute,
                            {{ tkn_code, query_user_tracks_playlists_groups_friends.arg(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );

                    if (!ret.value(tkn_albums_finished).toBool()) {
                        QJsonArray ar = ret.value(tkn_albums).toArray();
                        tracksPlaylistsByUser(user_id, &ar, ret.value(tkn_albums_offset).toInt());
                        ret.insert(tkn_albums, ar);
                    }
                    return ret;
                }

                QJsonObject userTracksPlaylists(const QString & user_id) {
                    QJsonObject ret = User::sRequest(
                        User::baseUrlStr(
                            qst_api_def, tkn_execute,
                            {{ tkn_code, query_user_tracks_playlists.arg(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );

                    if (!ret.value(tkn_albums_finished).toBool()) {
                        QJsonArray ar = ret.value(tkn_albums).toArray();
                        tracksPlaylistsByUser(user_id, &ar, ret.value(tkn_albums_offset).toInt());
                        ret.insert(tkn_albums, ar);
                    }
                    return ret;
                }
            };
        }
    }
}

#endif // VK_REQUESTS
