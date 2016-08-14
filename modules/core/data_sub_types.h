#ifndef DATA_SUB_TYPES
#define DATA_SUB_TYPES

namespace Core {
    enum RecType {
        rec_none = 0,
        rec_user,
        rec_song,
        rec_set
    };

    enum DataMediaType : int {
        dmt_unknow = 0,

        dmt_audio = 1,
        dmt_video = 2,

        dmt_artists = 4,
        dmt_groups = 8,

        dmt_set = 128,

        dmt_audio_set = dmt_set | dmt_audio,
        dmt_video_set = dmt_set | dmt_video,

        dmt_any_item = dmt_audio | dmt_video,
        dmt_any = dmt_set | dmt_any_item
    };

    #define EXTRACT_MEDIA_TYPE(val) (DataMediaType)(val & 3)

    enum DataSubType : int {
        dt_level = -199,
        dt_level_tree,
        dt_tree,

        dt_search = -159,

        dt_playlist = -99,
        dt_playlist_local,
        dt_playlist_cue,

        dt_playlist_sc = -59,
        dt_playlist_vk,
        dt_playlist_od,

        dt_none = 0,

        dt_local,
        dt_local_cue,

        dt_dummy,

        dt_web = 50,

        dt_site_od,
        dt_site_vk,
        dt_site_sc,

        dt_site_myzuka = dt_web + 50,
        dt_site_fourshared,
        dt_site_zaycev,
        dt_site_mp3base,
        dt_site_promodj,
        dt_site_mp3cc,
        dt_site_mp3pm,
        dt_site_shmidt,
        dt_site_jetune,
        dt_site_music_shara,
        dt_site_redmp3,
        dt_site_yandex,
        dt_site_youtube
    };
}

#endif // DATA_SUB_TYPES
