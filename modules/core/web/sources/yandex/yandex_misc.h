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
