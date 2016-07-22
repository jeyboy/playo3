#ifndef OD_AUTH
#define OD_AUTH

#include "od_defines.h"
#include "od_api_misc.h"
#include "modules/core/interfaces/iuser_interaction.h"
//#include "modules/core/web/services/recaptcha.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Auth : public Base, public IUserInteraction, public Misc {
            protected:
                bool sessionIsValid() { return !hasError(Manager::prepare() -> jsonGet(initAudioUrl())); }

                inline QUrl authRequestUrl(const QString & email, const QString & pass) const {
                    return QUrl(url_base_auth % path_auth.arg(encodeStr(email), encodeStr(pass)));
                }

                inline QUrl initUrl() const { return QUrl(url_base_auth % path_auth2 % siteHash()); }
            };
        }
    }
}

#endif // OD_AUTH
