#ifndef YOUTUBE_AUTH_H
#define YOUTUBE_AUTH_H

#include "youtube_defines.h"
#include "modules/core/interfaces/iuser_interaction.h"
//#include "modules/core/web/services/recaptcha.h"

#define CALC_EXPIRE(exp) QDateTime::currentMSecsSinceEpoch() / 1000 + exp

namespace Core {
    namespace Web {
        namespace Youtube {
            class Auth : public virtual Base, public IUserInteraction {
            public:
                QString authUrl() {
                    QUrl url(url_auth);

                    QUrlQuery query = QUrlQuery();
                    setParam(query, tkn_client_id, val_tkn);
                    setParam(query, tkn_redirect_uri, val_redirect_url);
                    setParam(query, tkn_response_type, tkn_code);
                    setParam(query, tkn_scope, LSTR("https://www.googleapis.com/auth/youtube https://www.googleapis.com/auth/youtube.readonly https://www.googleapis.com/auth/userinfo.profile")); // https://www.googleapis.com/auth/youtube.upload
                    setParam(query, LSTR("access_type"), LSTR("offline"));
                    setParam(query, LSTR("approval_prompt"), LSTR("force"));

                    url.setQuery(query);
                    return url.toString();
                }

                QString tokenUrl(const QString & code, bool auth = true) {
                    QUrl url(url_token);

                    QUrlQuery query = QUrlQuery();
                    setParam(query, tkn_redirect_uri, val_redirect_url);
                    setParam(query, auth ? tkn_code : tkn_refresh_token, code);
                    setParam(query, LSTR("client_id"), val_tkn);
                    setParam(query, LSTR("client_secret"), val_sec_tkn);
                    setParam(query, LSTR("grant_type"), auth ? LSTR("authorization_code") : tkn_refresh_token);

                    url.setQuery(query);
                    return url.toString();
                }

                bool refreshToken(const QString & refresh_token, QString & token, qint64 & expire) {
                    QJsonObject obj = Manager::prepare() -> jsonPost(tokenUrl(refresh_token, false));
                    if (JSON_HAS_KEY(obj, tkn_access_token)) {
                        token = JSON_STR(obj, tkn_access_token);
                        expire = CALC_EXPIRE(JSON_INT(obj, tkn_expires_in));
                        return true;
                    } else return false;
                }

                bool connectApi(QString & new_token, QString & refresh_token, qint64 & expire, QString & error) {
                    QUrl form_url;
                    QByteArray payload;

                    while(true) { // add behaviour line for wrong wmail or password
                        form_url = authUrl();
                        Response * resp = Manager::prepare() -> getFollowed(form_url);
                        Html::Document html = resp -> toHtml();

                        if (html.has("input[name='Email']")) { // if user not authorized
                            QHash<QString, QString> vals = QHash<QString, QString>{{LSTR("bgresponse"), LSTR("js_disabled")}};

                            if (error.isEmpty())
                                error = html.find(".error-msg").text();

                            Html::Tag * form = html.findFirst("form");

                            if (!form) {
                                error = LSTR("Auth form did not found");
                                Logger::obj().write(name(), error, Logger::log_error);
                                return false;
                            }

                            // div ||| [(style : display: none)(id : identifier-captcha)]
                            QString captcha_src;
                            Html::Tag * captcha_tag = form -> findFirst(".captcha-box img");

                            if (captcha_tag)
                                captcha_src = captcha_tag -> src();

                            if (captcha_src.isEmpty()) {
                                if (!showingLogin(name() % val_login_title_postfix, vals[tkn_auth_email], vals[tkn_auth_pass], error))
                                    return false;
                            } else {
                                if (!showingLoginWithCaptcha(name() % val_login_title_postfix, captcha_src,
                                    vals[tkn_auth_email], vals[tkn_auth_pass], vals[tkn_auth_captcha], error
                                )) return false;
                            }

                            form -> serializeForm(form_url, payload, vals, Html::Tag::fsf_percent_encoding);
                            form_url.setPath(form_url.path() % LSTR("Xhr"));
                            resp = Manager::prepare() -> formFollowed(form_url, payload);
                        } else {
                            error = LSTR("Login form did not found");
                            return false;
                        }

                        QJsonObject json = resp -> toJson();

                        if (JSON_HAS_KEY(json, tkn_encoded_profile_info)) {
                            form_url.setPath(LSTR("/signin/challenge/sl/password"));
                            QUrlQuery query = QUrlQuery(form_url.query());
                            query.removeQueryItem(tkn_profile_info);
                            query.addQueryItem(tkn_profile_info, JSON_STR(json, tkn_encoded_profile_info));
                            form_url.setQuery(query);

                            resp = Manager::prepare() -> formFollowed(form_url, payload);
                        } else {
                            error = LSTR("Validation of email failed");
                            return false;
                        }

                        {
                            Html::Document choose_html = resp -> toHtml();
                            if (choose_html.findFirst("#gaia_loginform")) {
                                error = LSTR("Login or password is incorrect");
                                continue;
                            }

                            Html::Tag * form = choose_html.findFirst("form");

                            if (!form) {
                                error = LSTR("Submit form did not found");
                                Logger::obj().write(name(), error, Logger::log_error);
                                return false;
                            }

                            error = QString();
                            //INFO: this request very sensitive to params and payload parts separation
                            QByteArray payload;
                            form -> serializeForm(form_url, payload, QHash<QString, QString> {{LSTR("submit_access"), const_true}, {LSTR("bgresponse"), LSTR("js_disabled")}}, Html::Tag::fsf_percent_encoding);
                            form_url = Manager::prepare() -> form(form_url, payload) -> toRedirectUrl();
                        }

                        QUrlQuery code_query = QUrlQuery(form_url.query());
                        QString code = code_query.queryItemValue(tkn_code);

                        if (code.isEmpty()) {
                            error = LSTR("Some error occured while receiving token");
                            return false;
                        }

                        QJsonObject obj = Manager::prepare() -> jsonPost(tokenUrl(code));

                        if (JSON_HAS_KEY(obj, tkn_access_token)) {
                            new_token = JSON_STR(obj, tkn_access_token);
                            expire = CALC_EXPIRE(JSON_INT(obj, tkn_expires_in));
                            refresh_token = JSON_STR(obj, tkn_refresh_token);
                            return true;
                        }
                    }
                }
            };
        }
    }
}

#endif // YOUTUBE_AUTH_H
