#ifndef OD_API_KEYS
#define OD_API_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Od {
            struct Keys {
                const QString base_url          = QStringLiteral("http://ok.ru/");
                const QString base_auth_url     = QStringLiteral("https://www.ok.ru/");
                const QString base_audio_url    = QStringLiteral("http://wmf1.odnoklassniki.ru/");

                const QString offset_key        = QStringLiteral("start");
                const QString limit_key         = QStringLiteral("count");
            };
        }
    }
}

#endif // OD_API_KEYS
