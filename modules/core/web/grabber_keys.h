#ifndef GRABBER_KEYS
#define GRABBER_KEYS

#include <qstringbuilder.h>

#include "modules/core/data_sub_types.h"

#define PACKAGE_REPLACE_FRAGMENT_TITLE QStringLiteral("Custom filter...")
#define PACKAGE_REPLACE_FRAGMENT QStringLiteral("%")

#define EXTRACT_ITEMS(json) json.value(tkn_content).toArray()

namespace Core {
    namespace Web {
        extern const QString block_items; // mixed items

        extern const QString block_items_audio; // only audio items
        extern const QString block_items_video; // only video items

        extern const QString block_sets;  // mixed sets

        extern const QString block_sets_audio;  // only audio sets
        extern const QString block_sets_video;  // only video sets

        extern const QString block_likes;
        extern const QString block_friends;
        extern const QString block_followings;
        extern const QString block_followers;
        extern const QString block_groups;

        extern const QString tkn_error;
        extern const QString tkn_content;
        extern const QString tkn_source_id;
        extern const QString tkn_media_type;
        extern const QString tkn_block_type;
        extern const QString tkn_dir_name;
        extern const QString tkn_amount;
        extern const QString tkn_loadable_cmd;
        extern const QString tkn_more_cmd;
        extern const QString tkn_cmd_parser;
        extern const QString tkn_is_empty;

        extern const QString tkn_grab_title;
        extern const QString tkn_grab_duration;
        extern const QString tkn_grab_size;
        extern const QString tkn_grab_id;
        extern const QString tkn_grab_url;
        extern const QString tkn_grab_refresh;
        extern const QString tkn_grab_genre_id;
        extern const QString tkn_grab_extension;
        extern const QString tkn_grab_bitrate;
        extern const QString tkn_grab_bpm;
        extern const QString tkn_grab_channels;
        extern const QString tkn_grab_discretion_rate;
        extern const QString tkn_grab_year;
        extern const QString tkn_grab_album;
        extern const QString tkn_grab_art_url;

        extern const QString tkn_skip_info;
        extern const QString tkn_dash;

        extern const QString val_def_extension;
    }
}

#endif // GRABBER_KEYS
