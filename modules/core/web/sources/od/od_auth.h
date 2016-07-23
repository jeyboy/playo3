#ifndef OD_AUTH
#define OD_AUTH

#include "od_defines.h"
#include "od_misc.h"
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

                bool siteConnection(QString & user_id, QString & hash, QString & err) {
                    QString authE, authP;

                    while(true) {
                        if (!showingLogin(val_login_title, authE, authP, err)) return false;

                        Response * reply = Manager::prepare() -> form(authRequestUrl(authE, authP), initHeaders());
                        QUrl url = reply -> toRedirectUrl();
                        QString hash_key = Manager::paramVal(url, tkn_httpsdata); // not used anywhere at this moment

                        reply = Manager::prepare() -> getFollowed(url, initHeaders());
                        err = reply -> paramVal(tkn_form_error);
                        if (!err.isEmpty()) {
                            reply -> deleteLater();
                            continue;
                        }

                        Html::Document doc = reply -> toHtml();
                        checkSecurity(doc);

                        if (!Manager::cookie(tkn_authcode).isEmpty()) {
                            setSiteUserID(grabUserId(doc));
                            setSiteHash(hash_key);
                            return true;
                        }
                        else err = doc.find(".anonym_e").text();
                    }

                    return false;
                }
            };
        }
    }
}

#endif // OD_AUTH
