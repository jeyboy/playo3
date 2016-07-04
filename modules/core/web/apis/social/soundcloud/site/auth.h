#ifndef SOUNDCLOUD_SITE_AUTH
#define SOUNDCLOUD_SITE_AUTH

#include "defines.h"
#include "modules/core/interfaces/iuser_interaction.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class Auth : public QueryBase, public IUserInteraction {
                protected:
                    bool connectUser() {
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
}

#endif // SOUNDCLOUD_SITE_AUTH
