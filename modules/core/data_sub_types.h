#ifndef DATA_SUB_TYPES
#define DATA_SUB_TYPES

#include <qobject.h>

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

        //reserved 4
        //reserved 8
        //reserved 16
        //reserved 32
        //reserved 64
        //reserved 128
        //reserved 256
        dmt_feed = 512,
        dmt_artist = 1024,

        dmt_user = 2048,
        dmt_group = 4096,

        dmt_set = 8192,

        dmt_audio_set = dmt_set | dmt_audio,
        dmt_video_set = dmt_set | dmt_video,

        dmt_any_item = dmt_audio | dmt_video,
        dmt_any = dmt_set | dmt_any_item
    };

    #define EXTRACT_MEDIA_TYPE(val) (DataMediaType)(val & 3)

    enum DataSubType : int {
        dt_level                    = -9000,
        dt_level_tree               = -8000,
        dt_tree                     = -7000,
        dt_search                   = -6000,

        dt_none                     = 0,

        dt_web                      = 1,
        dt_local                    = 2,
        dt_playlist                 = 4,

        dt_playlist_local           = dt_local | dt_playlist,

        dt_dummy                    = 10,

        dt_cue                      = 20,
        dt_local_cue                = dt_cue | dt_local,
        dt_web_cue                  = dt_cue | dt_web,
        dt_playlist_cue             = dt_cue | dt_playlist,

        dt_web_vk                   = 30 | dt_web,
        dt_playlist_vk              = dt_web_vk | dt_playlist,

        dt_web_od                   = 40 | dt_web,
        dt_playlist_od              = dt_web_od | dt_playlist,

        dt_web_sc                   = 50 | dt_web,
        dt_playlist_sc              = dt_web_sc | dt_playlist,

        dt_web_myzuka               = 60 | dt_web,
        dt_web_fourshared           = 70 | dt_web,
        dt_web_zaycev               = 80 | dt_web,
        dt_web_mp3base              = 90 | dt_web,
        dt_web_promodj              = 100 | dt_web,
        dt_web_mp3cc                = 110 | dt_web,
        dt_web_mp3pm                = 120 | dt_web,
        dt_web_shmidt               = 130 | dt_web,
        dt_web_jetune               = 140 | dt_web,
        dt_web_music_shara          = 150 | dt_web,
        dt_web_redmp3               = 160 | dt_web,
        dt_web_yandex               = 170 | dt_web,
        dt_web_youtube              = 180 | dt_web
    };
}

#endif // DATA_SUB_TYPES
