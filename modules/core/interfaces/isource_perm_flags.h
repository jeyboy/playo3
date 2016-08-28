#ifndef ISOURCE_PERM_FLAGS
#define ISOURCE_PERM_FLAGS

namespace Core {
    enum Permissions : int {
        perm_none = 0,
        perm_site = 1,
        perm_api = 2
    };

    enum PermitRequest {
        pr_none                 = 0,

        pr_search_media         = 1,
        pr_search_objects       = 2, // user or group

        pr_media_content        = 4,
        pr_object_content       = 8, // user or group
//        pr_feed                 = 8,
        pr_pack                 = 16, // charts // tops // etc ...
        pr_recommendations      = 32
    };

    enum SourceFlags : quint64 {
        sf_none = 0,

        sf_is_primary                       = 1,
        sf_is_content_shareable             = (quint64(1)) << 1,

        sf_api                              = (quint64(1)) << 2,
        sf_site                             = (quint64(1)) << 3,

        sf_auth_only                        = (quint64(1)) << 4,
        sf_link                             = (quint64(1)) << 5,
        sf_search                           = (quint64(1)) << 6,
        sf_prefer                           = (quint64(1)) << 7,
        sf_recommendation                   = (quint64(1)) << 8,
        sf_my                               = (quint64(1)) << 9,

        sf_offline_credentials_req          = (quint64(1)) << 10,
        sf_online_credentials_req           = (quint64(1)) << 11,

        sf_user                             = (quint64(1)) << 12,
        sf_group                            = (quint64(1)) << 13,

        sf_track                            = (quint64(1)) << 14,
        sf_video                            = (quint64(1)) << 15,
        sf_set                              = (quint64(1)) << 16, // include charts and etc
        sf_album                            = (quint64(1)) << 17,
        sf_stream                           = (quint64(1)) << 18, // radio and etc
        sf_photo                            = (quint64(1)) << 19,
        sf_post                             = (quint64(1)) << 20,
        sf_feed                             = (quint64(1)) << 21,
        sf_lyric                            = (quint64(1)) << 22,
        //reserved                             = (quint64(1)) << 23,

        sf_by_perma                         = (quint64(1)) << 24,
        sf_by_id                            = (quint64(1)) << 25,
        sf_by_title                         = (quint64(1)) << 26,
        sf_by_artist                        = (quint64(1)) << 27,
        sf_by_album                         = (quint64(1)) << 28,
        sf_by_set                           = (quint64(1)) << 29,
        sf_by_abc                           = (quint64(1)) << 30,
        sf_by_tag                           = (quint64(1)) << 31,
        sf_by_mood                          = (quint64(1)) << 32,
        sf_by_genre                         = (quint64(1)) << 33,
        sf_by_lyric                         = (quint64(1)) << 34,

        sf_section_new                      = (quint64(1)) << 35,
        sf_section_popular                  = (quint64(1)) << 36,
//        sf_section_chart                    = (quint64(1)) << 37,

        //////////// main combinations //////////////////////////

        sf_api_auth_only                    = sf_api & sf_auth_only,
        sf_site_auth_only                   = sf_site & sf_auth_only,

        sf_site_offline_credentials_req     = sf_site & sf_offline_credentials_req,
        sf_site_online_credentials_req      = sf_site & sf_online_credentials_req,

        sf_track_sets                       = sf_set & sf_track,
        sf_video_sets                       = sf_set & sf_video,
        sf_feeds                            = sf_set & sf_feed,
        sf_posts                            = sf_set & sf_post,
        sf_lyrics                           = sf_set & sf_lyric,

        sf_tracks_stream                    = sf_stream & sf_track,
        sf_videos_stream                    = sf_stream & sf_video,

        sf_new_tracks                       = sf_track & sf_section_new,
        sf_popular_tracks                   = sf_track & sf_section_popular,
        sf_tracks_by_genre                  = sf_track & sf_by_genre,
        sf_tracks_by_mood                   = sf_track & sf_by_mood,
        sf_tracks_by_tag                    = sf_track & sf_by_tag,
        sf_tracks_by_artist                 = sf_track & sf_by_artist,
        sf_tracks_by_album                  = sf_track & sf_by_album,

