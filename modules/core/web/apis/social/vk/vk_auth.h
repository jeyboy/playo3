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

                bool connectApi(QString & newToken, QString & userID, QString & error) {

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
            };
        }
    }
}

#endif // VK_AUTH
