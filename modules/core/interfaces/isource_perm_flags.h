#ifndef ISOURCE_PERM_FLAGS
#define ISOURCE_PERM_FLAGS

namespace Core {
    #define HAS_FLAG(flags, flag) ((flags & flag) == flag)

    enum SourceFlags : quint64 {
        sf_none = 0,

        sf_endpoint                         = 1,
        sf_site                             = (quint64(1)) << 1,
        sf_api                              = (quint64(1)) << 2,

        sf_site_auth_only                   = (quint64(1)) << 3,
        sf_api_auth_only                    = (quint64(1)) << 4,

        sf_offline_credentials_req          = (quint64(1)) << 5,
        sf_online_credentials_req           = (quint64(1)) << 6,

        sf_site_connectable                 = (quint64(1)) << 7,
        sf_api_connectable                  = (quint64(1)) << 8,

        sf_is_primary                       = (quint64(1)) << 9,
        sf_is_content_shareable             = (quint64(1)) << 10,

        sf_link                             = (quint64(1)) << 11,
        sf_search                           = (quint64(1)) << 12,
        sf_config                           = (quint64(1)) << 13,
        //reserved                           = (quint64(1)) << 14,
        //reserved                           = (quint64(1)) << 15,

        sf_recommendation                   = (quint64(1)) << 16,
        sf_my                               = (quint64(1)) << 17,

        sf_user                             = (quint64(1)) << 18,
        sf_group                            = (quint64(1)) << 19,

        sf_track                            = (quint64(1)) << 20,
        sf_video                            = (quint64(1)) << 21,
        sf_playlist                         = (quint64(1)) << 22, // sets from sociables
        sf_compilation                      = (quint64(1)) << 23, // include charts and etc
        sf_album                            = (quint64(1)) << 24,
        sf_stream                           = (quint64(1)) << 25, // radio and etc
        sf_photo                            = (quint64(1)) << 26,
        sf_post                             = (quint64(1)) << 27,
        sf_feed                             = (quint64(1)) << 28,
        sf_lyric                            = (quint64(1)) << 29,
        sf_label                            = (quint64(1)) << 30,
        sf_artist                           = (quint64(1)) << 31,

        sf_by_label                         = (quint64(1)) << 32,
        sf_by_perma                         = (quint64(1)) << 33,
        sf_by_id                            = (quint64(1)) << 34,
        sf_by_title                         = (quint64(1)) << 35,
        sf_by_artist                        = (quint64(1)) << 36,
        sf_by_track                         = (quint64(1)) << 37,
        sf_by_album                         = (quint64(1)) << 38,
        sf_by_compilation                   = (quint64(1)) << 39,
        sf_by_playlist                      = (quint64(1)) << 40,
        sf_by_stream                        = (quint64(1)) << 41,
        sf_by_abc                           = (quint64(1)) << 42,
        sf_by_tag                           = (quint64(1)) << 43,
        sf_by_mood                          = (quint64(1)) << 44,
        sf_by_genre                         = (quint64(1)) << 45,
        sf_by_lyric                         = (quint64(1)) << 46,
        sf_by_user                          = (quint64(1)) << 47,
        sf_by_group                         = (quint64(1)) << 48,
        sf_by_category                      = (quint64(1)) << 49,
        sf_by_like                          = (quint64(1)) << 50,
        sf_by_repost                        = (quint64(1)) << 51,

        sf_section_new                      = (quint64(1)) << 52,
        sf_section_popular                  = (quint64(1)) << 53,
        sf_section_categories               = (quint64(1)) << 54,
    //        sf_section_chart                    = (quint64(1)) << 55,

        //////////// combinations //////////////////////////
        sf_sociable                         = sf_user | sf_group,

        sf_both                             = sf_api | sf_site,
        sf_both_auth                        = sf_api_auth_only | sf_site_auth_only,
        sf_site_auth_api_not                = sf_api | sf_site_auth_only,
        sf_site_not_api_auth                = sf_api_auth_only | sf_site,

        sf_audio_playlist                   = sf_playlist | sf_track,
        sf_video_playlist                   = sf_playlist | sf_video,
        sf_blog                             = sf_compilation | sf_post,
        sf_photo_album                      = sf_compilation | sf_photo,

        sf_audio_stream                     = sf_stream | sf_track,
        sf_video_stream                     = sf_stream | sf_video,

        sf_new_audio                        = sf_track | sf_section_new,
        sf_popular_audio                    = sf_track | sf_section_popular,
        sf_audio_by_id                      = sf_track | sf_by_id,
        sf_audio_by_title                   = sf_track | sf_by_title,
        sf_audio_by_genre                   = sf_track | sf_by_genre,
        sf_audio_by_mood                    = sf_track | sf_by_mood,
        sf_audio_by_tag                     = sf_track | sf_by_tag,
        sf_audio_by_artist                  = sf_track | sf_by_artist,
        sf_audio_by_album                   = sf_track | sf_by_album,
        sf_audio_by_label                   = sf_track | sf_by_label,
        sf_audio_by_compilation             = sf_track | sf_by_compilation,
        sf_audio_by_playlist                = sf_track | sf_by_playlist,
        sf_audio_by_stream                  = sf_track | sf_by_stream,
        sf_audio_by_user                    = sf_track | sf_by_user,
        sf_audio_by_group                   = sf_track | sf_by_group,
        sf_audio_by_like                    = sf_track | sf_by_like,
        sf_audio_recs_by_user               = sf_user | sf_track | sf_recommendation, // tracks recs for user or group by id
        sf_audio_recs_by_me                 = sf_my | sf_audio_recs_by_user, // tracks recs only for current user
        sf_audio_recs_by_audio              = sf_track | sf_recommendation, // tracks recs by one track
        sf_audio_recs_by_playlist           = sf_track | sf_playlist | sf_recommendation, //recs by playlist or set
        sf_audio_recs_by_artist             = sf_track | sf_by_artist | sf_recommendation, //recs by playlist or set

