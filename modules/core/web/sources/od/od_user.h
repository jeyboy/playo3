#ifndef OD_USER
#define OD_USER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class User : public Base {
            public:
                QJsonValue userInfo(const QString & user_id = QString()) {
                    if (user_id.isEmpty())
                        return Manager::prepare() -> jsonGet(audioUrlStr(path_audio_init));
                    else
                        return Manager::prepare() -> jsonGet(audioUrlStr(tkn_my, {{tkn_uid, user_id}}));
                }

//                QJsonValue popular(const SearchLimit & /*limits*/) {
//                    return pRequest(
//                        popAudioUrl(),
//                        call_type_json,
//                        rules(),
//                        0,
//                        proc_json_extract,
//                        QStringList() << tkn_tracks
//                    );

////                    return lQuery(popAudioUrl(), QueryRules(tkn_tracks, requestLimit(), OD_SEARCH_LIMIT));
//                }
            };
        }
    }
}

#endif // OD_USER