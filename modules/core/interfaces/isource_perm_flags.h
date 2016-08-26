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

        sf_api_has                          = (quint64(1)) << 1,
        sf_site_has                         = (quint64(1)) << 2,
        sf_site_offline_credentials_req     = (quint64(1)) << 3,
        sf_site_online_credentials_req      = (quint64(1)) << 4,

        ///////////////////////////// info flags ///////////////////////
        sf_user_my_recomendations           = (quint64(1)) << 5, // give recomendations only for current user
        sf_user_recomendable                = (quint64(1)) << 6, // give recomendations by user
        sf_users_sociable                   = (quint64(1)) << 7, // have access to user content
        sf_users_searchable                 = (quint64(1)) << 8,

        sf_groups_sociable                  = (quint64(1)) << 9, // have access to group content
        sf_groups_serachable                = (quint64(1)) << 10,

        sf_section_new                      = (quint64(1)) << 11,
        sf_section_popular                  = (quint64(1)) << 12,
        sf_section_packs                    = (quint64(1)) << 13,

        sf_section_feeds                    = (quint64(1)) << 14,
        sf_section_streams                  = (quint64(1)) << 15,

        //        sf_taggable                         = (quint64(1)) << 26, // has grouping by tags
        //        sf_genreable                        = (quint64(1)) << 27, // has grouping by genres
        //        sf_moodable                         = (quint64(1)) << 28, // has grouping by moods

        sf_content_user_tracks_has          = (quint64(1)) << 16,
        sf_content_group_tracks_has         = (quint64(1)) << 17,

        sf_content_user_videos_has          = (quint64(1)) << 18,
        sf_content_group_videos_has         = (quint64(1)) << 19,

        sf_content_user_sets_has            = (quint64(1)) << 20,
        sf_content_group_sets_has           = (quint64(1)) << 21,

        sf_content_user_albums_has          = (quint64(1)) << 22,
        sf_content_group_albums_has         = (quint64(1)) << 23,

        sf_content_user_streams_has         = (quint64(1)) << 24, // also includes radios and etc
        sf_content_group_streams_has        = (quint64(1)) << 25, // also includes radios and etc

        sf_content_user_photo_has           = (quint64(1)) << 26,
        sf_content_group_photo_has          = (quint64(1)) << 27,

        sf_content_user_posts_has           = (quint64(1)) << 28,
        sf_content_group_posts_has          = (quint64(1)) << 29,

