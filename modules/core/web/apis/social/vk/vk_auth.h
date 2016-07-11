#ifndef VK_AUTH
#define VK_AUTH

#include "vk_defines.h"
#include "modules/core/interfaces/iuser_interaction.h"
#include "modules/core/web/apis/service/recaptcha.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Auth : public Base, public IUserInteraction {
            protected:
                QString authUrl() {
                    QUrl url(url_auth);

                    QUrlQuery query = QUrlQuery();
                    setParam(query, tkn_version, val_version);
                    setParam(query, tkn_display, val_page_token);
                    setParam(query, tkn_client_id, val_client_token);
                    setParam(query, tkn_response_type, val_response_token);
                    setParam(query, tkn_scope, val_scope);
                    setParam(query, tkn_redirect_uri, url_redirect_uri);

                    url.setQuery(query);
                    return url.toString();
                }


                bool connectApi(QString & newToken, QString & userID, QString & expiration, QString & error) {
                    QUrl form_url = authUrl();

                    while(true) {
                        Response * resp = Manager::prepare() -> getFollowed(form_url);
                        QString err;

                //        Logger::dump(resp -> toText().toUtf8());

                        Html::Document html = resp -> toHtml(false);

                        if (html.has("input[name='pass']")) { // if user not authorized
                            resp -> deleteLater();
                            QHash<QString, QString> vals;
                            err = html.find(".service_msg_warning").text();

                            Html::Tag * form = html.findFirst("form");

                            if (!form) {
                                Logger::obj().write(val_auth_title, QStringLiteral("Auth form did not found"), true);
                                return false;
                            }

                            QString captcha_src;
                            Html::Set captcha_set = form -> find("img#captcha");

                            if (!captcha_set.isEmpty())
                                captcha_src = captcha_set.first() -> value("src");

                            if (captcha_src.isEmpty()) {
                                if (!showingLogin(val_auth_title, vals[tkn_email], vals[tkn_password], err))
                                    return false;
                            } else {
                                if (!showingLoginWithCaptcha(val_auth_title, captcha_src,
                                    vals[tkn_email], vals[tkn_password], vals[tkn_captcha], err
                                )) return false;
                            }

                            form_url = form -> serializeFormToUrl(vals);
                            qDebug() << form_url;
                            resp = Manager::prepare() -> formFollowed(form_url);
                            html.output();
                        } else return false; // something went wrong

                        form_url = resp -> toUrl();
                        QUrlQuery query(form_url.fragment());

                        if (query.hasQueryItem(tkn_error)) {
                            error = query.queryItemValue(tkn_error_description);
                            return false;
                        } else if (query.hasQueryItem(tkn_access_token)) {
                            newToken = query.queryItemValue(tkn_access_token);
                            userID = query.queryItemValue(tkn_user_id);
                            expiration = query.queryItemValue(tkn_expires_in);

                            return true;
                        }
                        else form_url = authUrl();
                    }
                }

                bool connectSite() {
                    return false;
                }
            };
        }
    }
}

#endif // VK_AUTH
