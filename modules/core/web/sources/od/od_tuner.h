#ifndef OD_TUNER
#define OD_TUNER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Tuner : public virtual Base {
            protected:
                QJsonValue tunersList(const QString & locale = QStringLiteral("ru")) {
                    QJsonArray tuners = saRequest(
                        audioUrlStr(
                            path_audio_tuners,
                            {{ tkn_locale, locale }}
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_tuners
                    );

                    QJsonArray block_content;
                    for(QJsonArray::Iterator tuner = tuners.begin(); tuner != tuners.end(); tuner++) {
                        QJsonObject tuner_obj = (*tuner).toObject();

                        QString name = tuner_obj.value(tkn_name).toString();
                        QString artists_str;

                        QJsonArray artists = tuner_obj.value(tkn_artists).toArray();
                        for(QJsonArray::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                            QJsonObject artist_obj = (*artist).toObject();

                            artists_str = artists_str % ',' % ' ' %  artist_obj.value(tkn_name).toString();
                        }

                        name = QStringLiteral("%1 (%2)").arg(name, artists_str.mid(2));
                        int source_id = sourceType();
                        QString uid = idToStr(tuner_obj.value(QStringLiteral("data")));

                        block_content << QJsonObject {
                            {tkn_id, uid },
                            {tkn_name, name},
                            {tkn_loadable_cmd,
                                 Cmd::build(
                                    source_id, cmd_mtd_tracks_by_tuner,
                                    {{CMD_ID, uid}}
                                 ).toString()
                            }
                        };
                    }

                    return prepareBlock(dmt_audio_set, block_content);
                }
            };
        }
    }
}

#endif // OD_TUNER
