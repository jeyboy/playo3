#ifndef OD_TRACK
#define OD_TRACK

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Track : public Base {
            protected:
                // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
                inline QString playAudioUrl(QString tid/*, int item_type = 5*/) { return audioUrlStr(path_audio_play, QUrlQuery(tkn_tid_eq % tid)); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
                inline QString play30AudioUrl() { return audioUrlStr(path_audio_play_30_sec); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))
                inline QString isDownloadedAudioUrl() { return audioUrlStr(path_audio_is_downloaded); } // params : (tid: track id)

                inline QString searchTracksUrl(const QString & predicate) { return audioUrlStr(path_audio_search_tracks, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs

                QJsonArray audioInfo(const QStringList & uids) { // take only tracks - response also contains artists and albums arrays
                    return sRequest(
                        customAudioUrl(uids),
                        call_type_json
                    ).value(tkn_tracks).toArray();
                }
            };
        }
    }
}

#endif // OD_TRACK
