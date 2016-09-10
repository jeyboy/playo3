#ifndef YANDEX_REQUESTS_H
#define YANDEX_REQUESTS_H

#include "yandex_auth.h"
#include "yandex_album.h"
#include "yandex_artist.h"
#include "yandex_feed.h"
#include "yandex_playlist.h"
#include "yandex_set.h"
#include "yandex_track.h"
#include "yandex_user.h"

//#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Requests : /*public Sociable,*/ public User, public Auth,
                    public Playlist, public Set, public Track {

                inline bool endReached(QJsonObject & /*response*/, QueriableArg * /*arg*/) { return true; }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return true; }
            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_primary_source |
                        sf_auth_site_has | sf_site_online_credentials_req |
                        sf_items_serachable | sf_sets_serachable | sf_packable |
//                        sf_sociable_users /*| sf_sociable_groups*/ | sf_shareable |
//                        /*sf_recomendable_by_item |*/ sf_recomendable_by_user |
//                        /*sf_newable |*/ sf_populable |

                        sf_content_audio_has | sf_content_video_has
//                        sf_content_photo_has | sf_content_news_has |

//                        sf_site_auth_mandatory
                    );
                }

                void saveAdditionals(QJsonObject & obj) {
//                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_root));
                }
                void loadAdditionals(QJsonObject & obj) {
//                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
//                    clearFriends();
//                    clearGroups();
                    Manager::removeCookies(QUrl(url_root));
                }

                bool connectUserSite() {
                    QString user_id;

                    if (siteConnection(user_id, error)) {
                        setSiteUserID(user_id);

                        return true;
                    }
                    else return false;
                }

                bool hasOfflineCredentials()     { return false; /*!siteToken().isEmpty();*/ }
                bool takeOfflineCredentials() {
                    if (hasOfflineCredentials()) return true;

//                    QString html = Manager::prepare() -> getFollowed(url_site_base) -> toText();
//                    QRegularExpression reg("sc_version = \"(\\d+)\"");
//                    QRegularExpressionMatch match = reg.match(html);

//                    if (match.hasMatch()) {
//                        QString app_id = match.captured(1);
//                        setSiteHash(app_id);

//                        QRegularExpression js_reg("(http[:\\/\\w\\.\\-]+app-[\\w-]+\\.js)");
//                        match = js_reg.match(html);
//                        if (match.hasMatch()) {
//                            QString js_url = match.captured(1);
//                            QString js_content = Manager::prepare() -> getFollowed(js_url) -> toText();

//                            QRegularExpression id("client_id:\"(\\w+)\"");
//                            match = id.match(js_content);
//                            if (match.hasMatch()) {
//                                QString token = match.captured(1);
//                                setSiteToken(token);
//                                return true;
//                            }
//                        }
//                    }

                    return false;
                }


                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_site: { return url_site_v1 % predicate; }
                        case qst_site_alt1: { return url_site_v2 % predicate; }
                        default: { return url_root % predicate; }
                    }
                }
                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_api) { return QUrlQuery(); }
            public:
                Requests() {
                    // need to add checking presents of cookies
                    Manager::prepare() -> getFollowed(QUrl(url_root));
                }

                // here refresh_page must by eq to track id
                QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType) {
                    switch(itemMediaType) {
                        case dmt_audio: return trackUrl(refresh_page);
//                        case dmt_video: return trackUrl(refresh_page);
                        default: return QString();
                    }
                }

                QJsonValue popular(const SearchLimit & limits) {
                    return sRequest(
                        topUrl(tkn_tracks, limits.genre),
                        call_type_json
                    ).value(tkn_tracks).toArray();

                //                    return sQuery(topUrl(tkn_tracks, limits.genre)).value(tkn_tracks).toArray();
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() || limits.by_popularity()) {
                        return popular(limits);
                    } else {
                        QString tkn = limits.by_artists() ? tkn_artists : tkn_tracks;

                        return sRequest(
                            searchUrl(limits.predicate, tkn),
                            call_type_json
                        ).value(tkn).toObject().value(QStringLiteral("items")).toArray();

                //                        if (limits.by_artists())
                //                            return sQuery(searchUrl(limits.predicate, tkn_artists)).value(tkn_artists).toObject().value(tkn).toArray();
                //                        else
                //                            return sQuery(searchUrl(limits.predicate, tkn_tracks)).value(tkn_tracks).toObject().value(tkn).toArray();
                    }
                }
            };
        }
    }
}

#endif // YANDEX_REQUESTS_H
