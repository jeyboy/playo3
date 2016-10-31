#include "grabber_keys.h"

namespace Core {
    namespace Web {
        extern const QString block_items                = QString::number(dmt_any_item);
        extern const QString block_items_audio          = QString::number(dmt_audio);
        extern const QString block_items_video          = QString::number(dmt_video);

        extern const QString block_sets                 = QString::number(dmt_set);
        extern const QString block_sets_audio           = QString::number(dmt_audio_set);
        extern const QString block_sets_video           = QString::number(dmt_video_set);

        extern const QString block_likes                = QStringLiteral("blk_likes");
        extern const QString block_friends              = QStringLiteral("blk_friends");
        extern const QString block_followings           = QStringLiteral("blk_followings");
        extern const QString block_followers            = QStringLiteral("blk_followers");
        extern const QString block_groups               = QStringLiteral("blk_groups");

        extern const QString tkn_error                  = QStringLiteral("err");
        extern const QString tkn_content                = QStringLiteral("cnt");
        extern const QString tkn_source_id              = QStringLiteral("sid");
        extern const QString tkn_media_type             = QStringLiteral("mtype");
        extern const QString tkn_block_type             = QStringLiteral("btype");
        extern const QString tkn_dir_name               = QStringLiteral("dir_name");
        extern const QString tkn_amount                 = QStringLiteral("amount");
        extern const QString tkn_loadable_cmd           = QStringLiteral("ld_cmd");
        extern const QString tkn_more_cmd               = QStringLiteral("mr_cmd");
        extern const QString tkn_cmd_parser             = QStringLiteral("cmd_prsr");
        extern const QString tkn_is_empty               = QStringLiteral("is_empty");

        extern const QString tkn_grab_title             = QStringLiteral("gtitle");
        extern const QString tkn_grab_duration          = QStringLiteral("gduration");
        extern const QString tkn_grab_size              = QStringLiteral("gsize");
        extern const QString tkn_grab_id                = QStringLiteral("gid");
        extern const QString tkn_grab_url               = QStringLiteral("gurl");
        extern const QString tkn_grab_refresh           = QStringLiteral("grefresh_url");
        extern const QString tkn_grab_genre_id          = QStringLiteral("ggenre_id");
        extern const QString tkn_grab_extension         = QStringLiteral("gextension");
        extern const QString tkn_grab_bitrate           = QStringLiteral("gbitrate");
        extern const QString tkn_grab_bpm               = QStringLiteral("gbpm");
        extern const QString tkn_grab_channels          = QStringLiteral("gchannels");
        extern const QString tkn_grab_discretion_rate   = QStringLiteral("gdiscretion_rate");
        extern const QString tkn_grab_year              = QStringLiteral("gyear");
        extern const QString tkn_grab_album             = QStringLiteral("galbum");
        extern const QString tkn_grab_art_url           = QStringLiteral("gart");

        extern const QString tkn_skip_info              = QStringLiteral("gskip");
        extern const QString tkn_dash                   = QStringLiteral(" - ");

        extern const QString val_def_extension          = QStringLiteral("mp3");
        extern const QString val_login_title_postfix    = QStringLiteral(" auth");

        extern const QString const_true                 = QStringLiteral("true");
        extern const QString const_false                = QStringLiteral("false");
    //    extern const QString page_offset_key        = QStringLiteral("%1");
    }
}
