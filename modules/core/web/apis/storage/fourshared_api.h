#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/auth/oauth.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/misc/file_utils/extensions.h"
#include "modules/core/interfaces/iuser_interaction.h"

#include "fourshared_request_api.h"
#include "fourshared_request_site.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Api: public ISource, public RequestApi, public RequestSite, public IUserInteraction, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { Manager::addCookie(val_lang_cookie); }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_fourshared; }

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
                QJsonArray userInfo(QString & uid) {
                    Permissions res = permissions(pr_user_content);
                    if (!res) return QJsonArray();

                    return loadSet({{tkn_grab_refresh, uid}}).toArray();
                }

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

                QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType);

//                QString downloadLink(const QString & refresh_page) {
//                    if (refresh_page.isEmpty()) return QString();

//                    Html::Document doc = Web::Manager::prepare() -> getFollowed(QUrl(url_down_base % refresh_page.mid(12))) -> toHtml();
//                    return doc.find("a[href~'/download/']").link();
//                }
            protected:
                inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_json) {
//                    QString token = ptype == qpt_json ? apiToken() : siteToken();
                    return QUrlQuery(/*tkn_oauth_consumer % val_token*/);
                }
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        /*sf_auth_api_has | */ sf_auth_site_has |
                        sf_content_audio_has | sf_content_video_has |
                        sf_items_serachable |
                        sf_populable | sf_shareable |
                        sf_api_user_content_auth_only | sf_api_search_auth_only |
                        sf_site_user_content_auth_only
                    );
                }
                inline Headers siteHeaders() {
                    return Headers({{QStringLiteral("x-security"), Manager::cookie(QStringLiteral("Login"), url_html_site_base)}});
                }

                void saveAdditionals(QJsonObject & obj) { Manager::saveCookies(obj, QUrl(url_html_site_base)); }
                void loadAdditionals(QJsonObject & obj) { Manager::loadCookies(obj); }
                void clearAdditionals() { Manager::removeCookies(url_html_site_base); }

                QJsonArray procUserData(const QJsonObject & user_data);

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

                bool connectUserSite();

                inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate) { return url_api_base.arg(val_version) % predicate % val_json_ext; }

                bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false);
                inline bool endReached(QJsonObject & response, QueriableArg * arg) { return response.value(tkn_files).toArray().size() < arg -> per_request_limit; }
            };
        }
    }
}

#endif // FOURSHARED_API
