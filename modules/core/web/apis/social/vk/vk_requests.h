#ifndef VK_REQUESTS
#define VK_REQUESTS

#include "vk_auth.h"
#include "vk_feed.h"
#include "vk_group.h"
#include "vk_playlist.h"
//#include "vk_set.h"
#include "vk_track.h"
#include "vk_video.h"
#include "vk_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"
#include "modules/core/interfaces/isource.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Requests : public ISource, public Sociable, public Auth, public Feed, public Group,
                    public Playlist, /*public Set,*/ public Track, public Video, public User {
            public:
                QJsonValue userTracksGroupsFriends(const QString & user_id) {
                    return sRequest(
                        baseUrlStr(
                            qst_api_def, tkn_execute,
                            {{ tkn_code, query_user_tracks_groups_friends.arg(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );
                }

                QJsonObject userInfo(QString & user_id) {
                    QJsonObject ret = sRequest(
                        baseUrlStr(
                            qst_api_def, tkn_execute,
                            {{ tkn_code, query_user_tracks_playlists_groups_friends.arg(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );

                    if (!ret.value(tkn_albums_finished).toBool()) {
                        QJsonArray ar = ret.value(tkn_albums).toArray();
                        audioAlbums(uid, ar, ret.value(tkn_albums_offset).toInt());
                        ret.insert(tkn_albums, ar);
                    }
                    return ret;
                }

                QJsonObject userAudio(QString & user_id) {
                    QJsonObject ret = sRequest(
                        baseUrlStr(
                            qst_api_def, tkn_execute,
                            {{ tkn_code, query_user_tracks_playlists.arg(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );

                    if (!ret.value(tkn_albums_finished).toBool()) {
                        QJsonArray ar = ret.value(tkn_albums).toArray();
                        audioAlbums(uid, ar, ret.value(tkn_albums_offset).toInt());
                        ret.insert(tkn_albums, ar);
                    }
                    return ret;
                }
            };
        }
    }
}

#endif // VK_REQUESTS