        sf_new_videos                       = sf_video & sf_section_new,
        sf_popular_videos                   = sf_video & sf_section_popular,
        sf_videos_by_genre                  = sf_video & sf_by_genre,
        sf_videos_by_mood                   = sf_video & sf_by_mood,
        sf_videos_by_tag                    = sf_video & sf_by_tag,

        sf_new_lyrics                       = sf_lyric & sf_section_new,
        sf_popular_lyrics                   = sf_lyric & sf_section_popular,
        sf_lyrics_search_by_title           = sf_lyric & sf_search & sf_by_title, // rule including search by text part


        sf_tracks_recs_by_track_id          = sf_track & sf_by_id & sf_recommendation, // tracks recs by one track
        sf_tracks_recs_by_tracks_ids        = sf_track_sets & sf_by_id & sf_recommendation, // recs by a bunch of tracks
        sf_tracks_recs_by_set_id            = sf_track & sf_set & sf_by_id & sf_recommendation, //recs by playlist or set

        sf_videos_recs_by_video_id          = sf_video & sf_by_id & sf_recommendation, // videos recs by one video
        sf_videos_recs_by_videos_ids        = sf_video_sets & sf_by_id & sf_recommendation, // recs by a bunch of videos
        sf_videos_recs_by_set_id            = sf_video & sf_set & sf_by_id & sf_recommendation, //recs by playlist or set


        sf_users_sociable                   = sf_user & sf_link,
        sf_user_search_by_title             = sf_user & sf_search & sf_by_title,
        sf_user_search_by_perma             = sf_user & sf_search & sf_by_perma,
        sf_user_search_by_id                = sf_user & sf_search & sf_by_id,
        sf_user_track_sets                  = sf_user & sf_track_sets,
        sf_user_video_sets                  = sf_user & sf_video_sets,
        sf_user_feeds                       = sf_user & sf_feeds,
        sf_user_tracks                      = sf_user & sf_track,
        sf_user_videos                      = sf_user & sf_video,
        sf_user_photos                      = sf_user & sf_photo,
        sf_user_posts                       = sf_user & sf_post,
        sf_user_streams                     = sf_user & sf_stream,
        sf_user_track_albums                = sf_user_tracks & sf_album,
        sf_user_photo_albums                = sf_user_photos & sf_album,
        sf_user_tracks_recs                 = sf_user_tracks & sf_by_id & sf_recommendation, // tracks recs for user by id
        sf_my_user_tracks_recs              = sf_my & sf_user_tracks_recs, // tracks recs only for current user
        sf_user_videos_recs                 = sf_user_videos & sf_by_id & sf_recommendation, // videos recs for user by id
        sf_my_user_videos_recs              = sf_my & sf_user_videos_recs, // videos recs only for current user


        sf_groups_sociable                  = sf_group & sf_link,
        sf_group_search_by_title            = sf_group & sf_search & sf_by_title,
        sf_group_search_by_perma            = sf_group & sf_search & sf_by_perma,
        sf_group_search_by_id               = sf_group & sf_search & sf_by_id,
        sf_group_track_sets                 = sf_group & sf_track_sets,
        sf_group_video_sets                 = sf_group & sf_video_sets,
        sf_group_feeds                      = sf_group & sf_feeds,
        sf_group_tracks                     = sf_group & sf_track,
        sf_group_videos                     = sf_group & sf_video,
        sf_group_photos                     = sf_group & sf_photo,
        sf_group_posts                      = sf_group & sf_post,
        sf_group_streams                    = sf_group & sf_stream,
        sf_group_track_albums               = sf_group_tracks & sf_album,
        sf_group_photo_albums               = sf_group_photos & sf_album,
        sf_group_tracks_recs                = sf_group_tracks & sf_by_id & sf_recommendation, // tracks recs for group by id
        sf_group_videos_recs                = sf_group_videos & sf_by_id & sf_recommendation, // videos recs for group by id

