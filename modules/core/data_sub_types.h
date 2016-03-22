#ifndef DATA_SUB_TYPES
#define DATA_SUB_TYPES

namespace Core {
    enum DataSubType {
        dt_playlist = -99,
        dt_playlist_local,
        dt_playlist_cue,

        dt_playlist_sc = -59,
        dt_playlist_vk,
        dt_playlist_od,

        dt_none = 0,

        dt_local,
        dt_local_cue,

        dt_web = 50,

        dt_site_od,
        dt_site_echonest,
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
