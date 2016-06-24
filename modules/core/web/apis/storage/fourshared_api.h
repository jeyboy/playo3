#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/auth/oauth.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/misc/file_utils/extensions.h"

#include "fourshared_request_api.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Api: public ISource, public RequestApi, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { Manager::addCookie(val_lang_cookie); }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_fourshared; }
                inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = qpt_json) {
//                    QString token = ptype == qpt_json ? apiToken() : siteToken();
                    return QUrlQuery(/*tkn_oauth_consumer % val_token*/);
                }
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        /*sf_auth_api_has | sf_api_user_content_auth_only | */sf_auth_site_has
                            | sf_site_user_content_auth_only | sf_api_search_auth_only
                    );
                }

                void saveAdditionals(QJsonObject & obj) {
                    Manager::saveCookies(obj, QUrl(url_html_site_base));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Manager::loadCookies(obj);
                }

                QToolButton * initButton(QWidget * parent = 0);

                QJsonValue popular(const SearchLimit & limits) {
                    Permissions res = permissions(pr_media_content);
                    if (!res) return QJsonArray();

                    return res == perm_api ? popularApi(limits) : popularSite(limits);
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    Permissions res = permissions(pr_search);
                    if (!res) return QJsonArray();

                    return res == perm_api ? searchProcApi(limits) : searchProcSite(limits);
                }

                inline void userInfo(QString & uid, Func * func) { ThreadUtils::obj().run(this, &Api::userInfo, uid, func); }
                QJsonValue userInfo(QString & uid) {
                    Permissions res = permissions(pr_user_content);
                    if (!res) return QJsonArray();

                    return loadSet({{tkn_grab_refresh, uid}});
                }

                QJsonValue loadSet(const QVariantMap & attrs) {
                    QString url(url_html_change_dir % QStringLiteral("?dirId=") % siteToken());

                    Headers headers = {{QStringLiteral("x-security"), Manager::cookie(QStringLiteral("Login"), url_html_site_base)}};

                    return saRequest(
                        url,
                        call_type_html,
                        0,
                        (AdditionalProc)attrs.value(tkn_grab_set_parser, proc_tracks1).toInt(),
                        QStringList(),
                        call_method_post,
                        headers
                    );
                }

                QString refresh(const QString & refresh_page, const DataMediaType & /*itemMediaType*/) {
                    if (!permissions(pr_media_content)) return QString();

                    Html::Document doc = Web::Manager::prepare() -> getFollowed(refresh_page) -> toHtml();
                    return doc.find("input.jsD1PreviewUrl").value();
                }

                QString downloadLink(const QString & refresh_page) {
                    if (refresh_page.isEmpty()) return QString();

                    Html::Document doc = Web::Manager::prepare() -> getFollowed(QUrl(url_down_base % refresh_page.mid(12))) -> toHtml();
                    return doc.find("a[href~'/download/']").link();
                }
            protected:
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

                bool connectUserSite() {
                    Manager::removeCookies(url_html_site_base);
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

                inline QString baseUrlStr(const QString & predicate) { return url_api_base.arg(val_version) % predicate % val_json_ext; }

                bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
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
                                Info::extract(js, QStringLiteral("'http"), QStringLiteral("'"), url, 1);

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
            };
        }
    }
}

#endif // FOURSHARED_API
