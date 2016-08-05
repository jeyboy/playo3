#ifndef OD_RADIO
#define OD_RADIO

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Radio : public virtual Base {
            protected:
                QJsonValue tunersList(const QString & locale = QStringLiteral("ru")) {
                    return saRequest(
                        audioUrlStr(
                            path_audio_tuners_for_radio,
                            {{ tkn_locale, locale }}
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_tuners
                    );
                }

                QJsonValue radio(const QString & tuner_id, const QString & locale = QStringLiteral("ru")) { //TODO: need to check
                    QJsonObject obj = sRequest(
                                audioUrlStr(
                                    path_audio_radio,
                                    {
                                        { tkn_tuner, tuner_id },
                                        { tkn_locale, locale }
                                    }
                                ),
                                call_type_json
                            );

                    return obj;
                }
            };
        }
    }
}

#endif // OD_RADIO
