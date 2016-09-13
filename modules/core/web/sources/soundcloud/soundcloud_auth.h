#ifndef SOUNDCLOUD_AUTH
#define SOUNDCLOUD_AUTH

#include "soundcloud_defines.h"
#include "modules/core/interfaces/iuser_interaction.h"
#include "modules/core/web/services/recaptcha.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Auth : public Base, public IUserInteraction {
            protected:
                QString authUrl() {
                    QUrl url(url_connect);

                    QUrlQuery query = genDefaultParams(qst_api);
                    setParam(query, tkn_response_type, val_response_type);
                    setParam(query, tkn_scope, val_scope);
                    setParam(query, tkn_redirect_uri, url_redirect);
                    setParam(query, tkn_display, val_display);

                    url.setQuery(query);
                    return url.toString();
                }

                /////////////////
                /// AUTH
                ////////////////

                //QString authTokenUrl() const {
                //    QUrl url("https://api.soundcloud.com/oauth2/token");
                //    QUrlQuery query = genDefaultParams();


                //    query.addQueryItem("client_secret", val_client_tkn);
                //    query.addQueryItem("scope", "non-expiring");

                ////    query.addQueryItem("grant_type", "password");
                ////    query.addQueryItem("username", "USERNAME");
                ////    query.addQueryItem("password", "PASSWORD");

                //    url.setQuery(query);
                //    return url.toString();
                //}
                inline QUrl authTokenUrl() const { return QUrl(url_auth_token); }
                inline QString confirmAuthUrl(const QString & access_token) { return url_auth_confirm % access_token; }

                QByteArray authTokenUrlParams(const QString & code) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_client_secret, val_client_tkn);
                    setParam(query, tkn_grant_type, val_grant_type);
                    setParam(query, tkn_redirect_uri, url_redirect);
                    setParam(query, tkn_code, code);

                    return query.toString(QUrl::FullyEncoded).toUtf8();
                }

                bool connectApi(QString & newToken, QString & userID, QString & error) {
                    QUrl auth_url = authUrl();
                    QUrl form_url = auth_url;
                    Response * resp = Manager::prepare() -> getFollowed(form_url);
                    QHash<QString, QString> vals;

                    while(true) {
                        QString err;
                        Html::Document html = resp -> toHtml();

                        Html::Tag * form = html.findFirst("form.authorize-token");

                        if (!form) {
                            Logger::obj().write(name(), QStringLiteral("Auth form did not found"), true);
                            return false;
                        }

                        if (form -> has("input[name='password']")) { // if user not authorized
                            err = html.find(".warning").text();

                            QString captcha_src;
                            Html::Set captcha_set = form -> find(QString("script[src^'" + RECAPTCHA_BASE_URL + "']").toUtf8().data());

                            if (!captcha_set.isEmpty())
                                captcha_src = captcha_set.first() -> value(tkn_src);

                            if (captcha_src.isEmpty()) {
                                if (!showingLogin(val_auth_title, vals[tkn_username], vals[tkn_password], err))
                                    return false;
                            } else {
                                if (!showingLoginWithCaptcha(
                                    val_auth_title, Recaptcha::V1::obj().takeImageUrl(captcha_src, vals[tkn_recaptcha_challenge_field]),
                                    vals[tkn_username], vals[tkn_password], vals[tkn_recaptcha_response_field], err
                                )) return false;
                            }
                        }

                        form_url = form -> serializeFormToUrl(vals, true);

                        if (form_url.isRelative())
                            form_url = auth_url.resolved(form_url);

                        resp = Manager::prepare() -> formFollowed(form_url, {{tkn_referer, form_url.toString()}});

                        QUrlQuery query(resp -> toUrl(false).query());

                        if (query.hasQueryItem(tkn_error)) {
                            error = query.queryItemValue(tkn_error_description);
                            qDebug() << "ERRRRRRRRRR" << error;
                            resp -> deleteLater();
                            return false;
                        } else if (query.hasQueryItem(tkn_code)) {
                            QJsonObject doc = Web::Manager::prepare() -> formFollowed(authTokenUrl(), authTokenUrlParams(query.queryItemValue(tkn_code))) -> toJson();

                            if (doc.contains(tkn_access_token)) {
                                newToken = doc.value(tkn_access_token).toString();
                                setSiteAdditionalToken(newToken);

                                doc = Web::Manager::prepare() -> jsonGet(confirmAuthUrl(newToken));
                                userID = QString::number(doc.value(tkn_id).toInt());
                                resp -> deleteLater();
                                return true;
                            }
                        }
                    }
                }

                bool connectSite() {
                //    curl 'https://sign-in.soundcloud.com/sign-in/password?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1467192015' -X POST -H
                //    'DNT: 1' -H 'Host: sign-in.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com'

                //    payload:

                //        client_id
                //            "02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea"

                //        credentials
                //            Object { identifier="jeyboy1985@gmail.com",  password="5675675675"}

                //        identifier
                //            "jeyboy1985@gmail.com"

                //        password
                //            "5675675675"

                //        recaptcha_response
                //            ""

                //        scope
                //            "fast-connect non-expiring purchase signup upload"



                //    respond:

                //        {"session":{"access_token":"1-138878-99021496-..."},"sign_in":{"user_urn":"soundcloud:users:9902","method":"password"}}

                    return false;
                }

//                bool initOAuthToken() {
//                    QJsonObject obj = Manager::prepare() -> jsonPost(
//                        baseUrlStr(
//                            qst_site, QStringLiteral("connect/token"),
//                            {{LSTR("scope"), LSTR("non-expiring%20fast-connect%20purchase%20upload")}}
//                        )
//                    );

//                    bool res = JSON_HAS_KEY(obj, LSTR("access_token"));

//                    if (res)
//                        setSiteAdditionalToken(JSON_CSTR(obj, LSTR("access_token")));

//                    return res;
//                }
            };
        }
    }
}

#endif // SOUNDCLOUD_AUTH
