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

                bool connectApi(QString & new_token, QString & user_id, qint64 & expiration, QString & error) {
                    QUrl form_url = authUrl();

                    while(true) {
                        Response * resp = Manager::prepare() -> getFollowed(form_url);
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
                                captcha_src = captcha_tag -> src();

                            if (captcha_src.isEmpty()) {
                                if (!showingLogin(name() % val_login_title_postfix, vals[tkn_email], vals[tkn_password], error))
                                    return false;
                            } else {
                                if (!showingLoginWithCaptcha(name() % val_login_title_postfix, captcha_src,
                                    vals[tkn_email], vals[tkn_password], vals[tkn_captcha], error
                                )) return false;
                            }

                            error = QString();
                            QByteArray payload;

                            form -> serializeForm(form_url, payload, vals);
                            resp = Manager::prepare() -> formFollowed(form_url, payload);
                        } else return false; // something went wrong

                        form_url = resp -> toUrl(false);
                        QUrlQuery query(form_url.fragment());

                        if (query.hasQueryItem(tkn_error)) {
                            error = query.queryItemValue(tkn_error_description);
                            return false;
                        } else if (query.hasQueryItem(tkn_access_token)) {
                            new_token = query.queryItemValue(tkn_access_token);
                            user_id = query.queryItemValue(tkn_user_id);
                            QString exp_str = query.queryItemValue(tkn_expires_in);
                            expiration = exp_str.toInt();
                            resp -> deleteLater();

                            return true;
                        } else {
                            QString confirm_url;
                            QRegularExpression ruga(LSTR("location.href = \"([^\"]+)\""));
                            QRegularExpressionMatchIterator i = ruga.globalMatch(resp -> toText());

                            while (i.hasNext()) {
                                QRegularExpressionMatch match = i.next();
                                if (match.hasMatch()) {
                                    QString text = match.captured(1);

                                    if (text.indexOf(LSTR("act=grant_access")) > -1 && text.indexOf(LSTR("cancel=1")) == -1) {
                                        confirm_url = text;
                                        break;
                                    }
                                }
                            }

                            if (confirm_url.isEmpty()) {
                                form_url = authUrl();
                                error = LSTR("Some shit happened... :(");
                                continue;
                            } else {
                                //https://login.vk.com/?act=grant_access&client_id=4332211&settings=327706&redirect_uri=https%3A%2F%2Foauth.vk.com%2Fblank.html&response_type=token&direct_hash=3744c1197ad0359075&token_type=0&v=5.52&state=&display=page&ip_h=954e1677fa11b49ae8&hash=a5fa4acec0d43cc6f4&https=1
                                resp = Manager::prepare() -> getFollowed(confirm_url);
                                form_url = resp -> toUrl(false);
                                query = QUrlQuery(form_url.fragment());
                            }
                        }

                        new_token = query.queryItemValue(tkn_access_token);
                        user_id = query.queryItemValue(tkn_user_id);
                        QString exp_str = query.queryItemValue(tkn_expires_in);
                        expiration = exp_str.toInt();
                        resp -> deleteLater();
                        return true;
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
