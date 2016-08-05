#ifndef OD_TUNER
#define OD_TUNER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Tuner : public virtual Base {
            protected:
                QJsonValue tunersList(const QString & locale = QStringLiteral("ru")) {
                    return saRequest(
                        audioUrlStr(
                            path_audio_tuners,
                            {{ tkn_locale, locale }}
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_tuners
                    );
                }
            };
        }
    }
}

#endif // OD_TUNER
