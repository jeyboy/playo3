#ifndef OD_REQUESTS
#define OD_REQUESTS

#include "od_artist.h"
#include "od_auth.h"
#include "od_playlist.h"
#include "od_radio.h"
#include "od_set.h"
#include "od_track.h"
#include "od_video.h"
#include "od_video_playlist.h"

#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Requests : public ISource, public Sociable, public Artist, public Auth, public Playlist,
                    public Radio, public Set, public Track, public Video, public VideoPlaylist {
            protected:

            public:

            };
        }
    }
}

#endif // OD_REQUESTS
