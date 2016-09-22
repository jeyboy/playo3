#ifndef YANDEX_MISC_H
#define YANDEX_MISC_H

#include <qstring.h>
#include <qcryptographichash.h>

#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/iuser_interaction.h"
#include "yandex_keys.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Misc : public virtual ISource, public virtual IQueriable, public virtual IUserInteraction {
                protected:
                    inline Misc() {}

                    // url_api_v1 % account-status.jsx?lang=ru&external-domain=music.yandex.ua&overembed=false // return keys: success, reason
                    // url_api_v2 % auth?external-domain=music.yandex.ua&overembed=no&__t=1451819854469 // returns csrf, device_id, logged, premium, invalid
//                    bool sessionIsValid() { return false; }

                    Headers headers() { return {{ LSTR("X-Retpath-Y"), encodeStr(baseUrlStr(qst_site_base))}}; }

                    // curl 'https://music.yandex.ua/api/v2.1/handlers/auth?external-domain=music.yandex.ua&overembed=no&__t=1474529009008' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Cache-Control: no-cache' -H 'Connection: keep-alive' -H 'Cookie: yandexuid=2190563221452621883; L=WnEJRFAPV0xrQVpRcXN0SEdPV0t6bmtBXl0rEDg8e3p/cg==.1474342480.12659.397393.538aa840261797a55723bf7670f887c4; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Ffeed%22%7D; yabs-vdrf=TR-DN203_mrS1pDnNIW57Yqy0pDnNEmOrC0y1pDnNWG5dJ5C1pDnNDm3LUrK1WDnN5GIw4Am0VjnNDm1Aa5G1hTbN201Z8L819ifNtWF8wa009ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; Session_id=noauth:1474430260; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Pragma: no-cache' -H 'Referer: https://music.yandex.ua/genre/electronic' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fgenre%2Felectronic'
                    QString sign() {
                        QJsonObject response = Manager::prepare() -> jsonGet(
                            QString(url_site_v2_1.arg(siteLocale(val_default_locale)) % LSTR("auth")),
                            headers()
                        );
                        return JSON_CSTR(response, LSTR("csrf"));
                    }

                    QString calcKey(const QString & path, const QString & s) {
                        QString str = path.mid(1) + s;
    //                    str.replace(QStringLiteral("\r\n"), QStringLiteral("\n"))

                        QCryptographicHash hash(QCryptographicHash::Md5);
                        hash.addData((val_key_set % str).toLatin1());
                        return QString(hash.result().toHex().toLower());
                    }
            };
        }
    }
}

#endif // YANDEX_MISC_H
