#ifndef YANDEX_AUTH_H
#define YANDEX_AUTH_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Auth : public virtual Base {
            public:
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

                        QUrl form_url = auth_form -> serializeFormToUrl(vals, false, login_link_href);

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
