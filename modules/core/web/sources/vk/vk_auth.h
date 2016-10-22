#ifndef VK_AUTH
#define VK_AUTH

#include "vk_defines.h"
#include "modules/core/interfaces/iuser_interaction.h"
#include "modules/core/web/services/recaptcha.h"

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

                bool connectApi(QString & new_token, QString & user_id, QString & expiration, QString & error) {
                    QUrl form_url = authUrl();

                    while(true) {
                        Response * resp = Manager::prepare() -> getFollowed(form_url);

                //        Logger::dump(resp -> toText().toUtf8());

                        Html::Document html = resp -> toHtml(false);

                        if (html.has("input[name='pass']")) { // if user not authorized
                            resp -> deleteLater();
                            QHash<QString, QString> vals;

                            if (error.isEmpty())
                                error = html.find(".service_msg_warning").text();

                            Html::Tag * form = html.findFirst("form");

                            if (!form) {
                                error = LSTR("Auth form did not found");
                                Logger::obj().write(name(), error, Logger::log_error);
                                return false;
                            }

                            QString captcha_src;
                            Html::Tag * captcha_tag = form -> findFirst("img#captcha");

                            if (captcha_tag)
                                captcha_src = captcha_tag -> value("src");

                            if (captcha_src.isEmpty()) {
                                if (!showingLogin(name() % val_login_title_postfix, vals[tkn_email], vals[tkn_password], error))
                                    return false;
                            } else {
                                if (!showingLoginWithCaptcha(name() % val_login_title_postfix, captcha_src,
                                    vals[tkn_email], vals[tkn_password], vals[tkn_captcha], error
                                )) return false;
                            }

                            error = QString();
                            form_url = form -> serializeFormToUrl(vals);
                            resp = Manager::prepare() -> formFollowed(form_url);
                        } else return false; // something went wrong

                        form_url = resp -> toUrl();
                        QUrlQuery query(form_url.fragment());

                        if (query.hasQueryItem(tkn_error)) {
                            error = query.queryItemValue(tkn_error_description);
                            return false;
                        } else if (query.hasQueryItem(tkn_access_token)) {
                            new_token = query.queryItemValue(tkn_access_token);
                            user_id = query.queryItemValue(tkn_user_id);
                            expiration = query.queryItemValue(tkn_expires_in);

                            return true;
                        }
                        else {
                            form_url = authUrl();
                            error = LSTR("Some shit happened... :(");
                        }
                    }
                }

                bool connectSite(QString & /*error*/) { return true; } // site automatically authenticated with api
                bool captchaProcessing(QJsonObject & json, QString & url_str) {
                    QJsonObject stat_obj = json.value(tkn_error).toObject();

                    QString captchaText;
                    showingCaptcha(QUrl(stat_obj.value(tkn_captcha_img).toString()), captchaText);
                    if (captchaText.isEmpty())
                        return false;

                    QUrl url(url_str);
                    QUrlQuery query(url.query());
                    query.removeQueryItem(tkn_captcha_sid);
                    query.removeQueryItem(tkn_captcha);

                    query.addQueryItem(tkn_captcha_sid, stat_obj.value(tkn_captcha_sid).toString());
                    query.addQueryItem(tkn_captcha, captchaText);

                    url.setQuery(query);

                    return sRequest(url.toString(), json, call_type_json);
                }
            };
        }
    }
}

#endif // VK_AUTH