        sf_new_artist                       = sf_artist | sf_section_new,
        sf_popular_artist                   = sf_artist | sf_section_popular,
        sf_artist_by_id                     = sf_artist | sf_by_id,
        sf_artist_by_title                  = sf_artist | sf_by_title,

        sf_new_album                        = sf_album | sf_section_new,
        sf_popular_album                    = sf_album | sf_section_popular,
        sf_album_by_id                      = sf_album | sf_by_id,
        sf_album_by_title                   = sf_album | sf_by_title,
        sf_album_by_label                   = sf_album | sf_by_label,
        sf_album_by_artist                  = sf_album | sf_by_artist,

        sf_new_video                        = sf_video | sf_section_new,
        sf_popular_video                    = sf_video | sf_section_popular,
        sf_video_categories                 = sf_video | sf_section_categories,
        sf_video_by_id                      = sf_video | sf_by_id,
        sf_video_by_category                = sf_video | sf_by_category,
        sf_video_by_title                   = sf_video | sf_by_title,
        sf_video_by_genre                   = sf_video | sf_by_genre,
        sf_video_by_mood                    = sf_video | sf_by_mood,
        sf_video_by_tag                     = sf_video | sf_by_tag,
        sf_video_by_playlist                = sf_video | sf_by_playlist,
        sf_video_by_user                    = sf_video | sf_by_user,
        sf_video_by_group                   = sf_video | sf_by_group,
        sf_video_recs_by_user               = sf_user | sf_video | sf_recommendation, // videos recs for user or group by id
        sf_video_recs_by_me                 = sf_my | sf_video_recs_by_user, // videos recs only for current user
        sf_video_recs_by_video              = sf_video | sf_recommendation, // videos recs by one video
        sf_video_recs_by_playlist           = sf_video | sf_playlist | sf_recommendation, //recs by playlist or set
        sf_video_recs_by_artist             = sf_video | sf_by_artist | sf_recommendation, //recs by playlist or set


        sf_audio_playlist_by_id             = sf_audio_playlist | sf_by_id,
        sf_audio_playlist_by_title          = sf_audio_playlist | sf_by_title,
        sf_audio_playlist_by_user           = sf_audio_playlist | sf_by_user,
        sf_audio_playlist_by_group          = sf_audio_playlist | sf_by_group,
        sf_audio_playlist_by_tag            = sf_audio_playlist | sf_by_tag,
        sf_audio_playlist_by_track          = sf_audio_playlist | sf_by_track,

        sf_video_playlist_by_id             = sf_video_playlist | sf_by_id,
        sf_video_playlist_by_user           = sf_video_playlist | sf_by_user,
        sf_video_playlist_by_group          = sf_video_playlist | sf_by_group,

        sf_feed_by_user                     = sf_feed | sf_by_user,
        sf_feed_by_group                    = sf_feed | sf_by_group,

        sf_stream_list                      = sf_stream | sf_compilation,
        sf_stream_config                    = sf_stream | sf_config,
        sf_stream_by_user                   = sf_stream | sf_by_user,
        sf_stream_by_group                  = sf_stream | sf_by_group,
        sf_stream_by_genre                  = sf_stream | sf_by_genre,
        sf_stream_by_artist                 = sf_stream | sf_by_artist,

        sf_post_by_user                     = sf_post | sf_by_user,
        sf_post_by_group                    = sf_post | sf_by_group,

        sf_photo_by_user                    = sf_photo | sf_by_user,
        sf_photo_by_group                   = sf_photo | sf_by_group,

        sf_photo_album_by_user              = sf_photo_album | sf_by_user,
        sf_photo_album_by_group             = sf_photo_album | sf_by_group,


        sf_new_lyric                        = sf_lyric | sf_section_new,
        sf_popular_lyric                    = sf_lyric | sf_section_popular,
        sf_lyric_by_title                   = sf_lyric | sf_by_title, // rule including search by text part
        sf_lyric_by_audio                   = sf_lyric | sf_track, // rule including search by track id
        sf_lyric_by_video                   = sf_lyric | sf_video, // rule including search by video id

        sf_user_sociable                    = sf_user | sf_link,
        sf_user_by_title                    = sf_user | sf_by_title,
        sf_user_by_perma                    = sf_user | sf_by_perma,
        sf_user_by_id                       = sf_user | sf_by_id,
        sf_user_by_likes                    = sf_user | sf_by_like,
        sf_user_by_audio_repost             = sf_user | sf_by_repost | sf_track,
        sf_user_by_user                     = sf_user | sf_by_user,


        sf_group_sociable                   = sf_group | sf_link,
        sf_group_by_user                    = sf_group | sf_by_user,
        sf_group_by_title                   = sf_group | sf_by_title,
        sf_group_by_perma                   = sf_group | sf_by_perma,
        sf_group_by_id                      = sf_group | sf_by_id,
        sf_group_by_track                   = sf_group | sf_by_track
    } ;
}

#endif // ISOURCE_PERM_FLAGS
