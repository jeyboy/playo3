#ifndef OD_SET
#define OD_SET

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Set : public Base {
            protected:
                inline QString listenedAudioUrl() { return audioUrlStr(path_audio_history); } // params : pagination attrs
                inline QString downloadedAudioUrl() { return audioUrlStr(path_audio_downloaded); } // params : pagination attrs
                inline QString popularAudioUrl() { return audioUrlStr(path_audio_popular_tracks); }
                // artists // tracks // tuners // collections // albums
                inline QString popAudioUrl() { return audioUrlStr(path_audio_popular); } // (locale: 'ru') and pagination attrs

                QJsonValue popular(const SearchLimit & /*limits*/) {
                    return pRequest(
                        popAudioUrl(),
                        call_type_json,
                        rules(),
                        0,
                        proc_json_extract,
                        QStringList() << tkn_tracks
                    );

//                    return lQuery(popAudioUrl(), QueryRules(tkn_tracks, requestLimit(), OD_SEARCH_LIMIT));
                }
            };
        }
    }
}

#endif // OD_SET
