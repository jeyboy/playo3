#include "grabber_keys.h"

namespace Core {
    namespace Web {
        extern const QString block_items                = QString::number(dmt_any_item);
        extern const QString block_items_audio          = QString::number(dmt_audio);
        extern const QString block_items_video          = QString::number(dmt_video);

        extern const QString block_sets                 = QString::number(dmt_set);
        extern const QString block_sets_audio           = QString::number(dmt_audio_set);
        extern const QString block_sets_video           = QString::number(dmt_video_set);

        extern const QString block_likes                = QLatin1String("blk_likes");
        extern const QString block_friends              = QLatin1String("blk_friends");
        extern const QString block_followings           = QLatin1String("blk_followings");
        extern const QString block_followers            = QLatin1String("blk_followers");
        extern const QString block_groups               = QLatin1String("blk_groups");

        extern const QString tkn_error                  = QLatin1String("err");
        extern const QString tkn_content                = QLatin1String("cnt");
        extern const QString tkn_source_id              = QLatin1String("sid");
        extern const QString tkn_media_type             = QLatin1String("mtype");
        extern const QString tkn_block_type             = QLatin1String("btype");
        extern const QString tkn_dir_name               = QLatin1String("dir_name");
        extern const QString tkn_amount                 = QLatin1String("amount");
        extern const QString tkn_loadable_cmd           = QLatin1String("ld_cmd");
        extern const QString tkn_more_cmd               = QLatin1String("mr_cmd");
        extern const QString tkn_cmd_parser             = QLatin1String("cmd_prsr");
        extern const QString tkn_is_empty               = QLatin1String("is_empty");
        extern const QString tkn_is_loadable            = QLatin1String("is_loadable");

        extern const QString tkn_grab_title             = QLatin1String("gtitle");
        extern const QString tkn_grab_duration          = QLatin1String("gduration");
        extern const QString tkn_grab_size              = QLatin1String("gsize");
        extern const QString tkn_grab_id                = QLatin1String("gid");
        extern const QString tkn_grab_url               = QLatin1String("gurl");
        extern const QString tkn_grab_refresh           = QLatin1String("grefresh_url");
        extern const QString tkn_grab_genre_id          = QLatin1String("ggenre_id");
        extern const QString tkn_grab_extension         = QLatin1String("gextension");
        extern const QString tkn_grab_bitrate           = QLatin1String("gbitrate");
        extern const QString tkn_grab_bpm               = QLatin1String("gbpm");
        extern const QString tkn_grab_channels          = QLatin1String("gchannels");
        extern const QString tkn_grab_discretion_rate   = QLatin1String("gdiscretion_rate");
        extern const QString tkn_grab_year              = QLatin1String("gyear");
        extern const QString tkn_grab_album             = QLatin1String("galbum");
        extern const QString tkn_grab_art_url           = QLatin1String("gart");

        extern const QString tkn_skip_info              = QLatin1String("gskip");
        extern const QString tkn_dash                   = QLatin1String(" - ");

        extern const QString val_def_extension          = QLatin1String("mp3");
        extern const QString val_login_title_postfix    = QLatin1String(" auth");

        extern const QString const_default_locale       = QLatin1String("ua");
        extern const QString const_true                 = QLatin1String("true");
        extern const QString const_false                = QLatin1String("false");
    //    extern const QString page_offset_key        = QStringLiteral("%1");
    }
}
