#ifndef VK_KEYS
#define VK_KEYS

#include <qstringbuilder.h>

#include "modules/core/web/grabber_keys.h"

#define VK_DEFAULT_OFFSET_TEMPLATE QLatin1String("ZZZZ")

namespace Core {
    namespace Web {
        namespace Vk {
            extern const QString tkn_version;
            extern const QString tkn_display;
            extern const QString tkn_client_id;
            extern const QString tkn_response_type;
            extern const QString tkn_scope;
            extern const QString tkn_redirect_uri;
            extern const QString tkn_access_token;
            extern const QString tkn_offset;
            extern const QString tkn_limit;
            extern const QString tkn_response;
            extern const QString tkn_finished;
            extern const QString tkn_user_id;
            extern const QString tkn_expires_in;
            extern const QString tkn_error;
            extern const QString tkn_error_description;
            extern const QString tkn_error_msg;
            extern const QString tkn_error_code;
            extern const QString tkn_captcha_img;
            extern const QString tkn_captcha_sid;
            extern const QString tkn_captcha;
            extern const QString tkn_execute;
            extern const QString tkn_code;
//            extern const QString tkn_albums;
//            extern const QString tkn_video_albums;
//            extern const QString tkn_audios;
//            extern const QString tkn_audio_list;
//            extern const QString tkn_video_list;
//            extern const QString tkn_groups;
//            extern const QString tkn_friends;
            extern const QString tkn_albums_offset;
            extern const QString tkn_albums_finished;
            extern const QString tkn_url;
            extern const QString tkn_id;
            extern const QString tkn_owner_id;
            extern const QString tkn_artist;
            extern const QString tkn_title;
            extern const QString tkn_duration;
            extern const QString tkn_genre_id;
            extern const QString tkn_fields;
            extern const QString tkn_counters;
            extern const QString tkn_user_ids;
            extern const QString tkn_screen_name;
            extern const QString tkn_object_id;
            extern const QString tkn_type;
            extern const QString tkn_lyrics_id;
            extern const QString tkn_email;
            extern const QString tkn_password;
            extern const QString tkn_date;
            extern const QString tkn_items;
            extern const QString tkn_time_sep;
            extern const QString tkn_q;
            extern const QString tkn_photo;
            extern const QString tkn_group_ids;
            extern const QString tkn_domain;
            extern const QString tkn_video_art;
            extern const QString tkn_player;


            extern const QString url_redirect_uri;
            extern const QString url_api_base;
            extern const QString url_site_base;
            extern const QString url_auth;


            extern const QString path_groups_by_id;
            extern const QString path_groups_search;
            extern const QString path_users_search;
            extern const QString path_user_info;
            extern const QString path_resole_user;
            extern const QString path_lyrics;


            extern const QString val_str_true;
            extern const QString val_str_false;
            extern const QString val_name;
            extern const QString val_api_call_limit;
            extern const QString val_version;
            extern const QString val_page_token;
            extern const QString val_client_token;
            extern const QString val_response_token;
            extern const QString val_scope;
            extern const QString val_user_fields;
            extern const QString val_group_types;


            extern const QString query_track_playlists(const QString & owner_id);
            extern const QString query_video_playlists(const QString & owner_id);

            extern const QString query_user_videos(const QString & owner_id);

            extern const QString query_user_groups_friends(const QString & owner_id);
            extern const QString query_user_tracks_groups_friends(const QString & owner_id);
            extern const QString query_user_tracks_playlists_groups_friends(const QString & owner_id);
            extern const QString query_user_tracks_playlists(const QString & owner_id);
        }
    }
}

#endif // VK_KEYS
