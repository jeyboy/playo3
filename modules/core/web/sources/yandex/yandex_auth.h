#ifndef YANDEX_AUTH_H
#define YANDEX_AUTH_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Auth : public virtual Base {
            public:
//                curl 'https://music.yandex.ua/api/v2.1/handlers/auth?external-domain=music.yandex.ua&overembed=no&__t=1473537631012' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://music.yandex.ua/genre/easy' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fgenre%2Feasy' -H 'X-Requested-With: XMLHttpRequest' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=N9ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1; _ym_visorc_1028356=b' -H 'Connection: keep-alive'

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
