#ifndef SOUNDCLOUD_API_REQUESTS
#define SOUNDCLOUD_API_REQUESTS

#include "defines.h"

#include "auth.h"
#include "group.h"
#include "playlist.h"
#include "set.h"
#include "track.h"
#include "user.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Requests : public Auth, public Group, public Playlist,
                        public Set, public Track, public User {
                public:
                    inline virtual ~Requests() {}
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_REQUESTS
