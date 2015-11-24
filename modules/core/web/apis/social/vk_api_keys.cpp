#include "vk_api_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            extern const QString tkn_name              = QStringLiteral("Vk");
            extern const QString tkn_auth_title        = QStringLiteral("Vk auth");
            extern const QString tkn_str_true          = QStringLiteral("1");
            extern const QString tkn_str_false         = QStringLiteral("0");
            extern const QString tkn_version           = QStringLiteral("v");
            extern const QString tkn_display           = QStringLiteral("display");
            extern const QString tkn_client_id         = QStringLiteral("client_id");
            extern const QString tkn_response_type     = QStringLiteral("response_type");
            extern const QString tkn_scope             = QStringLiteral("scope");
            extern const QString tkn_redirect_uri      = QStringLiteral("redirect_uri");
            extern const QString tkn_access_token      = QStringLiteral("access_token");
            extern const QString tkn_offset            = QStringLiteral("offset");
            extern const QString tkn_limit             = QStringLiteral("count");
            extern const QString tkn_response          = QStringLiteral("response");
            extern const QString tkn_finished          = QStringLiteral("finished");
            extern const QString tkn_user_id           = QStringLiteral("user_id");
            extern const QString tkn_expires_in        = QStringLiteral("expires_in");
            extern const QString tkn_error             = QStringLiteral("error");
            extern const QString tkn_error_description = QStringLiteral("error_description_key");
            extern const QString tkn_error_msg         = QStringLiteral("error_msg");
            extern const QString tkn_error_code        = QStringLiteral("error_code");
            extern const QString tkn_captcha_img       = QStringLiteral("captcha_img");
            extern const QString tkn_captcha_sid       = QStringLiteral("captcha_sid");
            extern const QString tkn_captcha           = QStringLiteral("captcha_key");
            extern const QString tkn_execute           = QStringLiteral("execute");
            extern const QString tkn_code              = QStringLiteral("code");
            extern const QString tkn_albums            = QStringLiteral("albums");
            extern const QString tkn_audio_list        = QStringLiteral("audio_list");
            extern const QString tkn_groups            = QStringLiteral("groups");
            extern const QString tkn_friends           = QStringLiteral("friends");
            extern const QString tkn_albums_offset     = QStringLiteral("albums_offset");
            extern const QString tkn_albums_finished   = QStringLiteral("albums_finished");
            extern const QString tkn_url               = QStringLiteral("url");
            extern const QString tkn_id                = QStringLiteral("id");
            extern const QString tkn_owner_id          = QStringLiteral("owner_id");
            extern const QString tkn_artist            = QStringLiteral("artist");
            extern const QString tkn_title             = QStringLiteral("title");
            extern const QString tkn_duration          = QStringLiteral("duration");
            extern const QString tkn_genre_id          = QStringLiteral("genre_id");
            extern const QString tkn_fields            = QStringLiteral("fields");
            extern const QString tkn_counters          = QStringLiteral("counters");
            extern const QString tkn_user_ids          = QStringLiteral("user_ids");
            extern const QString tkn_screen_name       = QStringLiteral("screen_name");
            extern const QString tkn_object_id         = QStringLiteral("object_id");
            extern const QString tkn_type              = QStringLiteral("type");
            extern const QString tkn_lyrics_id         = QStringLiteral("lyrics_id");
            extern const QString tkn_email             = QStringLiteral("email");
            extern const QString tkn_password          = QStringLiteral("pass");
            extern const QString tkn_predef1           = QStringLiteral("_1_");
            extern const QString tkn_predef2           = QStringLiteral("_2_");

            extern const QString url_redirect_uri      = QStringLiteral("https://oauth.vk.com/blank.html");
            extern const QString url_base              = QStringLiteral("https://api.vk.com/method/");
            extern const QString url_auth              = QStringLiteral("https://oauth.vk.com/authorize");

            extern const QString path_user_info        = QStringLiteral("users.get");
            extern const QString path_resole_user      = QStringLiteral("utils.resolveScreenName");
            extern const QString path_lyrics           = QStringLiteral("audio.getLyrics");

            extern const QString val_api_call_limit    = QStringLiteral("20");
            extern const QString val_version           = QStringLiteral("5.21");
            extern const QString val_page_token        = QStringLiteral("page");
            extern const QString val_client_token      = QStringLiteral("4332211");
            extern const QString val_response_token    = QStringLiteral("token");
            extern const QString val_scope             = QStringLiteral("audio,video,friends,groups,offline");





