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
                        /*sf_auth_api_has | */sf_auth_site_has | sf_site_user_content_auth_only |
                        sf_api_search_auth_only | sf_api_user_content_auth_only
                    );
                }

                QToolButton * initButton(QWidget * parent = 0);

                QJsonArray popular(const SearchLimit & limits) {
                    if (!isPermitted(pf_media_content)) return QJsonArray();

                    return isConnected() ? popularAuth(limits) : popularNoAuth(limits);
                }

                QJsonArray searchProc(const SearchLimit & limits) {
                    if (!isPermitted(pf_search)) return QJsonArray();

                    return isConnected() ? searchProcAuth(limits) : searchProcNoAuth(limits);
                }

                QString refresh(const QString & refresh_page) {
                    if (!isPermitted(pf_media_content)) return QString();

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
                    Html::Document html = Manager::prepare() -> getFollowed(url_site_base);

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

                        if (!form) return true;
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

                                Html::Tag * img = (*track) -> findFirst(".playThumb img");
                                QString js = img -> value(QStringLiteral("onclick")), url;
                                Info::extract(js, QStringLiteral("'http"), QStringLiteral("'"), url, 1);

                                track_obj.insert(tkn_grab_url, url);
                                track_obj.insert(tkn_grab_refresh, name_tag -> link());
                                track_obj.insert(tkn_skip_info, true);
                                track_obj.insert(tkn_grab_title, name);

                                arg -> append(track_obj, track + 1 == tracks.end());
                            }

                            result = !tracks.isEmpty();
                        }

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
