#ifndef OD_REQUESTS
#define OD_REQUESTS

#include "od_user.h"
#include "od_artist.h"
#include "od_auth.h"
#include "od_playlist.h"
#include "od_collection.h"
#include "od_radio.h"
#include "od_set.h"
#include "od_track.h"
#include "od_video.h"
#include "od_video_playlist.h"

#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Requests : public ISource, public Sociable, public Artist, public User, public Auth, public Collection,
                    public Playlist, public Radio, public Set, public Track, public Video, public VideoPlaylist {
            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_primary_source |
                        /*sf_auth_api_has |*/ sf_auth_site_has | sf_site_online_credentials_req |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable | /*sf_groups_serachable |*/
                        sf_sociable_users | /*sf_sociable_groups |*/ sf_shareable | sf_packable |
//                        sf_recomendable_by_item | sf_recomendable_by_user |
                        /*sf_newable |*/ sf_populable |

                        /*sf_content_lyrics_has |*/ sf_content_audio_has | sf_content_video_has |
                        sf_content_photo_has | sf_content_news_has |

                        sf_site_auth_mandatory
                    );
                }

                Permissions permissions(const PermitRequest & req_perm = pr_search_media) { return ISource::permissions(req_perm); }

                void saveAdditionals(QJsonObject & obj) {
                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_root));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                    Manager::removeCookies(QUrl(url_root));
                }

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_site_audio: return url_base_audio % predicate;
                        default: return url_root % predicate;
                    }
                }

                QString baseUrlStr(const QuerySourceType & stype, const QString & predicate, const QUrlQuery & query) {
                    return Auth::baseUrlStr(stype, predicate % tkn_coma_dot % tkn_jsessionid % siteToken(), query);
                }

//                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_site_def) {
//                    return QUrlQuery();
////                    return QUrlQuery(tkn_jsessionid % siteToken());
//                }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    QJsonObject chunk_obj = response.value(tkn_chunk).toObject();
                    if (chunk_obj.isEmpty()) return false;
                    return chunk_obj.value(tkn_count).toInt() < arg -> per_request_limit/*OD_LIMIT_PER_REQUEST*/;
                }

                bool connectUserSite() {
                    QString user_id, hash;

                    if (siteConnection(user_id, hash, error)) {
                        setSiteUserID(user_id);
//                        setSiteHash(hash);

                        return true;
                    }
                    else return false;
                }

                bool sessionIsValid() { return !Auth::hasError(userInfo().toObject()); }

                bool takeOnlineCredentials() {
                    setSiteToken(Auth::grabSID());
                    return sessionIsValid();
                }

                QString refresh(const QString & item_id, const DataMediaType & item_media_type) {
                    switch(item_media_type) {
                        case dmt_audio: return trackUrl(item_id);
                        case dmt_video: return QString();
                        default:;
                    }

                    return QString();
                }

                QJsonValue popular(const SearchLimit & /*limits*/) {
                    return setByType(set_popular_tracks);
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() || limits.by_popularity())
                        return popular(limits);
                    else return tracksSearch(limits);
                }

                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();
                        linkables << Linkable(
                            obj.value(tkn_id).toString(),
                            obj.value(tkn_full_name).toString()
                        );
                    }
                }

//                inline void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
//                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
//                        QJsonObject obj = (*obj_iter).toObject();
//                        linkables << Linkable(
//                            QString::number(obj.value(tkn_id).toInt()),
//                            obj.value(QStringLiteral("name")).toString(),
//                            obj.value(tkn_screen_name).toString(),
//                            obj.value(tkn_photo).toString()
//                        );
//                    }
//                }
            public:
                QJsonValue userInfo() {
                    QJsonObject res = User::userInfo().toObject();

                    QJsonArray audios = res.value(tkn_tracks).toArray();
                    if (audios.isEmpty()) {
                        int totalTracks = res.value(QStringLiteral("totalTracks")).toInt();
                        if (totalTracks > 0)
                            res.insert(
                                Od::tkn_tracks,
                                userMedia(idToStr(res.value(tkn_me))).
                                    toObject().value(tkn_tracks)
                            );
                    }

                    clearFriends();
                    jsonToUsers(Friendable::linkables, res.take(tkn_friends).toArray());
//                   clearGroups();
//                   jsonToGroups(Groupable::linkables, res.take(tkn_friends).toArray());
                    return res;
                }

                inline QMap<QString, QString> setsList() {
                    return {
                        {setTypeToStr(set_popular_tracks),      QString::number(set_popular_tracks)},
                        {setTypeToStr(set_popular_artists),     QString::number(set_popular_artists)},
                        {setTypeToStr(set_popular_tuners),      QString::number(set_popular_tuners)},
                        {setTypeToStr(set_popular_collections), QString::number(set_popular_collections)},
                        {setTypeToStr(set_popular_albums),      QString::number(set_popular_albums)},
                        {setTypeToStr(set_listened),            QString::number(set_listened)},
                        {setTypeToStr(set_downloaded),          QString::number(set_downloaded)}
                    };
                }
                inline QJsonValue takeSet(const QString & set_params) {
                    QStringList params = set_params.split('|', QString::SkipEmptyParts);
                    SetType set_type = (SetType)params.first().toInt();
                    QString attrs = params.count() > 1 ? params.last() : QString();

                    return setByType(set_type, attrs);
                }
            };
        }
    }
}

#endif // OD_REQUESTS
