#ifndef YANDEX_AUTH_H
#define YANDEX_AUTH_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Auth : public Base {
            public:
                bool siteConnection(QString & /*user_id*/, QString & /*err*/) {
//                    QString authE, authP;

//                    while(true) {
//                        if (!showingLogin(val_login_title, authE, authP, err)) return false;

//                        Response * reply = Manager::prepare() -> form(authRequestUrl(authE, authP), initHeaders());
//                        QUrl url = reply -> toRedirectUrl();
////                        hash = Manager::paramVal(url, tkn_httpsdata); // not used anywhere at this moment

//                        reply = Manager::prepare() -> getFollowed(url, initHeaders());
//                        err = reply -> paramVal(tkn_form_error);
//                        if (!err.isEmpty()) {
//                            reply -> deleteLater();
//                            continue;
//                        }

//                        Html::Document doc = reply -> toHtml();
//                        checkSecurity(doc);

//                        if (!Manager::cookie(tkn_authcode).isEmpty()) {
//                            user_id = grabUserId(doc);
////                            setSiteHash(hash_key);
//                            return true;
//                        }
//                        else err = doc.find(".anonym_e").text();
//                    }

                    return false;
                }
            };
        }
    }
}

#endif // YANDEX_AUTH_H
