#ifndef OD_USER
#define OD_USER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class User : public Base {
            protected:
                inline QString initAudioUrl() { return audioUrlStr(path_audio_init); }
                inline QString myAudioUrl(const QString & uid) { return audioUrlStr(tkn_my, QUrlQuery(tkn_uid_eq % uid)); } // params: (uid: sets for friend request) and pagination attrs
                QJsonObject userInfo(const QString & uid) {
                    if (uid.isEmpty()) {
                        qDebug() << initAudioUrl();
                        return Manager::prepare() -> jsonGet(initAudioUrl());
                    } else
                        return Manager::prepare() -> jsonGet(myAudioUrl(uid));
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
