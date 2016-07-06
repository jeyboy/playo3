#ifndef SOUNDCLOUD_API_TRACK
#define SOUNDCLOUD_API_TRACK

#include "defines.h"
#include "../interfaces/itrack.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Track : public QueryBase, public ITrack {
                public:
                    QJsonValue tracksByUserLikes(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href


                        return res;
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_TRACK
