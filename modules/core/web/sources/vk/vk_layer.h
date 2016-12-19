#ifndef VK_LAYER_H
#define VK_LAYER_H



namespace Core {
    namespace Web {
        namespace Vk {
            class Layer : public Base {
            protected:
                Layer() {
                    flags = {
                        {sf_endpoint, (SourceFlags)(
                            sf_is_primary | sf_audio | sf_video | sf_sociable |
                            sf_photo | sf_feed | sf_lyric | sf_playlist | sf_compilation |
                            sf_site | sf_site_connectable | sf_api | sf_api_connectable)
                        },

                        {sf_feed,                       sf_both_auth},

//                        {sf_feed_by_user,               sf_both_auth},
//                        {sf_feed_by_group,              sf_both_auth},

                        {sf_is_shareable,               sf_both_auth /*| sf_multiple_keys*/},

                        {sf_search,                     sf_both_auth},

                        {sf_compilation,                sf_both_auth},

//                        {sf_popular_artist,         sf_both_auth},

//                        {sf_popular_audio,          sf_both_auth},
                        {sf_audio_by_id,                sf_both_auth},
                        {sf_audio_by_title,             sf_both_auth},
                        {sf_audio_by_genre,             sf_both_auth},
//                        {sf_audio_by_stream,        sf_both_auth},
                        {sf_audio_by_artist,            sf_both_auth},
//                        {sf_audio_by_compilation,   sf_both_auth},
                        {sf_audio_by_playlist,          sf_both_auth},
//                        {sf_audio_by_album,         sf_both_auth},
                        {sf_audio_by_user,              sf_both_auth},
                        {sf_audio_recs_by_user,         sf_both_auth},
                        {sf_audio_recs_by_audio,        sf_both_auth},

//                        {sf_audio_playlist_by_id,   sf_both_auth},
                        {sf_audio_playlist_by_user,     sf_both_auth},

                        {sf_video_categories,           sf_both_auth},
                        {sf_video_by_id,                sf_both_auth},
                        {sf_video_by_title,             sf_both_auth},
                        {sf_video_by_user,              sf_both_auth},
                        {sf_video_by_category,          sf_both_auth},
                        {sf_video_by_playlist,          sf_both_auth},

                        {sf_user_sociable,              sf_both_auth},
                        {sf_user_status,                sf_api},
                        {sf_user_by_id,                 sf_both_auth},
                        {sf_user_by_title,              sf_both_auth},
                        {sf_user_by_perma,              sf_both_auth},

                        {sf_group_sociable,             sf_both_auth},
                        {sf_group_by_user,              sf_both_auth},
                        {sf_group_by_title,             sf_both_auth},
                        {sf_group_by_id,                sf_both_auth},
                        {sf_group_by_perma,             sf_both_auth},

                        {sf_lyric_by_audio,             sf_both_auth}
                    };
                }
            };
        }
    }
}

#endif // VK_LAYER_H
