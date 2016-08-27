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

    enum SourceFlags : quint32 {
        sf_none = 0,

        sf_is_primary                       = 1,
        sf_is_content_shareable             = (quint64(1)) << 1,

        sf_api                              = (quint64(1)) << 2,
        sf_site                             = (quint64(1)) << 3,

        sf_auth_only                        = (quint64(1)) << 4,
        sf_connectable                      = (quint64(1)) << 5,
        sf_searchable                       = (quint64(1)) << 6,

        sf_offline_credentials_req          = (quint64(1)) << 6,
        sf_online_credentials_req           = (quint64(1)) << 7,

        sf_user                             = (quint64(1)) << 8,
        sf_group                            = (quint64(1)) << 9,

        sf_track                            = (quint64(1)) << 10,
        sf_video                            = (quint64(1)) << 11,
        sf_set                              = (quint64(1)) << 12, // include charts and etc
        sf_album                            = (quint64(1)) << 13,
        sf_stream                           = (quint64(1)) << 14, // radio and etc
        sf_photo                            = (quint64(1)) << 15,
        sf_post                             = (quint64(1)) << 16,
        sf_feed                             = (quint64(1)) << 17,
        //reserved                             = (quint64(1)) << 18,
        //reserved                             = (quint64(1)) << 19,

        sf_by_perma                         = (quint64(1)) << 20,
        sf_by_id                            = (quint64(1)) << 21,
        sf_by_title                         = (quint64(1)) << 22,
        sf_by_artist                        = (quint64(1)) << 23,
        sf_by_abc                           = (quint64(1)) << 24,
        sf_by_tag                           = (quint64(1)) << 25,
        sf_by_mood                          = (quint64(1)) << 26,
        sf_by_genre                         = (quint64(1)) << 27,
        sf_by_lyric                         = (quint64(1)) << 28,

        sf_section_new                      = (quint64(1)) << 29,
        sf_section_popular                  = (quint64(1)) << 30,
//        sf_section_chart                    = (quint64(1)) << 31,

        //////////// combinations //////////////////////////

        sf_api_auth_only                    = sf_api & sf_auth_only,
        sf_site_auth_only                   = sf_site & sf_auth_only,

        sf_site_offline_credentials_req     = sf_site & sf_offline_credentials_req,
        sf_site_online_credentials_req      = sf_site & sf_online_credentials_req,

        sf_track_sets                       = sf_set & sf_track,
        sf_video_sets                       = sf_set & sf_video,
        sf_feeds                            = sf_set & sf_feed,
        sf_posts                            = sf_set & sf_post,

        sf_new_tracks                       = sf_track & sf_section_new,
        sf_popular_tracks                   = sf_track & sf_section_popular,
        sf_tracks_by_genre                  = sf_track & sf_by_genre,
        sf_tracks_by_mood                   = sf_track & sf_by_mood,
        sf_tracks_by_tag                    = sf_track & sf_by_tag,

        sf_new_videos                       = sf_video & sf_section_new,
        sf_popular_videos                   = sf_video & sf_section_popular,
        sf_videos_by_genre                  = sf_video & sf_by_genre,
        sf_videos_by_mood                   = sf_video & sf_by_mood,
        sf_videos_by_tag                    = sf_video & sf_by_tag,

        sf_users_sociable                   = sf_user & sf_connectable,
        sf_user_track_sets                  = sf_user & sf_track_sets,
        sf_user_video_sets                  = sf_user & sf_video_sets,
        sf_user_feeds                       = sf_user & sf_feeds,
        sf_user_tracks                      = sf_user & sf_track,
        sf_user_videos                      = sf_user & sf_video,
        sf_user_search_by_title             = sf_user & sf_searchable & sf_by_title,
        sf_user_search_by_perma             = sf_user & sf_searchable & sf_by_perma,
        sf_user_search_by_id                = sf_user & sf_searchable & sf_by_id,

        sf_groups_sociable                  = sf_group & sf_connectable,
        sf_group_track_sets                 = sf_group & sf_track_sets,
        sf_group_video_sets                 = sf_group & sf_video_sets,
        sf_group_feeds                      = sf_group & sf_feeds,
        sf_group_tracks                     = sf_group & sf_track,
        sf_group_videos                     = sf_group & sf_video,
        sf_group_search_by_title            = sf_group & sf_searchable & sf_by_title,
        sf_group_search_by_perma            = sf_group & sf_searchable & sf_by_perma,
        sf_group_search_by_id               = sf_group & sf_searchable & sf_by_id,
    };

//    enum SourceAccessFlags : quint64 {
//        sf_none = 0,

//        sf_section_tracks_new               = 1,
//        sf_section_tracks_popular           = (quint64(1)) << 1,
//        sf_section_tracks_packs             = (quint64(1)) << 2,
//        sf_section_track_streams            = (quint64(1)) << 3,

//        sf_section_videos_new               = (quint64(1)) << 4,
//        sf_section_videos_popular           = (quint64(1)) << 5,
//        sf_section_videos_packs             = (quint64(1)) << 6,
//        sf_section_video_streams            = (quint64(1)) << 7,


//        //        sf_taggable                         = (quint64(1)) << 26, // has grouping by tags
//        //        sf_genreable                        = (quint64(1)) << 27, // has grouping by genres
//        //        sf_moodable                         = (quint64(1)) << 28, // has grouping by moods
//        //        sf_content_lyrics_has               = (quint64(1)) << 25,

//        sf_users_sociable                   = (quint64(1)) << 9, // have access to user content
//        sf_users_searchable                 = (quint64(1)) << 10,
//        sf_user_my_recomendations           = (quint64(1)) << 11, // give recomendations only for current user
//        sf_user_recomendable                = (quint64(1)) << 12, // give recomendations by user
//        sf_user_tracks_has                  = (quint64(1)) << 13,
//        sf_user_videos_has                  = (quint64(1)) << 14,
//        sf_user_sets_has                    = (quint64(1)) << 15,
//        sf_user_albums_has                  = (quint64(1)) << 16,
//        sf_user_streams_has                 = (quint64(1)) << 17, // also includes radios and etc
//        sf_user_photo_has                   = (quint64(1)) << 18,
//        sf_user_posts_has                   = (quint64(1)) << 19,

//        sf_groups_sociable                  = (quint64(1)) << 20, // have access to group content
//        sf_groups_serachable                = (quint64(1)) << 21,
//        sf_group_tracks_has                 = (quint64(1)) << 22,
//        sf_group_videos_has                 = (quint64(1)) << 23,
//        sf_group_sets_has                   = (quint64(1)) << 24,
//        sf_group_albums_has                 = (quint64(1)) << 25,
//        sf_group_streams_has                = (quint64(1)) << 26, // also includes radios and etc
//        sf_group_photo_has                  = (quint64(1)) << 27,
//        sf_group_posts_has                  = (quint64(1)) << 28,
//    };

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