//            extern const QString name_key              = QStringLiteral("Vk");
//            extern const QString auth_title_key        = QStringLiteral("Vk auth");

//            extern const QString bool_str_true         = QStringLiteral("1");
//            extern const QString bool_str_false        = QStringLiteral("0");

//            extern const QString base_url              = QStringLiteral("https://api.vk.com/method/");
//            extern const QString auth_url              = QStringLiteral("https://oauth.vk.com/authorize");

//            extern const QString user_info_path        = QStringLiteral("users.get");
//            extern const QString resole_user_path      = QStringLiteral("utils.resolveScreenName");
//            extern const QString lyrics_path           = QStringLiteral("audio.getLyrics");

//            extern const QString api_call_limit_val    = QStringLiteral("20");

//            extern const QString version_key           = QStringLiteral("v");
//            extern const QString version_val           = QStringLiteral("5.21");

//            extern const QString display_token         = QStringLiteral("display");
//            extern const QString page_token            = QStringLiteral("page");

//            extern const QString client_id_token       = QStringLiteral("client_id");
//            extern const QString client_token          = QStringLiteral("4332211");

//            extern const QString response_type_token   = QStringLiteral("response_type");
//            extern const QString response_token        = QStringLiteral("token");

//            extern const QString scope_token           = QStringLiteral("scope");
//            extern const QString scope_val             = QStringLiteral("audio,video,friends,groups,offline");

//            extern const QString redirect_uri_token    = QStringLiteral("redirect_uri");
//            extern const QString redirect_uri_val      = QStringLiteral("https://oauth.vk.com/blank.html");

//            extern const QString access_token_key      = QStringLiteral("access_token");
//            extern const QString offset_key            = QStringLiteral("offset");
//            extern const QString limit_key             = QStringLiteral("count");
//            extern const QString response_key          = QStringLiteral("response");
//            extern const QString finished_key          = QStringLiteral("finished");
//            extern const QString user_id_key           = QStringLiteral("user_id");
//            extern const QString expires_in_key        = QStringLiteral("expires_in");

//            extern const QString error_key             = QStringLiteral("error");
//            extern const QString error_description_key = QStringLiteral("error_description_key");
//            extern const QString error_msg_key         = QStringLiteral("error_msg");
//            extern const QString error_code_key        = QStringLiteral("error_code");
//            extern const QString captcha_img_key       = QStringLiteral("captcha_img");
//            extern const QString captcha_sid_key       = QStringLiteral("captcha_sid");
//            extern const QString captcha_key           = QStringLiteral("captcha_key");

//            extern const QString predef1_key           = QStringLiteral("_1_");
//            extern const QString predef2_key           = QStringLiteral("_2_");

//            extern const QString execute_key           = QStringLiteral("execute");
//            extern const QString code_key              = QStringLiteral("code");

//            extern const QString albums_key            = QStringLiteral("albums");
//            extern const QString audio_list_key        = QStringLiteral("audio_list");
//            extern const QString groups_key            = QStringLiteral("groups");
//            extern const QString friends_key           = QStringLiteral("friends");
//            extern const QString albums_offset_key     = QStringLiteral("albums_offset");
//            extern const QString albums_finished_key   = QStringLiteral("albums_finished");
//            extern const QString url_key               = QStringLiteral("url");

//            extern const QString id_key                = QStringLiteral("id");
//            extern const QString owner_id_key          = QStringLiteral("owner_id");
//            extern const QString artist_key            = QStringLiteral("artist");
//            extern const QString title_key             = QStringLiteral("title");
//            extern const QString duration_key          = QStringLiteral("duration");
//            extern const QString genre_id_key          = QStringLiteral("genre_id");

//            extern const QString fields_key            = QStringLiteral("fields");
//            extern const QString counters_key          = QStringLiteral("counters");
//            extern const QString user_ids_key          = QStringLiteral("user_ids");
//            extern const QString screen_name_key       = QStringLiteral("screen_name");
//            extern const QString object_id_key         = QStringLiteral("object_id");
//            extern const QString type_key              = QStringLiteral("type");
//            extern const QString lyrics_id_key         = QStringLiteral("lyrics_id");

//            extern const QString email_key             = QStringLiteral("email");
//            extern const QString password_key          = QStringLiteral("pass");
        }
    }
}