//        sf_content_lyrics_has               = (quint64(1)) << 25,
    };

    enum SourceContentFlags : quint64 {
        scf_none                            = 0,

        scf_content_track_recomendable      = 1,
        scf_content_tracks_recomendable     = (quint64(1)) << 1,
        scf_content_tracks_search_by_title  = (quint64(1)) << 2,
        scf_content_tracks_search_by_artist = (quint64(1)) << 3,
        scf_content_tracks_search_by_abc    = (quint64(1)) << 4,
        scf_content_tracks_search_by_tag    = (quint64(1)) << 5,
        scf_content_tracks_search_by_mood   = (quint64(1)) << 6,
        scf_content_tracks_search_by_genre  = (quint64(1)) << 7,
        scf_content_tracks_search_by_lyric  = (quint64(1)) << 8,

        scf_content_video_recomendable      = (quint64(1)) << 9,
        scf_content_videos_search_by_title  = (quint64(1)) << 10,
        scf_content_videos_search_by_artist = (quint64(1)) << 11,
        scf_content_videos_search_by_abc    = (quint64(1)) << 12,
        scf_content_videos_search_by_tag    = (quint64(1)) << 13,
        scf_content_videos_search_by_mood   = (quint64(1)) << 14,
        scf_content_videos_search_by_genre  = (quint64(1)) << 15,
        scf_content_videos_search_by_lyric  = (quint64(1)) << 16,

        scf_content_set_recomendable        = (quint64(1)) << 17, // also includes recs by playlists
        scf_content_sets_search_by_title    = (quint64(1)) << 18,
        scf_content_sets_search_by_abc      = (quint64(1)) << 19,
        scf_content_sets_search_by_tag      = (quint64(1)) << 20,
        scf_content_sets_search_by_genre    = (quint64(1)) << 21,
        scf_content_sets_search_by_mood     = (quint64(1)) << 22,

        scf_content_album_recomendable      = (quint64(1)) << 23,
        scf_content_albums_search_by_title  = (quint64(1)) << 24,
        scf_content_albums_search_by_artist = (quint64(1)) << 25,
        scf_content_albums_search_by_abc    = (quint64(1)) << 26,
        scf_content_albums_search_by_tag    = (quint64(1)) << 27,
        scf_content_albums_search_by_mood   = (quint64(1)) << 28,
        scf_content_albums_search_by_genre  = (quint64(1)) << 29,

        scf_content_streams_search_by_title = (quint64(1)) << 30,
        scf_content_streams_search_by_abc   = (quint64(1)) << 31,
        scf_content_streams_search_by_tag   = (quint64(1)) << 32,
        scf_content_streams_search_by_mood  = (quint64(1)) << 33,
        scf_content_streams_search_by_genre = (quint64(1)) << 34,

        scf_content_lyrics_search_by_title  = (quint64(1)) << 35,
        scf_content_lyrics_search_by_id     = (quint64(1)) << 36 // track id and etc..
    };

    enum SourcePerms : quint64 {
        sp_none                            = 0,

        ///////////////////////// sections ////////////////////////

        sp_site_section_popular_prefer     = 1,
        sp_site_section_popular_auth       = (quint64(1)) << 1,
        sp_api_section_popular_auth        = (quint64(1)) << 2,

        sp_site_section_new_prefer         = (quint64(1)) << 3,
        sp_site_section_new_auth           = (quint64(1)) << 4,
        sp_api_section_new_auth            = (quint64(1)) << 5,

        sp_site_section_packs_prefer       = (quint64(1)) << 6,
        sp_site_section_packs_auth         = (quint64(1)) << 7,
        sp_api_section_packs_auth          = (quint64(1)) << 8,

        sp_site_section_feeds_prefer       = (quint64(1)) << 9,
        sp_site_section_feeds_auth         = (quint64(1)) << 10,
        sp_api_section_feeds_auth          = (quint64(1)) << 11,

        sp_site_section_prefer             = sp_site_section_popular_prefer | sp_site_section_new_prefer |
                                                sp_site_section_packs_prefer | sp_site_section_feeds_prefer,

        sp_site_section_auth               = sp_site_section_popular_auth | sp_site_section_new_auth |
                                                sp_site_section_packs_auth | sp_site_section_feeds_auth,

        sp_api_section_auth                = sp_api_section_popular_auth | sp_api_section_new_auth |
                                                sp_api_section_packs_auth | sp_api_section_feeds_auth,

        ///////////////////////// media ///////////////////////////

        sp_site_tracks_prefer              = (quint64(1)) << 12,
        sp_site_tracks_auth                = (quint64(1)) << 13,
        sp_api_tracks_auth                 = (quint64(1)) << 14,

        sp_site_videos_prefer              = (quint64(1)) << 15,
        sp_site_videos_auth                = (quint64(1)) << 16,
        sp_api_videos_auth                 = (quint64(1)) << 17,

        sp_site_sets_prefer                = (quint64(1)) << 18,
        sp_site_sets_auth                  = (quint64(1)) << 19,
        sp_api_sets_auth                   = (quint64(1)) << 20,

        sp_site_albums_prefer              = (quint64(1)) << 21,
        sp_site_albums_auth                = (quint64(1)) << 22,
        sp_api_albums_auth                 = (quint64(1)) << 23,

        sp_site_streams_prefer             = (quint64(1)) << 24,
        sp_site_streams_auth               = (quint64(1)) << 25,
        sp_api_streams_auth                = (quint64(1)) << 26,

        sp_site_lyrics_prefer              = (quint64(1)) << 27,
        sp_site_lyrics_auth                = (quint64(1)) << 28,
        sp_api_lyrics_auth                 = (quint64(1)) << 29,

        sp_site_photo_prefer               = (quint64(1)) << 30,
        sp_site_photo_auth                 = (quint64(1)) << 31,
        sp_api_photo_auth                  = (quint64(1)) << 32,

        sp_site_media_prefer               = sp_site_tracks_prefer | sp_site_videos_prefer | sp_site_sets_prefer |
                                                sp_site_albums_prefer | sp_site_streams_prefer | sp_site_photo_prefer | sp_site_lyrics_prefer,

        sp_site_media_auth                 = sp_site_tracks_auth | sp_site_videos_auth | sp_site_sets_auth |
                                                sp_site_albums_auth | sp_site_streams_auth | sp_site_photo_auth | sp_site_lyrics_auth,

        sp_api_media_auth                  = sp_api_tracks_auth | sp_api_videos_auth | sp_api_sets_auth |
                                                sp_api_albums_auth | sp_api_streams_auth | sp_api_photo_auth | sp_api_lyrics_auth,

        sp_site_search_media_prefer        = (quint64(1)) << 33,
        sp_site_search_media_auth          = (quint64(1)) << 34,
        sp_api_search_media_auth           = (quint64(1)) << 35,

        ///////////////////////// users and groups ////////////////
        sp_site_objects_prefer             = (quint64(1)) << 36,
        sp_site_objects_auth               = (quint64(1)) << 37,
        sp_api_objects_auth                = (quint64(1)) << 38,

        sp_site_search_objects_prefer      = (quint64(1)) << 39,
        sp_site_search_objects_auth        = (quint64(1)) << 40,
        sp_api_search_objects_auth         = (quint64(1)) << 41,

        ///////////////////////// recomendations //////////////////
        sp_site_recomendations_prefer      = (quint64(1)) << 42,
        sp_site_recomendations_auth        = (quint64(1)) << 43,
        sp_api_recomendations_auth         = (quint64(1)) << 44,

        ///////////////////////////////////////////////////////////
        sp_site_prefer                     = sp_site_section_prefer | sp_site_media_prefer | sp_site_search_media_prefer |
                                                sp_site_objects_prefer | sp_site_search_objects_prefer | sp_site_recomendations_prefer,

        sp_site_auth_mandatory             = sp_site_section_auth | sp_site_media_auth | sp_site_search_media_auth |
                                                sp_site_objects_auth | sp_site_search_objects_auth | sp_site_recomendations_auth,

        sp_api_auth_mandatory              = sp_api_section_auth | sp_api_media_auth | sp_api_search_media_auth |
                                                sp_api_objects_auth | sp_api_search_objects_auth | sp_api_recomendations_auth,

        sp_auth_mandatory                  = sp_site_auth_mandatory | sp_api_auth_mandatory,
    };

    struct SourceFlagsPack {

    };
}

#endif // ISOURCE_PERM_FLAGS
