#include "api_keys.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            extern const QString url_site_base         = QStringLiteral("https://soundcloud.com/");
            extern const QString url_connect           = url_site_base % QStringLiteral("connect");

            extern const QString url_api_base          = QStringLiteral("https://api.soundcloud.com/");
            extern const QString url_auth_token        = url_api_base % QStringLiteral("oauth2/token");
            extern const QString url_auth_confirm      = url_api_base % QStringLiteral("me.json?oauth_token=");
            extern const QString url_redirect          = QStringLiteral("http://sos.com");

            extern const QString url_api_base2         = QStringLiteral("https://api-v2.soundcloud.com/");


            extern const QString path_playlists        = QStringLiteral("playlists/");
            extern const QString path_tracks           = QStringLiteral("tracks");
            extern const QString path_related_tracks   = path_tracks % QStringLiteral("/%1/related");
            extern const QString path_groups           = QStringLiteral("groups");
            extern const QString path_group_tracks     = path_groups % QStringLiteral("/%1/tracks");
            //            /groups/{id}/members
            extern const QString path_users            = QStringLiteral("users");
            extern const QString path_user_tracks      = path_users % QStringLiteral("/%1/tracks");
            extern const QString path_user_favorites   = path_users % QStringLiteral("/%1/favorites");
            extern const QString path_user_playlists   = path_users % QStringLiteral("/%1/playlists");
            extern const QString path_user_followings  = path_users % QStringLiteral("/%1/followings");
            extern const QString path_user_followers   = path_users % QStringLiteral("/%1/followers");
            extern const QString path_user_groups      = path_users % QStringLiteral("/%1/groups");

            extern const QString tkn_avatar_url        = QStringLiteral("avatar_url");
            extern const QString tkn_artwork_url       = QStringLiteral("artwork_url");
            extern const QString tkn_permalink         = QStringLiteral("permalink");
            extern const QString tkn_grant_type        = QStringLiteral("grant_type");
            extern const QString tkn_client_secret     = QStringLiteral("client_secret");
            extern const QString tkn_referer           = QStringLiteral("Referer");
            extern const QString tkn_access_token      = QStringLiteral("access_token");
            extern const QString tkn_code              = QStringLiteral("code");
            extern const QString tkn_error_description = QStringLiteral("error_description");
            extern const QString tkn_src               = QStringLiteral("src");
            extern const QString tkn_username          = QStringLiteral("username");
            extern const QString tkn_password          = QStringLiteral("password");
            extern const QString tkn_recaptcha_challenge_field
                                                       = QStringLiteral("recaptcha_challenge_field");
            extern const QString tkn_recaptcha_response_field
                                                       = QStringLiteral("recaptcha_response_field");
            extern const QString tkn_response_type     = QStringLiteral("response_type");
            extern const QString tkn_redirect_uri      = QStringLiteral("redirect_uri");
            extern const QString tkn_scope             = QStringLiteral("scope");
            extern const QString tkn_display           = QStringLiteral("display");
            extern const QString tkn_client_id         = QStringLiteral("client_id=");
            extern const QString tkn_response          = QStringLiteral("response");
            extern const QString tkn_error             = QStringLiteral("error");
            extern const QString tkn_errors            = QStringLiteral("errors");
            extern const QString tkn_error_message     = QStringLiteral("error_message");
            extern const QString tkn_error_code        = QStringLiteral("error_code");
            extern const QString tkn_offset            = QStringLiteral("offset");
            extern const QString tkn_limit             = QStringLiteral("limit");
            extern const QString tkn_audio_list        = QStringLiteral("audio_list");
            extern const QString tkn_playlist          = QStringLiteral("playlists");
            extern const QString tkn_followings        = QStringLiteral("followings");
            extern const QString tkn_followers         = QStringLiteral("followers");
            extern const QString tkn_groups            = QStringLiteral("groups");
            extern const QString tkn_tracks            = QStringLiteral("tracks");
            extern const QString tkn_id                = QStringLiteral("id");
            extern const QString tkn_title             = QStringLiteral("title");
            extern const QString tkn_name              = QStringLiteral("name");
            extern const QString tkn_full_name         = QStringLiteral("full_name");
            extern const QString tkn_user_id           = QStringLiteral("user_id");
            extern const QString tkn_download_url      = QStringLiteral("download_url");
            extern const QString tkn_stream_url        = QStringLiteral("stream_url");
            extern const QString tkn_video_url         = QStringLiteral("video_url");
            extern const QString tkn_original_format   = QStringLiteral("original_format");
            extern const QString tkn_duration          = QStringLiteral("duration");
            extern const QString tkn_genre_id          = QStringLiteral("genre_id");
            extern const QString tkn_types             = QStringLiteral("types");
            extern const QString tkn_q                 = QStringLiteral("q");
            extern const QString tkn_ids               = QStringLiteral("ids");
            extern const QString tkn_genres            = QStringLiteral("genres");
            extern const QString tkn_order             = QStringLiteral("order");


            extern const QString val_client_tkn        = QStringLiteral("54ca588303e1d2bf524509faf20931b4");
            extern const QString val_hotness_order     = QStringLiteral("hotness");
            extern const QString val_created_at_order  = QStringLiteral("created_at");
            extern const QString val_audio_types       = QStringLiteral("original,remix,live,podcast");
            extern const QString val_audio_org_types   = QStringLiteral("original,live");
            extern const QString val_audio_rmx_types   = QStringLiteral("remix");
            extern const QString val_auth_title        = QStringLiteral("Soundcloud auth");
            extern const QString val_name              = QStringLiteral("Soundcloud");
            extern const QString val_default_format    = QStringLiteral(".json");
            extern const QString val_id_tkn            = QStringLiteral("8f84790a84f5a5acd1c92e850b5a91b7");
            extern const QString val_response_type     = QStringLiteral("code");
            extern const QString val_scope             = QStringLiteral("non-expiring");
            extern const QString val_display           = QStringLiteral("popup");
            extern const QString val_grant_type        = QStringLiteral("authorization_code");
        }
    }
}
