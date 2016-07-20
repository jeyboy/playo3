#ifndef GRABBER_KEYS
#define GRABBER_KEYS

#include <qstringbuilder.h>

#define PACKAGE_REPLACE_FRAGMENT QStringLiteral("Custom filter...")

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

        extern const QString tkn_grab_is_set;
        extern const QString tkn_grab_set_items;
        extern const QString tkn_grab_set_parser;
        extern const QString tkn_grab_set_items_amount;

        extern const QString tkn_skip_info;
        extern const QString tkn_media_type;

        extern const QString val_def_extension;
    }
}

#endif // GRABBER_KEYS
