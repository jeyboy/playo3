#include "soundcloud_keys.h"


namespace Core {
    namespace Web {
        namespace Soundcloud {
            extern const QString url_site_base         = QLatin1String("https://soundcloud.com/");
            extern const QString url_connect           = url_site_base % QLatin1String("connect");

            extern const QString url_api_base          = QLatin1String("https://api.soundcloud.com/");
            extern const QString url_auth_token        = url_api_base % QLatin1String("oauth2/token");
            extern const QString url_auth_confirm      = url_api_base % QLatin1String("me.json?oauth_token=");
            extern const QString url_redirect          = QLatin1String("http://sos.com");

            extern const QString url_api_base2         = QLatin1String("https://api-v2.soundcloud.com/");


            extern const QString path_playlists        = QLatin1String("playlists/");
            extern const QString path_tracks           = QLatin1String("tracks");
            extern const QString path_related_tracks   = path_tracks % QLatin1String("/%1/related");
            extern const QString path_track_favorites  = path_tracks % QLatin1String("/%1/favorites");
            extern const QString path_groups           = QLatin1String("groups");
            extern const QString path_group_tracks     = path_groups % QLatin1String("/%1/tracks");
            //            /groups/{id}/members
            extern const QString path_users            = QLatin1String("users");
            extern const QString path_user_tracks      = path_users % QLatin1String("/%1/tracks");
            extern const QString path_user_favorites   = path_users % QLatin1String("/%1/favorites");
            extern const QString path_user_playlists   = path_users % QLatin1String("/%1/playlists");
            extern const QString path_user_followings  = path_users % QLatin1String("/%1/followings");
            extern const QString path_user_followers   = path_users % QLatin1String("/%1/followers");
            extern const QString path_user_groups      = path_users % QLatin1String("/%1/groups");

            extern const QString tkn_avatar_url        = QLatin1String("avatar_url");
            extern const QString tkn_artwork_url       = QLatin1String("artwork_url");
            extern const QString tkn_permalink         = QLatin1String("permalink");
            extern const QString tkn_grant_type        = QLatin1String("grant_type");
            extern const QString tkn_client_secret     = QLatin1String("client_secret");
            extern const QString tkn_referer           = QLatin1String("Referer");
            extern const QString tkn_access_token      = QLatin1String("access_token");
            extern const QString tkn_code              = QLatin1String("code");
            extern const QString tkn_error_description = QLatin1String("error_description");
            extern const QString tkn_src               = QLatin1String("src");
            extern const QString tkn_username          = QLatin1String("username");
            extern const QString tkn_password          = QLatin1String("password");
            extern const QString tkn_recaptcha_challenge_field
                                                       = QLatin1String("recaptcha_challenge_field");
            extern const QString tkn_recaptcha_response_field
                                                       = QLatin1String("recaptcha_response_field");
            extern const QString tkn_response_type     = QLatin1String("response_type");
            extern const QString tkn_redirect_uri      = QLatin1String("redirect_uri");
            extern const QString tkn_scope             = QLatin1String("scope");
            extern const QString tkn_display           = QLatin1String("display");
            extern const QString tkn_client_id         = QLatin1String("client_id=");
            extern const QString tkn_response          = QLatin1String("response");
            extern const QString tkn_error             = QLatin1String("error");
            extern const QString tkn_errors            = QLatin1String("errors");
            extern const QString tkn_error_message     = QLatin1String("error_message");
            extern const QString tkn_error_code        = QLatin1String("error_code");
            extern const QString tkn_offset            = QLatin1String("offset");
            extern const QString tkn_limit             = QLatin1String("limit");
            extern const QString tkn_tracks            = QLatin1String("tracks");
            extern const QString tkn_id                = QLatin1String("id");
            extern const QString tkn_title             = QLatin1String("title");
            extern const QString tkn_name              = QLatin1String("name");
            extern const QString tkn_full_name         = QLatin1String("full_name");
            extern const QString tkn_user_id           = QLatin1String("user_id");
            extern const QString tkn_download_url      = QLatin1String("download_url");
            extern const QString tkn_stream_url        = QLatin1String("stream_url");
            extern const QString tkn_video_url         = QLatin1String("video_url");
            extern const QString tkn_original_format   = QLatin1String("original_format");
            extern const QString tkn_duration          = QLatin1String("duration");
            extern const QString tkn_genre_id          = QLatin1String("genre_id");
            extern const QString tkn_types             = QLatin1String("types");
            extern const QString tkn_q                 = QLatin1String("q");
            extern const QString tkn_ids               = QLatin1String("ids");
            extern const QString tkn_genres            = QLatin1String("genres");
            extern const QString tkn_order             = QLatin1String("order");
            extern const QString tkn_tag_list          = QLatin1String("tag_list");


            extern const QString val_station_head_tkn  = QLatin1String("soundcloud:artist-stations:");
            extern const QString val_client_tkn        = QLatin1String("54ca588303e1d2bf524509faf20931b4");
            extern const QString val_hotness_order     = QLatin1String("hotness");
            extern const QString val_created_at_order  = QLatin1String("created_at");
            extern const QString val_audio_types       = QLatin1String("original,remix,live,podcast");
            extern const QString val_audio_org_types   = QLatin1String("original,live");
            extern const QString val_audio_rmx_types   = QLatin1String("remix");
            extern const QString val_name              = QLatin1String("Soundcloud");
            extern const QString val_default_format    = QLatin1String(".json");
            extern const QString val_id_tkn            = QLatin1String("8f84790a84f5a5acd1c92e850b5a91b7");
            extern const QString val_response_type     = QLatin1String("code");
            extern const QString val_scope             = QLatin1String("non-expiring");
            extern const QString val_display           = QLatin1String("popup");
            extern const QString val_grant_type        = QLatin1String("authorization_code");
        }
    }
}
