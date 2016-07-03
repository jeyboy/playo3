#ifndef SOUNDCLOUD_AUTH
#define SOUNDCLOUD_AUTH

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
//            class SiteAuth : SiteBase {
//            protected:
//            };

            class ApiAuth : ApiBase {
            protected:
                QString authUrl() {
                    QUrl url(url_connect);

                    QUrlQuery query = genDefaultParams();
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
            };
        }
    }
}

#endif // SOUNDCLOUD_AUTH
