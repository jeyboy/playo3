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
                        case dmt_unknow: return QString();
                    }
                }

                QJsonValue popular(const SearchLimit & /*limits*/) {
                    return setByType(set_popular_tracks);
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() || limits.by_popularity())
                        return popular(limits);
                    else return tracksSearch(limits);
                }
            public:

            };
        }
    }
}

#endif // OD_REQUESTS
