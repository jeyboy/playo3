#ifndef DATA_SUB_TYPES
#define DATA_SUB_TYPES

namespace Core {
    enum DataSubType {
        dt_local = -999,
        dt_local_cue,

        dt_site_od = -4,
        dt_site_echonest = -3,
        dt_site_vk = -2,
        dt_site_sc = -1,
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
