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

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Requests : public Sociable, public Auth, public Album, public Artist,
                    public Feed, public Playlist, public Set, public Track, public User {

                inline bool endReached(QJsonObject & response, QueriableArg * /*arg*/) {
                    QJsonObject pager_obj = JSON_OBJ(response, tkn_pager);
                    return (JSON_INT(pager_obj, tkn_page) + 1) * JSON_INT(pager_obj, LSTR("perPage")) >= JSON_INT(pager_obj, LSTR("total"));
                }
                inline bool extractStatus(QueriableArg * arg, QJsonObject & json, int & /*code*/, QString & /*message*/) {
                    while(true) { // INFO: check captcha validation456
                        if (JSON_HAS_KEY(json, JSON_ERR_FIELD)) {
                            Html::Document doc(JSON_STR(json, JSON_ERR_FIELD));

                            Html::Tag * form_tag = doc.findFirst(".form form");

                            if (!form_tag) {
                                qCritical() << "Captcha form is not found";
                                return false;
                            }

                            QString captcha_img = form_tag -> findFirst("img") -> src();

                            QHash<QString, QString> fields;
                            showingCaptcha(QUrl(captcha_img), fields[tkn_rep]);
                            if (fields[tkn_rep].isEmpty())
                                return false;

                            QUrl captcha_proc_url = form_tag -> serializeFormToUrl(fields);
                            if(captcha_proc_url.isRelative())
                                captcha_proc_url = QUrl(url_root).resolved(captcha_proc_url);
                            json = Manager::prepare() -> jsonGet(captcha_proc_url, arg -> headers, arg -> post_proc & proc_json_wrap);
                        }
                        else return true;
                    }
                }
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
                inline QUrlQuery genDefaultParams(const QuerySourceType & stype = qst_api) {
                    switch(stype) {
                        case qst_site:
                        case qst_site_alt1:
                            return QUrlQuery(
                                QString(
                                    LSTR("external-domain=") % LSTR("music.yandex.ua") %
                                    LSTR("&overembed=") % LSTR("false") %
                                    LSTR("&lang=") % LSTR("uk")
                                )
                            );

                        default: return QUrlQuery();
                    }
                }
            public:
                Requests() {
                    // need to add checking presents of cookies
//                    Manager::prepare() -> getFollowed(QUrl(url_root));
                }

                // here refresh_page must by eq to track id
                QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType) {
                    switch(itemMediaType) {
                        case dmt_audio: return trackUrl(refresh_page);
                        case dmt_video: return videoUrl(refresh_page);
                        default: return QString();
                    }
                }

                QJsonValue popular(const SearchLimit & limits) {
                    return setByType(set_popular_tracks, limits);
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() || limits.by_popularity())
                        return popular(limits);
                    else {
                        QJsonArray blocks;

                        if (limits.include_audio()) {
                            if (limits.by_artists())
                                blocks << artistsSearch(limits);

                            if (limits.by_songs_name() || limits.by_titles())
                                blocks << tracksSearch(limits);

                            if (limits.by_sets()) {
                                blocks << albumsSearch(limits);
                                blocks << playlistsSearch(limits);
                            }
                        }

                        if (limits.include_video())
                            blocks << videoSearch(limits);

                        return blocks;
                    }
                }
            };
        }
    }
}

#endif // YANDEX_REQUESTS_H
