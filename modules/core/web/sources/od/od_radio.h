#ifndef OD_RADIO
#define OD_RADIO

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Radio : public Base {
            protected:
                QJsonValue tunersList(const QString & locale = QStringLiteral("ru"), int count = 25, int offset = 0) { //TODO: need to check
                    return pRequest(
                        audioUrlStr(
                            path_audio_tuners_for_radio,
                            {
                                { tkn_pid, playlist_id },
                                { QStringLiteral("locale"), locale }
                            }
                        ),
                        call_type_json, rules(offset, count),
                        0, proc_json_extract, QStringList() << tkn_tracks
                    );
                }

                QJsonValue radio(const QString & tuner_id, const QString & locale = QStringLiteral("ru")) { //TODO: need to check
                    QJsonObject obj = sRequest(
                                audioUrlStr(
                                    path_audio_radio,
                                    {
                                        { QStringLiteral("tuner"), tuner_id },
                                        { QStringLiteral("locale"), locale }
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
