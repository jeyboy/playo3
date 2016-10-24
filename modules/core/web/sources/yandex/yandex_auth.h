#ifndef YANDEX_AUTH_H
#define YANDEX_AUTH_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Auth : public virtual Base {
            public:
                // curl 'https://mc.yandex.ru/sync_cookie_get?' -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Cache-Control: no-cache' -H 'Connection: keep-alive' -H 'Cookie: yandexuid=2190563221452621883; fuid01=56955a2c3f1c53b0.5AYIr42dBsF0CtuBmG1JgrlntZoem9fw4qrxupdTQHaXczrlLlJj7lMNoZ0onbqCukZkBf_SZZ5ZWtq15bxptwvfEM9yoiNeo_-ulstn0vguuT_FpA41VE0mcRYXqF62; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; L=WnEJRFAPV0xrQVpRcXN0SEdPV0t6bmtBXl0rEDg8e3p/cg==.1474342480.12659.397393.538aa840261797a55723bf7670f887c4; _ym_uid=1473434567276000673; sync_ok_ua=ua; yabs-sid=227182441474546360; sync_ok_by=by' -H 'DNT: 1' -H 'Host: mc.yandex.ru' -H 'Origin: https://music.yandex.ua' -H 'Pragma: no-cache' -H 'Referer: https://music.yandex.ua/genre/electronic' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0'

                bool checkConnection(QString & user_id) {
                    QJsonObject response = Manager::prepare() -> jsonGet(
                        QString(url_site_v1.arg(siteLocale(val_default_locale)) % LSTR("auth.jsx"))
                    );
                    response = JSON_OBJ(response, LSTR("user"));

                    if (JSON_CSTR(response, tkn_uid) != LSTR("0")) {
                        user_id = JSON_CSTR(response, tkn_login);
                        return true;
                    } else return false;
                }

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

                        QUrl form_url = auth_form -> serializeFormToUrl(vals, Html::Tag::fsf_none, login_link_href);

                        //form also contains captcha field //.js-domik-captcha

                        Html::Document resp_doc = Manager::prepare() -> formFollowed(form_url) -> toHtml();

                        if (checkConnection(user_id))
                            return true;
                        else {
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
