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

                    // curl 'https://music.yandex.ua/api/v2.1/handlers/auth?external-domain=music.yandex.ua&overembed=no&__t=1474529009008' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Cache-Control: no-cache' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Pragma: no-cache' -H 'Referer: https://music.yandex.ua/genre/electronic' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fgenre%2Felectronic'
                    QString sign() {
                        QJsonObject response = Manager::prepare() -> jsonGet(
                            QString(url_site_v2_1.arg(siteLocale(const_default_locale)) % LSTR("auth")),
                            headers()
                        );
                        return JSON_CSTR(response, LSTR("csrf"));
                    }

                    QString calcKey(const QString & path, const QString & s) {
                        QString str = path.mid(1) + s;
    //                    str.replace(LSTR("\r\n"), LSTR("\n"))

                        QCryptographicHash hash(QCryptographicHash::Md5);
                        hash.addData((val_key_set % str).toLatin1());
                        return QString(hash.result().toHex().toLower());
                    }
            };
        }
    }
}

#endif // YANDEX_MISC_H
