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
            class Api: public RequestApi, public ISource, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_fourshared; }
                inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = json) {
                    return QUrlQuery(tkn_oauth_consumer % (token().isEmpty() ? val_token : token()));
                }
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_auth_api_has | sf_auth_site_has | sf_site_user_content_auth_only |
                        sf_api_search_auth_only | sf_api_user_content_auth_only
                    );
                }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

//                QToolButton * initButton(QWidget * parent = 0);

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
            public slots:
                inline void openTab() { ISource::openTab(userID()); }
                inline void disconnectUser() {
                    clearParams();
                    initButton();
                }

            protected:
                bool connectApi() {
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

                bool connectSite() {
//                    Html::Document html = Manager::prepare() -> getFollowed(url_site_base);

//                    <form name="loginForm" autocomplete="on" method="post" action="https://www.4shared.com/web/login" class="loginform jsLoginForm">

//                          <div class="i round4 headLoginDropdown">
//                            <div class="dropdownTopArrow"></div>
//                            <div style="display:none;" id="iloginRejectReason" class="round1 alert jsErrorPlace"></div>
//                            <div class="wr">
//                              <div data-element="h10_1" class="input-light-small round4 marginT5 gaClick">
//                                <input type="text" autocomplete="on" placeholder="Логин" name="login" class="jsInputLogin">
//                              </div>
//                              <div data-element="h10.2" class="input-light-small round4 marginT5 gaClick">
//                                <input type="password" autocomplete="on" placeholder="Пароль" name="password" class="jsInputPassword">
//                              </div>

//                              <input type="hidden" value="http%3A%2F%2Fwww.4shared.com%2Faccount%2Fhome.jsp" name="returnTo">

//                              <div style="display: none; padding: 3px 0px 0 0 ; text-align:right" class="capsWarning small red">Caps Lock включён</div>

//                              <div class="rememberMeBlock f12 paddingT10 floatLeft">
//                                <input type="checkbox" class="absmid" checked="checked" id="remember" name="remember">
//                                <input type="hidden" name="_remember" value="on">
//                                <label class="absmid" for="remember">Запомнить меня</label>
//                              </div>

//                              <div class="paddingT10 floatRight">
//                                <input type="submit" style="width: 100%;" data-element="h11" class="submit-light round4 gaClick" value="Вход">
//                              </div>

//                              <div class="clear"></div>

//                              <div class="socialLogin f11">
//                                <div class="ii lucida">
//                                  <a data-element="h10.3" class="gaClick remindPassLink marginT5" href="/remindPassword.jsp">Забыли пароль?</a>

//                                  <div class="socialButtons">
//                                    <div class="floatLeft langCorrection">
//                                      Войти с помощью :
//                                    </div>
//                                    <div class="floatRight">
//                                      <a data-element="h12" class="tw floatLeft sprite1 gaClick" href="http://www.4shared.com/servlet/signin/twitter?fp=http%3A%2F%2Fwww.4shared.com%2Faccount%2Fhome.jsp"></a>
//                                      <a data-element="h13" class="fb floatLeft sprite1 gaClick" href="http://www.4shared.com/servlet/signin/facebook?fp=http%3A%2F%2Fwww.4shared.com%2Faccount%2Fhome.jsp"></a>
//                                      <a data-element="h14" class="go floatLeft sprite1 gaClick" href="http://www.4shared.com/servlet/signin/google?fp=http%3A%2F%2Fwww.4shared.com%2Faccount%2Fhome.jsp"></a>
//                                      </div>
//                                    <div class="clear"></div>
//                                  </div>
//                                </div>
//                              </div>

//                            </div>
//                          </div>
//                        </form>

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
                                QString js = img -> value(QStringLiteral("onclick"));
                                int url_index = js.indexOf(QStringLiteral("'http")) + 1;

                                if (url_index != -1) {
                                    int end_url_index = js.indexOf(QChar('\''), url_index);
                                    track_obj.insert(tkn_grab_url, js.mid(url_index, end_url_index - url_index));
                                }


//                                track_obj.insert(tkn_grab_album, album_and_year.first());
//                                track_obj.insert(tkn_grab_year, album_and_year.last());
                                track_obj.insert(tkn_grab_refresh, name_tag -> link());
//                                track_obj.insert(tkn_skip_info, true);
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
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return true; } // stub
            };
        }
    }
}

#endif // FOURSHARED_API
