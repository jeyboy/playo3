#ifndef YANDEX_AUTH_H
#define YANDEX_AUTH_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Auth : public virtual Base {
            public:
                // curl 'https://music.yandex.ua/handlers/auth.jsx?lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.8911682290463797' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://music.yandex.ua/' -H 'X-Retpath-Y: https://music.yandex.ua/' -H 'X-Requested-With: XMLHttpRequest' -H 'Cookie: Session_id=3:1474023377.5.0.1474023377854:F3CJsg:16.0|363617853.0.2|4:118655.726606.c8Y2XC5XsZJwkOLjRCP6VQofygY; yandexuid=2190563221452621883; L=RFwFBQVmBgVyWVRkYU16VwFUVUNRenN4IyQzOyoWU19QRA==.1474023377.12651.37347.0faa78387be102abce08ff7197b92a62; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1#1789383377.udn.cDpqZXlib3kxOTg1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=OhTbN201Z8L819ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; ys=udn.cDpqZXlib3kxOTg1; _ym_isad=1; _ym_visorc_1028356=b; sessionid2=3:1474023377.5.0.1474023377854:F3CJsg:16.1|363617853.0.2|4:118655.305085.0npC_lcOUN-MsuMxUPyo_YPp5QQ; yandex_login=jeyboy1985' -H 'Connection: keep-alive'

                bool siteConnection(QString & user_id, QString & err) {
                    QHash<QString, QString> vals;
                    Response * response = Manager::prepare() -> getFollowed(baseUrlStr(qst_site_base));
                    setSiteLocale(response -> toUrl(false).host().section('.', -1));
                    Html::Document doc = response -> toHtml();
                    Html::Tag * login_link = doc.findFirst(".log-in");

                    if (!login_link) {
                        qCritical() << name() << "Auth link is not found";
                        return false;
                    }

                    QString login_link_href = login_link -> link().replace(LSTR("undefined"), baseUrlStr(qst_site_base).section('/', 2, 2));
                    //INFO: patch locale for auth link / why its always eq to ru ?
                    login_link_href.replace('.' % val_default_locale % '/', '.' % siteLocale() % '/');

                    Html::Document login_doc = Manager::prepare() -> getFollowed(login_link_href) -> toHtml();
                    Html::Tag * auth_form = login_doc.findFirst("form");
                    if (!auth_form) {
                        qCritical() << name() << "Auth form is not found";
                        return false;
                    }

                    while(true) {
                        if (!showingLogin(name() % val_login_title_postfix, vals[LSTR("login")], vals[LSTR("passwd")], err)) return false;

                        QUrl form_url = auth_form -> serializeFormToUrl(vals, false, login_link_href);

                        //form also contains captcha field //.js-domik-captcha

                        Html::Document resp_doc = Manager::prepare() -> formFollowed(form_url) -> toHtml();
                        QJsonObject response = Manager::prepare() -> jsonGet(baseUrlStr(qst_site, LSTR("auth.jsx"), {}));
                        response = JSON_OBJ(response, LSTR("user"));

                        if (JSON_CSTR(response, tkn_uid) != LSTR("0")) {
                            user_id = JSON_CSTR(response, tkn_login);
                            return true;
                        } else {
                            Html::Tag * err_tag = resp_doc.findFirst(".js-messages");
                            if (err_tag)
                                err = err_tag -> text();
                        }
                    }

                    return false;
                }
            };
        }
    }
}

#endif // YANDEX_AUTH_H
