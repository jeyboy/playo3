#ifndef YOUTUBE_AUTH_H
#define YOUTUBE_AUTH_H

#include "youtube_defines.h"
#include "modules/core/interfaces/iuser_interaction.h"
//#include "modules/core/web/services/recaptcha.h"

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
                    setParam(query, tkn_response_type, LSTR("code"));
                    setParam(query, tkn_scope, LSTR("https://www.googleapis.com/auth/youtube https://www.googleapis.com/auth/youtube.readonly")); // https://www.googleapis.com/auth/youtube.upload
                    setParam(query, LSTR("access_type"), LSTR("offline"));

                    url.setQuery(query);
                    return url.toString();
                }

                QString tokenUrl(const QString & code) {
                    QUrl url(url_token);

                    QUrlQuery query = QUrlQuery();
                    setParam(query, tkn_redirect_uri, val_redirect_url);
                    setParam(query, LSTR("code"), code);
                    setParam(query, LSTR("client_id"), val_tkn);
                    setParam(query, LSTR("client_secret"), val_sec_tkn);
                    setParam(query, LSTR("grant_type"), LSTR("authorization_code"));

                    url.setQuery(query);
                    return url.toString();
                }

                bool connectApi(QString & new_token, QString & user_id, QString & expiration, QString & error) {
                    QUrl form_url = authUrl();

                    while(true) {
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
                                captcha_src = captcha_tag -> value("src");

                            if (captcha_src.isEmpty()) {
                                if (!showingLogin(name() % val_login_title_postfix, vals[tkn_auth_email], vals[tkn_auth_pass], error))
                                    return false;
                            } else {
                                if (!showingLoginWithCaptcha(name() % val_login_title_postfix, captcha_src,
                                    vals[tkn_auth_email], vals[tkn_auth_pass], vals[tkn_auth_captcha], error
                                )) return false;
                            }

                            error = QString();
                            form_url = form -> serializeFormToUrl(vals);
                            form_url.setPath(form_url.path() % LSTR("Xhr"));
                            resp = Manager::prepare() -> formFollowed(form_url);
                        } else {
                            error = LSTR("Login form did not found");
                            return false; // something went wrong
                        }

                        QJsonObject json = resp -> toJson();

                        if (JSON_HAS_KEY(json, tkn_encoded_profile_info)) {
                            form_url.setPath(LSTR("/signin/challenge/sl/password"));
                            QUrlQuery query = QUrlQuery(form_url.query());
                            query.removeQueryItem(tkn_profile_info);
                            query.addQueryItem(tkn_profile_info, JSON_STR(json, tkn_encoded_profile_info));
                            form_url.setQuery(query);

                            qDebug() << form_url;
                            resp = Manager::prepare() -> formFollowed(form_url);
                        } else {
                            error = LSTR("Validation of email failed");
                            return false;
                        }

                        Html::Document choose_html = resp -> toHtml();
                        Html::Tag * form = choose_html.findFirst("form");

                        if (!form) {
                            error = LSTR("Submit form did not found");
                            Logger::obj().write(name(), error, Logger::log_error);
                            return false;
                        }

                        error = QString();
                        //INFO: this request very sensitive to params and payload parts separation
                        QByteArray payload;
                        form -> serializeForm(form_url, payload, QHash<QString, QString> {{LSTR("submit_access"), LSTR("true")}, {LSTR("bgresponse"), LSTR("js_disabled")}});
                        form_url = Manager::prepare() -> form(form_url, payload) -> toRedirectUrl();

                        QUrlQuery code_query = QUrlQuery(form_url.query());
                        QString code = code_query.queryItemValue(LSTR("code"));

                        if (code.isEmpty()) {
                            error = LSTR("Some error occured while receiving token");
                            return false;
                        }

                        QJsonObject obj = Manager::prepare() -> jsonPost(tokenUrl(code));

                        int i = 0;
//                        QUrlQuery query(form_url.fragment());

//                        if (query.hasQueryItem(tkn_error)) {
//                            error = query.queryItemValue(tkn_error_description);
//                            return false;
//                        } else if (query.hasQueryItem(tkn_access_token)) {
//                            new_token = query.queryItemValue(tkn_access_token);
//                            user_id = query.queryItemValue(tkn_user_id);
//                            expiration = query.queryItemValue(tkn_expires_in);

//                            return true;
//                        }
//                        else {
//                            form_url = authUrl();
//                            error = LSTR("Some shit happened... :(");
//                        }
                    }
                }
            };
        }
    }
}

#endif // YOUTUBE_AUTH_H
