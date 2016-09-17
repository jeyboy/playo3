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
                                captcha_proc_url = QUrl(baseUrlStr(qst_site_base)).resolved(captcha_proc_url);
                            json = Manager::prepare() -> jsonGet(captcha_proc_url, arg -> headers, arg -> post_proc & proc_json_wrap);
                        }
                        else return true;
                    }
                }

                void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    int i = 0;
//                    if (arr.size() == 0 || (arr.size() == 1 && arr[0].toObject().contains(QStringLiteral("errors")))) return;

                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
//                        QJsonObject obj = (*obj_iter).toObject();

//                        QString full_name = obj.value(tkn_full_name).toString();
//                        QString perma = obj.value(tkn_permalink).toString();

//                        linkables << Linkable(
//                            QString::number(obj.value(tkn_id).toInt()),
//                            full_name.isEmpty() ? obj.value(tkn_username).toString() : full_name,
//                            perma,
//                            obj.value(tkn_avatar_url).toString()
//                        );
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
                    Manager::saveCookies(obj, QUrl(baseUrlStr(qst_site_base)));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                    Manager::removeCookies(QUrl(baseUrlStr(qst_site_base)));
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


                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate = QString()) {
                    QString locale = siteLocale(val_default_locale);

                    switch(stype) {
                        case qst_site: {        return url_site_v1.arg(locale) % predicate; }
                        case qst_site_alt1: {   return url_site_v2.arg(locale) % predicate; }
                        default: {              return url_root.arg(locale) % predicate; }
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
                Requests() {}

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

                QJsonValue userInfo(const QString & user_id) {
                    Permissions perm = permissions(pr_object_content);

                    if (perm > perm_none) {
                        clearFriends();
                        jsonToUsers(Friendable::linkables, friends(user_id));

                        return userMedia(user_id);
                    }

                    return QJsonArray();
                }

                QJsonValue userMedia(const QString & user_id) {
                    Permissions perm = permissions(pr_object_content);

                    if (perm > perm_none) {

                    }

//                    switch(perm) {
//                        case perm_site: {
//                            Response * response = Manager::prepare() -> postFollowed(
//                                IQueriable::baseUrlStr(
//                                    qst_site, LSTR("al_audio.php"),
//                                    {
//                                        { LSTR("act"), LSTR("load_silent") },
//                                        { LSTR("al"), LSTR("1") },
//                                        { LSTR("album_id"), LSTR("-2") },
//                                        { LSTR("band"), LSTR("true") },
//                                        { LSTR("owner_id"), user_id }
//                                    }
//                                ),
//                                Auth::dntHeader()
//                            );

//                            QJsonArray tracks_res, items = JSON_ARR(RESPONSE_TO_JSON_OBJECT(response), LSTR("list"));
//                            QHash<QString, QJsonArray> albums;

//                            Track::prepareTracks(items, tracks_res, &albums);
//                            /////////////////////////////////////

//                            QJsonArray playlists_res;

//                            for(QHash<QString, QJsonArray>::Iterator album = albums.begin(); album != albums.end(); album++) {
//                                playlists_res << QJsonObject {
//                                    {tkn_items, album.value()},
//                                    {tkn_id, album.key()},
//                                    {tkn_title, QString::number(playlists_res.size())} // for real names need send request to get user playlists
//                                };
//                            }

//                            /////////////////////////////////////
//                            QJsonArray res = videoByUser(user_id).toArray();

//                            res.prepend(prepareBlock(dmt_audio, tracks_res, {{tkn_dir_name, LSTR("Tracks")}}));
//                            res.prepend(prepareBlock(dmt_audio_set, playlists_res, {{tkn_dir_name, LSTR("Tracks")}}));

//                            return res;
//                        break;}

//                        case perm_api: {
//                            QJsonObject ret = User::sRequest(
//                                User::baseUrlStr(
//                                    qst_api, tkn_execute,
//                                    {{ tkn_code, query_user_tracks_playlists(user_id) }}
//                                ),
//                                call_type_json, 0, proc_json_extract
//                            );

//                            QJsonArray res = videoByUser(user_id).toArray();

//                            if (!ret.value(tkn_albums_finished).toBool()) {
//                                QJsonArray ar = ret.value(block_sets_audio).toArray();
//                                tracksPlaylistsByUser(user_id, &ar, ret.value(tkn_albums_offset).toInt());
//                                ret.insert(block_sets_audio, ar);
//                            }

//                            res.prepend(prepareBlock(dmt_audio, ret.value(block_items_audio), {{tkn_dir_name, LSTR("Tracks")}}));
//                            res.prepend(prepareBlock(dmt_audio_set, ret.value(block_sets_audio), {{tkn_dir_name, LSTR("Tracks")}}));

//                            return ret;
//                        break;}

//                        default:;
//                    }

//                    return QJsonArray();
                }
            };
        }
    }
}

#endif // YANDEX_REQUESTS_H
