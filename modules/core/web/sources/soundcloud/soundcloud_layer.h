#ifndef SOUNDCLOUD_LAYER_H
#define SOUNDCLOUD_LAYER_H

#include "soundcloud_auth.h"
#include "soundcloud_group.h"
#include "soundcloud_playlist.h"
#include "soundcloud_set.h"
#include "soundcloud_audio.h"
#include "soundcloud_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Layer : public Sociable, public Auth, public Group,
                    public Playlist, public Set, public Audio, public User {
            protected:
                Layer() {
                    flags = {
                        {sf_endpoint, (SourceFlags)(
                            sf_offline_credentials_req | sf_is_primary | sf_audio | sf_user/*sf_sociable*/ |
                            sf_feed | sf_playlist | sf_compilation | sf_site | sf_api | sf_api_connectable)
                        },

//                        {sf_feed,                   sf_site},

                        {sf_feed_by_user,               sf_both_auth},
//                        {sf_feed_by_group,          sf_site_auth_only},

                        {sf_is_shareable,               sf_site_not_api_auth},

                        {sf_search,                     sf_site_not_api_auth},

                        {sf_compilation,                sf_site_not_api_auth},

//                        {sf_popular_audio,          sf_site_auth_only},
                        {sf_like_audio,                 sf_both_auth},
                        {sf_audio_by_id,                sf_site_not_api_auth},
                        {sf_audio_by_title,             sf_site_not_api_auth},
                        {sf_audio_by_genre,             sf_site_not_api_auth},
                        {sf_audio_by_tag,               sf_site_not_api_auth},
                        {sf_audio_by_group,             sf_site_not_api_auth},
                        {sf_audio_by_like,              sf_site_not_api_auth},
                        {sf_audio_by_user,              sf_site_not_api_auth},
                        {sf_audio_recs_by_user,         sf_site},
                        {sf_audio_recs_by_audio,        sf_site_not_api_auth},

//                        {sf_audio_playlist_by_id,   sf_site_auth_only},
                        {sf_audio_playlist_by_title,    sf_site_not_api_auth},
                        {sf_audio_playlist_by_tag,      sf_site},
                        {sf_audio_playlist_by_audio,    sf_site_not_api_auth},
                        {sf_audio_playlist_by_user,     sf_site_not_api_auth},

                        {sf_user_sociable,              sf_site_not_api_auth},
//                        {sf_group_sociable,             sf_site_not_api_auth},

                        {sf_user_by_id,                 sf_site_not_api_auth},
                        {sf_user_by_title,              sf_site_not_api_auth},
                        {sf_user_by_likes,              sf_site_not_api_auth},
                        {sf_user_by_audio_repost,       sf_site_not_api_auth},

//                        {sf_group_by_user,              sf_site_not_api_auth},
//                        {sf_group_by_title,             sf_site_not_api_auth},
//                        {sf_group_by_id,                sf_site_not_api_auth},
//                        {sf_group_by_audio,             sf_site}
                    };
                }


            };
        }
    }
}

#endif // SOUNDCLOUD_LAYER_H
