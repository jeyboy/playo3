#ifndef OD_TUNER
#define OD_TUNER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Tuner : public virtual Base {
            protected:
                QJsonValue tunersList(const QString & locale = QStringLiteral("ru")) {
                    QJsonArray block_content = saRequest(
                        audioUrlStr(
                            path_audio_tuners,
                            {{ tkn_locale, locale }}
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_tuners
                    );

                    return prepareBlock(dmt_audio_set, block_content);
                }
            };
        }
    }
}

#endif // OD_TUNER
