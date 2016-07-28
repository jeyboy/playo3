#ifndef OD_ARTIST
#define OD_ARTIST

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Artist : public Base {
            protected:
                inline QString artistAudiosUrl() { return audioUrlStr(path_audio_by_artist_id); } // params : (artistId: artist id)
                inline QString searchArtistsUrl(const QString & predicate) { return audioUrlStr(path_audio_search_artists, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs
            };
        }
    }
}

#endif // OD_ARTIST
