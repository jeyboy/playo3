#ifndef FOURSHARED_REQUESTS
#define FOURSHARED_REQUESTS

#include "fourshared_auth.h"
#include "fourshared_item.h"
#include "fourshared_set.h"

#include "modules/core/interfaces/isource.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Requests : public ISource, public Auth, public Item, public Set {
                QJsonArray procUserData(const QJsonObject & user_data) {
                    QJsonObject info = user_data[QStringLiteral("info")].toObject();

                    QJsonArray dirs = info.value(QStringLiteral("dirs")).toArray();
                    QJsonArray files = info.value(QStringLiteral("files")).toArray();

                    QJsonArray res;

                    for(QJsonArray::Iterator dir = dirs.begin(); dir != dirs.end(); dir++) {
                        QJsonObject dir_obj = (*dir).toObject();

                        if (dir_obj.value(QStringLiteral("canPlay")).toBool()) { // check on audio fides only :(
                            QJsonObject set_obj;


                            set_obj.insert(tkn_grab_is_set, true);
                            set_obj.insert(tkn_grab_title, dir_obj.value(QStringLiteral("name")).toString());
                            set_obj.insert(tkn_grab_refresh, dir_obj.value(QStringLiteral("id")).toString());

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
            protected:
                inline virtual ~Requests() {}

                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        /*sf_auth_api_has | */ sf_auth_site_has |
                        sf_content_audio_has | sf_content_video_has |
                        sf_items_serachable |
                        sf_populable | sf_shareable |
                        sf_api_object_content_auth_only | sf_api_search_media_auth_only |
                        sf_site_object_content_auth_only
                    );
                }

                Permissions permissions(const PermitRequest & req_perm = pr_search_media) { return ISource::permissions(req_perm); }

//                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
//                    switch(stype) {
//                        case qst_api_def: return url_api_base % predicate % val_default_format;
//                        case qst_site_def: return url_api_base % predicate;
//                        case qst_site_alt1: return url_api_base2 % predicate;
//                        default: return QString();
//                    }
//                }

                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) { return url_api_base.arg(val_version) % predicate % val_json_ext; }

                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_api_def) {
//                    QString token = ptype == qpt_json ? apiToken() : siteToken();
                    return QUrlQuery(/*tkn_oauth_consumer % val_token*/);
                }

                inline Headers siteHeaders() {
                    return Headers({{QStringLiteral("x-security"), Manager::cookie(QStringLiteral("Login"), url_html_site_base)}});
                }

                void saveAdditionals(QJsonObject & obj) { Manager::saveCookies(obj, QUrl(url_html_site_base)); }
                void loadAdditionals(QJsonObject & obj) {
                    Manager::loadCookies(obj);
                    Manager::addCookie(val_lang_cookie);
                }
                void clearAdditionals() { Manager::removeCookies(url_html_site_base); }

                QJsonValue searchProc(const SearchLimit & limits) { return itemsSearch(limits); }

                QJsonValue loadSet(const QVariantMap & attrs) {
                    QString url(url_html_change_dir % QStringLiteral("?dirId=") % attrs.value(tkn_grab_refresh).toString());

                    return procUserData(sRequest(
                        url,
                        call_type_json,
                        0,
                        proc_none,
                        QStringList(),
                        call_method_post,
                        siteHeaders()
                    ));
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

                bool connectUserSite() {
                    Html::Document html = Manager::prepare() -> getFollowed(url_html_site_base) -> toHtml();

                    Html::Tag * form = html.findFirst("form[name='loginForm']");
                    if (!form) return false;
                    QHash<QString, QString> vals;
                    QString err;

                    while(true) {
                        if (!showingLogin(QStringLiteral("4shared auth"), vals[QStringLiteral("login")], vals[QStringLiteral("password")], err))
                            return false;

                        QUrl form_url = form -> serializeFormToUrl(vals);
                        Html::Document doc = Manager::prepare() -> formFollowed(form_url) -> toHtml();
                        form = doc.findFirst("form[name='loginForm']");

                        if (!form) {
                            Html::Document acc_doc = Manager::prepare() -> getFollowed(url_html_user_root) -> toHtml();
                            Html::Tag * root_id_tag = acc_doc.findFirst("input.jsRootId");
                            if (root_id_tag)
                                setSiteUserID(root_id_tag -> value());
                            else
                                qDebug() << "Cannot find root id for current user";

                            return true;
                        }
                    }

                    return false;
                }

                bool connectUserApi() {
                    // INFO: эти ебанные пидорасы не могут полгода уже как починить свой oauth
////                    OAuth auth("22abeb63487b7f6b75051079b7e610b1", "71970e08961f3a78e821f51f989e6cb568cbd0ce");
////                    bool res = auth.initiate(url_api_base.arg(val_version) % QStringLiteral("oauth/initiate"));
////                    if (res)
////                        res = auth.autorize(url_api_base.arg(val_version) % QStringLiteral("oauth/authorize"));
////                    if (res)
////                        res = auth.access(url_api_base.arg(val_version) % QStringLiteral("oauth/token"));

////                    return res;

////                    OAuth auth("i0SEyiZts1mMGizAVDjn4nhOH", "O52MLLcKA0LknMLf47ZKyvTQhwdhLshsSY865hEgERS6ediuSj", QStringLiteral("http://localhost:3000/"));
////                    bool res = auth.initiate(QStringLiteral("https://api.twitter.com/oauth/request_token"));
////                    if (res)
////                        res = auth.autorize(QStringLiteral("https://api.twitter.com/oauth/authenticate"));
////                    if (res)
////                        res = auth.access(QStringLiteral("https://api.twitter.com/oauth/access_token"));

//                    return res;

                    return false;
                }
            public:
                QJsonValue popular(const SearchLimit & limits) { return setByType(set_popular, limits); }

                QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType) {
                    if (!permissions(pr_media_content)) return QString();

                    QUrl url(refresh_page);
                    bool is_uid = url.host().isEmpty(); // get link from uid

                    switch(itemMediaType) {
                        case dmt_audio: {
                            if (is_uid) {
                                url = QUrl(QStringLiteral("http://www.4shared.com/web/rest/v1/playlist?itemType=file&beforeId=null&afterId=null&index=0&itemId=") % refresh_page);
                                QString res = Manager::prepare() -> putFollowed(url, siteHeaders()) -> toText();
                                Info::extract(res, QStringLiteral("http"), QStringLiteral("\""), res);
                                return res;
                            } else {
                                Html::Document doc = Web::Manager::prepare() -> getFollowed(refresh_page) -> toHtml();
                                return doc.find("input.jsD1PreviewUrl").value();
                            }
                        }

                        case dmt_video: {
                            if (is_uid) {
                                url = QUrl(QStringLiteral("http://www.4shared.com/web/account/videoPreview?fileID=") % refresh_page);
                                QString res = Manager::prepare() -> getFollowed(url, siteHeaders()) -> toText();
                                res = Info::extractLimitedBy(res, QStringLiteral("file: \""), QStringLiteral("\""));
                                return res;
                            } else {
                                //TODO: write me
                            }
                        }
                        default: return QString();
                    }
                }
            };
        }
    }
}

#endif // FOURSHARED_REQUESTS
