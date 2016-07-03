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

    //                void getGroupInfo(const QString & uid, QJsonObject & object) {
    //                    object.insert(tkn_audio_list, groupAudio(uid));
    //                }

//                    void getUserInfo(const QString & uid, QJsonObject & object) {
//                        object.insert(tkn_audio_list, userAudio(uid));
//                        object.insert(tkn_playlist, userPlaylists(uid));
//                        QThread::msleep(REQUEST_DELAY);
//                        object.insert(tkn_followings, userFollowings(uid)); // return bad request error
//                        object.insert(tkn_followers, userFollowers(uid));
//                        QThread::msleep(REQUEST_DELAY);
//                        object.insert(tkn_groups, userGroups(uid));
//                    }

                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_REQUESTS
