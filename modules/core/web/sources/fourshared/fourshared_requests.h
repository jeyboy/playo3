#ifndef FOURSHARED_REQUESTS
#define FOURSHARED_REQUESTS

#include "fourshared_auth.h"
#include "fourshared_item.h"
#include "fourshared_track.h"
#include "fourshared_video.h"
#include "fourshared_set.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Requests : public Auth, public Item, public Set, public Track, public Video {
                QJsonValue procJson(const QJsonValue & json, const AdditionalProc & /*proc*/) {
                    QJsonObject user_data = json.toObject();

                    QJsonObject info = user_data[QStringLiteral("info")].toObject();

                    QJsonArray dirs = info.value(QStringLiteral("dirs")).toArray();
                    QJsonArray files = info.value(QStringLiteral("files")).toArray();

                    QJsonArray res;

                    for(QJsonArray::Iterator dir = dirs.begin(); dir != dirs.end(); dir++) {
                        QJsonObject dir_obj = (*dir).toObject();

                        if (dir_obj.value(QStringLiteral("canPlay")).toBool()) { // check on audio fides only :(
                            QJsonObject set_obj;

//                            set_obj.insert(tkn_grab_is_set, true);
                            set_obj.insert(tkn_media_type, dmt_any);

                            set_obj.insert(
                                tkn_loadable_cmd,
                                Cmd::build(
                                    sourceType(), cmd_mtd_load_set_data,
                                    {{CMD_ID, dir_obj.value(QStringLiteral("id")).toString()}}
                                ).toString()
                            );

                            set_obj.insert(tkn_grab_title, dir_obj.value(QStringLiteral("name")).toString());
//                            set_obj.insert(tkn_grab_refresh, dir_obj.value(QStringLiteral("id")).toString());

                            res.append(set_obj);
                        }
                    }

                    for(QJsonArray::Iterator file = files.begin(); file != files.end(); file++) {
                        QJsonObject file_obj = (*file).toObject();

                        QString fileType = file_obj.value(QStringLiteral("typeCss")).toString();
                        bool isAudio = fileType == QStringLiteral("audio");
                        bool isVideo = fileType == QStringLiteral("video");

                        if (isAudio || isVideo) {
                            QJsonObject item_obj;

                            QString name = file_obj.value(QStringLiteral("name")).toString(), ext;
                            Extensions::obj().extractExtension(name, ext);

                            if (file_obj.contains(QStringLiteral("prStyle")))
                                item_obj.insert(tkn_grab_art_url, file_obj[QStringLiteral("prStyle")].toString());

                            item_obj.insert(tkn_skip_info, true);
                            item_obj.insert(tkn_grab_refresh, file_obj.value(QStringLiteral("id")).toString());
                            item_obj.insert(tkn_grab_title, name);
                            item_obj.insert(tkn_grab_extension, ext);

                            item_obj.insert(tkn_media_type, isAudio ? dmt_audio : dmt_video);

                            res.append(item_obj);
                        }
                    }

                    return res;
                }
                bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply) {
                    Html::Document doc = reply -> toHtml(removeReply);

                    bool result = false;

                    switch(arg -> post_proc) {
                        case proc_tracks1: {
                            Html::Set tracks = doc.find(".listView.res_table tr[valign='top']");;

                            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                                QJsonObject track_obj;

                                Html::Tag * name_tag = (*track) -> findFirst(".fname a");

                                QString name = name_tag -> text().trimmed(), ext;
                                Extensions::obj().extractExtension(name, ext);

                                Html::Tag * play_img = (*track) -> findFirst(".playThumb img");
                                QString js = play_img -> value(QStringLiteral("onclick")), url;
                                Info::extract(js, QStringLiteral("http"), QStringLiteral("'"), url);

                                Html::Tag * img = (*track) -> findFirst(".advancedThumb .imgDiv table img");

                                if (img)
                                    track_obj.insert(tkn_grab_art_url, img -> value(QStringLiteral("src")));

                                track_obj.insert(tkn_grab_url, url);
                                track_obj.insert(tkn_grab_refresh, name_tag -> link());
                                track_obj.insert(tkn_skip_info, true);
                                track_obj.insert(tkn_grab_title, name);
                                track_obj.insert(tkn_grab_extension, ext);
                                track_obj.insert(tkn_media_type, dmt_audio);

                                arg -> append(track_obj, track + 1 == tracks.end());
                            }

                            result = !tracks.isEmpty();
                        break;}

                        case proc_video1: {
                            Html::Set tracks = doc.find(".listView.res_table tr[valign='top']");;

                            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                                QJsonObject track_obj;

                                Html::Tag * name_tag = (*track) -> findFirst(".fname a");

                                QString name = name_tag -> text().trimmed(), ext;
                                Extensions::obj().extractExtension(name, ext);

                                Html::Tag * img = (*track) -> findFirst(".advancedThumb .imgDiv table img");

                                if (img)
                                    track_obj.insert(tkn_grab_art_url, img -> value(QStringLiteral("src")));

                                track_obj.insert(tkn_grab_refresh, name_tag -> link());
                                track_obj.insert(tkn_skip_info, true);
                                track_obj.insert(tkn_grab_title, name);
                                track_obj.insert(tkn_grab_extension, ext);
                                track_obj.insert(tkn_media_type, dmt_video);

                                arg -> append(track_obj, track + 1 == tracks.end());
                            }

                            result = !tracks.isEmpty();
                        break;}

                        default: ;
                    }

                    return result;
                }
                inline bool endReached(QJsonObject & response, QueriableArg * arg) { return response.value(tkn_files).toArray().size() < arg -> per_request_limit; }
            protected:
                inline virtual ~Requests() {}

                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        /*sf_auth_api_has | */ sf_auth_site_has |
                        sf_content_audio_has | sf_content_video_has |
                        sf_items_serachable |
                        sf_populable | sf_shareable | sf_packable |
                        sf_api_object_content_auth_only | sf_api_search_media_auth_only |
                        sf_site_object_content_auth_only
                    );
                }
                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_api_base: return url_api_base % predicate % val_json_ext;
                        case qst_api_search: return url_api_search % predicate % val_json_ext;
                        case qst_site_base: return url_html_site_search % predicate;
                        case qst_site_search: return url_html_site_search % predicate;
                        default: return QString();
                    }
                }
                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_api_base) {
//                    QString token = ptype == qpt_json ? apiToken() : siteToken();
                    return QUrlQuery(/*tkn_oauth_consumer % val_token*/);
                }

                void saveAdditionals(QJsonObject & obj) { Manager::saveCookies(obj, QUrl(url_html_site_base)); }
                void loadAdditionals(QJsonObject & obj) {
                    Manager::loadCookies(obj);
                    Manager::addCookie(val_lang_cookie);
                }
                void clearAdditionals() { Manager::removeCookies(url_html_site_base); }

                QJsonValue loadSetData(const QUrlQuery & attrs) {
                    return itemsByCollection(attrs.queryItemValue(CMD_ID));
                }
                QJsonValue loadSetData(const QString & attrs) { return loadSetData(QUrlQuery(attrs)); }

                bool connectUserSite() {
                    QString user_id;

                    if (connectSite(user_id, error)) {
                        setSiteUserID(user_id);
                        return true;
                    }

                    return false;
                }
                bool connectUserApi() { return connectApi(); }
            public:
                QJsonValue searchProc(const SearchLimit & limits) {
                    QJsonArray blocks;

                    if (limits.include_audio())
                        blocks << tracksSearch(limits);

                    if (limits.include_video())
                        blocks << videoSearch(limits);

                    return blocks;
                }

                QJsonValue popular(const SearchLimit & limits) {
                    QJsonArray blocks;

                    if (limits.include_audio())
                        blocks << setByType(set_popular_audio, limits);

                    if (limits.include_video())
                        blocks << setByType(set_popular_video, limits);

                    return blocks;
                }

                QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType) {
                    if (!permissions(pr_media_content)) return QString();

                    QUrl url(refresh_page);
                    bool is_uid = url.host().isEmpty(); // get link from uid
                    QString res;

                    switch(itemMediaType) {
                        case dmt_audio: {
                            if (is_uid)
                                res = trackUrlFromId(refresh_page);
                            else
                                res = trackUrlFromPath(refresh_page);
                        break;}

                        case dmt_video: {
                            if (is_uid)
                                res = videoUrlFromId(refresh_page);
                            else
                                res = videoUrlFromPath(refresh_page);
                        break;}

                        default: ;
                    }

                    return res.isEmpty() ? res : res % QStringLiteral("\r\n") % cookies();
                }

                QJsonValue openSet(const QUrlQuery & attrs) {
                    return QJsonArray() << Set::setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }
            };
        }
    }
}

#endif // FOURSHARED_REQUESTS
