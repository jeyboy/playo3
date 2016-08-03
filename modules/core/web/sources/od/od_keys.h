#ifndef OD_KEYS
#define OD_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Od {
            extern const QString url_root;
            extern const QString url_base_auth;
            extern const QString url_base_audio;


            extern const QString path_sid;
            extern const QString path_auth;
            extern const QString path_auth2;

            extern const QString path_audio_init;
            extern const QString path_audio_play;
            extern const QString path_audio_play_30_sec;
            extern const QString path_audio_info;
            extern const QString path_audio_history;
            extern const QString path_audio_popular;
            extern const QString path_audio_popular_tracks;
            extern const QString path_audio_add_to_playlist_form;
            extern const QString path_audio_radio;
            extern const QString path_audio_tuners_for_radio;
            extern const QString path_audio_is_downloaded;
            extern const QString path_audio_downloaded;
            extern const QString path_audio_collections;
            extern const QString path_audio_by_artist_id;
            extern const QString path_audio_by_album_id;
            extern const QString path_audio_search_albums;
            extern const QString path_audio_search_artists;
            extern const QString path_audio_search_tracks;
            extern const QString path_audio_search;

            extern const QString path_audio_playlist_add;
            extern const QString path_audio_playlist_del;
            extern const QString path_audio_playlists;
            extern const QString path_audio_to_playlist;
            extern const QString path_audio_del_from_playlist;


            extern const QString tkn_ids;
            extern const QString tkn_uid;
            extern const QString tkn_pid;
            extern const QString tkn_tid;
            extern const QString tkn_q;

            extern const QString tkn_me;
            extern const QString tkn_my;
            extern const QString tkn_friends;
            extern const QString tkn_artists;
            extern const QString tkn_tracks;
            extern const QString tkn_albums;
            extern const QString tkn_playlists;
            extern const QString tkn_coma_dot;
            extern const QString tkn_authcode;
            extern const QString tkn_httpsdata;
            extern const QString tkn_form_error;
            extern const QString tkn_client_hash;
            extern const QString tkn_md5;
            extern const QString tkn_play;
            extern const QString tkn_captcha;
            extern const QString tkn_secret;
            extern const QString tkn_code;
            extern const QString tkn_sid;
            extern const QString tkn_header_user_agent;
            extern const QString tkn_error;
            extern const QString tkn_offset;
            extern const QString tkn_limit; // separate key for request limit
            extern const QString tkn_count; // separate key for json vakue
            extern const QString tkn_chunk;
            extern const QString tkn_jsessionid;

            extern const QString tkn_dash;
            extern const QString tkn_full_name;
            extern const QString tkn_id;
            extern const QString tkn_owner;
            extern const QString tkn_name;
            extern const QString tkn_duration;
            extern const QString tkn_size;
            extern const QString tkn_ensemble;
            extern const QString tkn_art_url;
            extern const QString tkn_perma;

            extern const QString val_login_title;
            extern const QString val_resend_sms_title;
            extern const QString val_sms_code_title;
            extern const QString val_name;
        }
    }
}

#endif // OD_KEYS
