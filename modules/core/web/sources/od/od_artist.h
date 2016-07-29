#ifndef OD_ARTIST
#define OD_ARTIST

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Artist : public Base {
            protected:
//                inline QString searchArtistsUrl(const QString & predicate) { return audioUrlStr(path_audio_search_artists, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs

                QJsonValue artistsInfo(const QStringList & uids) {
                    return sRequest(
                        audioInfoUrl(uids),
                        call_type_json
                    ).value(tkn_artists).toArray();
                }

//                QJsonValue artistsSearch(const SearchLimit & limits) { // need to check
//                    return pRequest(
//                        audioUrlStr(
//                            path_audio_search_artists,
//                            { {tkn_q, limits.predicate} }
//                        ),
//                        call_type_json,
//                        rules(limits.start_offset, qMin(OD_SEARCH_LIMIT, limits.items_limit)),
//                        0, proc_json_extract, QStringList() << tkn_artists
//                    );
//                }
            };
        }
    }
}

#endif // OD_ARTIST
