#ifndef DATA_SUB_TYPES
#define DATA_SUB_TYPES

namespace Core {
    enum RecType {
        rec_none = 0,
        rec_user_audio,
        rec_user_video,
        rec_audio,
        rec_set,
        rec_tags,
        rec_artist,
        rec_artist_audio,
        rec_artist_album,
        rec_label_audio,
        rec_label_album,
    };

    enum DataMediaType : int {
        dmt_unknow = 0,

        dmt_audio = 1,
        dmt_video = 2,
        dmt_dir = 4,

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
        dmt_dir_set = dmt_set | dmt_dir,

        dmt_audio_cat = dmt_audio_set | dmt_dir,
        dmt_video_cat = dmt_video_set | dmt_dir,

        dmt_any_item = dmt_audio | dmt_video,
        dmt_any = dmt_set | dmt_any_item
    };

    #define EXTRACT_MEDIA_TYPE(val) (DataMediaType)(val & 3)

    #define DATA_SUB_TYPE_BASE 10
    enum DataSubType : int {
        dt_level                    = -9000,
        dt_level_tree               = -8000,
        dt_tree                     = -7000,
        dt_search                   = -1000,

        dt_none                     = 0,

        dt_web                      = 1,
        dt_local                    = 2,
        dt_playlist                 = 4,
        dt_stream                   = 8,

        dt_location                 = dt_web | dt_local,

        dt_playlist_local           = dt_local | dt_playlist,

        dt_dummy                    = DATA_SUB_TYPE_BASE,

        dt_cue                      = DATA_SUB_TYPE_BASE * 2,
        dt_local_cue                = dt_cue | dt_local,
        dt_web_cue                  = dt_cue | dt_web,
        dt_playlist_cue             = dt_cue | dt_playlist,

        dt_web_vk                   = DATA_SUB_TYPE_BASE * 3 | dt_web,
        dt_playlist_vk              = dt_web_vk | dt_playlist,

        dt_web_od                   = DATA_SUB_TYPE_BASE * 4 | dt_web,
        dt_playlist_od              = dt_web_od | dt_playlist,

        dt_web_sc                   = DATA_SUB_TYPE_BASE * 5 | dt_web,
        dt_playlist_sc              = dt_web_sc | dt_playlist,

        dt_web_myzuka               = DATA_SUB_TYPE_BASE * 6 | dt_web,
        dt_web_fourshared           = DATA_SUB_TYPE_BASE * 7 | dt_web,
        dt_web_zaycev               = DATA_SUB_TYPE_BASE * 8 | dt_web,
        dt_web_mp3base              = DATA_SUB_TYPE_BASE * 9 | dt_web,
        dt_web_promodj              = DATA_SUB_TYPE_BASE * 10 | dt_web,
        dt_web_mp3cc                = DATA_SUB_TYPE_BASE * 11 | dt_web,
        dt_web_mp3pm                = DATA_SUB_TYPE_BASE * 12 | dt_web,
        dt_web_shmidt               = DATA_SUB_TYPE_BASE * 13 | dt_web,
        dt_web_jetune               = DATA_SUB_TYPE_BASE * 14 | dt_web,
        dt_web_music_shara          = DATA_SUB_TYPE_BASE * 15 | dt_web,
        dt_web_redmp3               = DATA_SUB_TYPE_BASE * 16 | dt_web,
        dt_web_yandex               = DATA_SUB_TYPE_BASE * 17 | dt_web,
        dt_web_youtube              = DATA_SUB_TYPE_BASE * 18 | dt_web
    };

    #define DST_FLAGS(dst)  (dst % DATA_SUB_TYPE_BASE)

    #define DST_HAS_FLAG(dst, flag) (DST_FLAGS(dst) & flag)
    #define DST_EXTRACT_FLAGS(dst) (Core::DataSubType)((dst - DST_FLAGS(dst)) | dst & Core::dt_location)
    #define DST_EXTRACT_FLAG(dst, flag) (Core::DataSubType)(dst - DST_HAS_FLAG(dst, flag))
    #define DST_APPEND_FLAG(dst, flag) DST_HAS_FLAG(dst, flag) ? dst : (Core::DataSubType)(dst + flag)

    #define DST_SET_FOLDER(dst) DST_APPEND_FLAG(dst, dt_playlist)

    #define DST_IS_WEB(dst) DST_FLAGS(dst) & dt_web
    #define DST_IS_LOCAL(dst) DST_FLAGS(dst) & dt_local
    #define DST_IS_PLAYLIST(dst) DST_FLAGS(dst) & dt_playlist
    #define DST_IS_STREAM(dst) DST_FLAGS(dst) & dt_stream
}

#endif // DATA_SUB_TYPES
