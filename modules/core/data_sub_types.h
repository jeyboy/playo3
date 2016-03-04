#ifndef DATA_SUB_TYPES
#define DATA_SUB_TYPES

namespace Core {
    enum DataSubType {
        dt_playlist_local = -9999,
        dt_playlist_cue,

        dt_platlist_soundcloud = -8999,
        dt_playlist_vk,
        dt_playlist_od,

        dt_local = -999,
        dt_local_cue,

        dt_site_od = -99,
        dt_site_echonest,
        dt_site_vk,
        dt_site_sc,

        dt_none = 0,

        dt_site_myzuka,
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