        //////////// control combinations //////////////////////////


    };

//    enum SourceContentFlags : quint64 {
//        scf_none                            = 0,

//        scf_content_track_recomendable      = 1,
//        scf_content_tracks_recomendable     = (quint64(1)) << 1,
//        scf_content_tracks_search_by_title  = (quint64(1)) << 2,
//        scf_content_tracks_search_by_artist = (quint64(1)) << 3,
//        scf_content_tracks_search_by_abc    = (quint64(1)) << 4,
//        scf_content_tracks_search_by_tag    = (quint64(1)) << 5,
//        scf_content_tracks_search_by_mood   = (quint64(1)) << 6,
//        scf_content_tracks_search_by_genre  = (quint64(1)) << 7,
//        scf_content_tracks_search_by_lyric  = (quint64(1)) << 8,

//        scf_content_video_recomendable      = (quint64(1)) << 9,
//        scf_content_videos_search_by_title  = (quint64(1)) << 10,
//        scf_content_videos_search_by_artist = (quint64(1)) << 11,
//        scf_content_videos_search_by_abc    = (quint64(1)) << 12,
//        scf_content_videos_search_by_tag    = (quint64(1)) << 13,
//        scf_content_videos_search_by_mood   = (quint64(1)) << 14,
//        scf_content_videos_search_by_genre  = (quint64(1)) << 15,
//        scf_content_videos_search_by_lyric  = (quint64(1)) << 16,

//        scf_content_set_recomendable        = (quint64(1)) << 17, // also includes recs by playlists
//        scf_content_sets_search_by_title    = (quint64(1)) << 18,
//        scf_content_sets_search_by_abc      = (quint64(1)) << 19,
//        scf_content_sets_search_by_tag      = (quint64(1)) << 20,
//        scf_content_sets_search_by_genre    = (quint64(1)) << 21,
//        scf_content_sets_search_by_mood     = (quint64(1)) << 22,

//        scf_content_album_recomendable      = (quint64(1)) << 23,
//        scf_content_albums_search_by_title  = (quint64(1)) << 24,
//        scf_content_albums_search_by_artist = (quint64(1)) << 25,
//        scf_content_albums_search_by_abc    = (quint64(1)) << 26,
//        scf_content_albums_search_by_tag    = (quint64(1)) << 27,
//        scf_content_albums_search_by_mood   = (quint64(1)) << 28,
//        scf_content_albums_search_by_genre  = (quint64(1)) << 29,

//        scf_content_streams_search_by_title = (quint64(1)) << 30,
//        scf_content_streams_search_by_abc   = (quint64(1)) << 31,
//        scf_content_streams_search_by_tag   = (quint64(1)) << 32,
//        scf_content_streams_search_by_mood  = (quint64(1)) << 33,
//        scf_content_streams_search_by_genre = (quint64(1)) << 34,

//        scf_content_lyrics_search_by_title  = (quint64(1)) << 35,
//        scf_content_lyrics_search_by_id     = (quint64(1)) << 36 // track id and etc..
//    };

//    enum SourcePerms : quint64 {
//        sp_none                            = 0,

//        ///////////////////////// sections ////////////////////////

//        sp_site_section_popular_prefer     = 1,
//        sp_site_section_popular_auth       = (quint64(1)) << 1,
//        sp_api_section_popular_auth        = (quint64(1)) << 2,

//        sp_site_section_new_prefer         = (quint64(1)) << 3,
//        sp_site_section_new_auth           = (quint64(1)) << 4,
//        sp_api_section_new_auth            = (quint64(1)) << 5,

//        sp_site_section_packs_prefer       = (quint64(1)) << 6,
//        sp_site_section_packs_auth         = (quint64(1)) << 7,
//        sp_api_section_packs_auth          = (quint64(1)) << 8,

//        sp_site_section_feeds_prefer       = (quint64(1)) << 9,
//        sp_site_section_feeds_auth         = (quint64(1)) << 10,
//        sp_api_section_feeds_auth          = (quint64(1)) << 11,

//        sp_site_section_prefer             = sp_site_section_popular_prefer | sp_site_section_new_prefer |
//                                                sp_site_section_packs_prefer | sp_site_section_feeds_prefer,

//        sp_site_section_auth               = sp_site_section_popular_auth | sp_site_section_new_auth |
//                                                sp_site_section_packs_auth | sp_site_section_feeds_auth,

//        sp_api_section_auth                = sp_api_section_popular_auth | sp_api_section_new_auth |
//                                                sp_api_section_packs_auth | sp_api_section_feeds_auth,

//        ///////////////////////// media ///////////////////////////

//        sp_site_tracks_prefer              = (quint64(1)) << 12,
//        sp_site_tracks_auth                = (quint64(1)) << 13,
//        sp_api_tracks_auth                 = (quint64(1)) << 14,

//        sp_site_videos_prefer              = (quint64(1)) << 15,
//        sp_site_videos_auth                = (quint64(1)) << 16,
//        sp_api_videos_auth                 = (quint64(1)) << 17,

//        sp_site_sets_prefer                = (quint64(1)) << 18,
//        sp_site_sets_auth                  = (quint64(1)) << 19,
//        sp_api_sets_auth                   = (quint64(1)) << 20,

//        sp_site_albums_prefer              = (quint64(1)) << 21,
//        sp_site_albums_auth                = (quint64(1)) << 22,
//        sp_api_albums_auth                 = (quint64(1)) << 23,

//        sp_site_streams_prefer             = (quint64(1)) << 24,
//        sp_site_streams_auth               = (quint64(1)) << 25,
//        sp_api_streams_auth                = (quint64(1)) << 26,

//        sp_site_lyrics_prefer              = (quint64(1)) << 27,
//        sp_site_lyrics_auth                = (quint64(1)) << 28,
//        sp_api_lyrics_auth                 = (quint64(1)) << 29,

//        sp_site_photo_prefer               = (quint64(1)) << 30,
//        sp_site_photo_auth                 = (quint64(1)) << 31,
//        sp_api_photo_auth                  = (quint64(1)) << 32,

//        sp_site_media_prefer               = sp_site_tracks_prefer | sp_site_videos_prefer | sp_site_sets_prefer |
//                                                sp_site_albums_prefer | sp_site_streams_prefer | sp_site_photo_prefer | sp_site_lyrics_prefer,

//        sp_site_media_auth                 = sp_site_tracks_auth | sp_site_videos_auth | sp_site_sets_auth |
//                                                sp_site_albums_auth | sp_site_streams_auth | sp_site_photo_auth | sp_site_lyrics_auth,

//        sp_api_media_auth                  = sp_api_tracks_auth | sp_api_videos_auth | sp_api_sets_auth |
//                                                sp_api_albums_auth | sp_api_streams_auth | sp_api_photo_auth | sp_api_lyrics_auth,

//        sp_site_search_media_prefer        = (quint64(1)) << 33,
//        sp_site_search_media_auth          = (quint64(1)) << 34,
//        sp_api_search_media_auth           = (quint64(1)) << 35,

//        ///////////////////////// users and groups ////////////////
//        sp_site_objects_prefer             = (quint64(1)) << 36,
//        sp_site_objects_auth               = (quint64(1)) << 37,
//        sp_api_objects_auth                = (quint64(1)) << 38,

//        sp_site_search_objects_prefer      = (quint64(1)) << 39,
//        sp_site_search_objects_auth        = (quint64(1)) << 40,
//        sp_api_search_objects_auth         = (quint64(1)) << 41,

//        ///////////////////////// recomendations //////////////////
//        sp_site_recomendations_prefer      = (quint64(1)) << 42,
//        sp_site_recomendations_auth        = (quint64(1)) << 43,
//        sp_api_recomendations_auth         = (quint64(1)) << 44,

//        ///////////////////////////////////////////////////////////
//        sp_site_prefer                     = sp_site_section_prefer | sp_site_media_prefer | sp_site_search_media_prefer |
//                                                sp_site_objects_prefer | sp_site_search_objects_prefer | sp_site_recomendations_prefer,

//        sp_site_auth_mandatory             = sp_site_section_auth | sp_site_media_auth | sp_site_search_media_auth |
//                                                sp_site_objects_auth | sp_site_search_objects_auth | sp_site_recomendations_auth,

//        sp_api_auth_mandatory              = sp_api_section_auth | sp_api_media_auth | sp_api_search_media_auth |
//                                                sp_api_objects_auth | sp_api_search_objects_auth | sp_api_recomendations_auth,

//        sp_auth_mandatory                  = sp_site_auth_mandatory | sp_api_auth_mandatory,
//    };

//    struct SourceFlagsPack {

//    };
}

#endif // ISOURCE_PERM_FLAGS
