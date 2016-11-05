#include "youtube_keys.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            extern const QString url_api_base          = QStringLiteral("https://www.googleapis.com/youtube/v3/");
            extern const QString url_info              = QStringLiteral("http://www.youtube.com/get_video_info?video_id=%1");
//            extern const QString url_video             = QStringLiteral("http://youtube.com/get_video?video_id=%1&t=%2&fmt=18&asv=2");
            extern const QString url_embed             = QStringLiteral("https://www.youtube.com/watch?v=%1&feature=player_embedded");
            extern const QString url_auth              = QStringLiteral("https://accounts.google.com/o/oauth2/auth");
            extern const QString url_token             = QStringLiteral("https://accounts.google.com/o/oauth2/token");

            extern const QString path_search           = QStringLiteral("search");
            extern const QString path_video_cats       = QStringLiteral("videoCategories");
            extern const QString path_quide_cats       = QStringLiteral("guideCategories");
            extern const QString path_subscriptions    = QStringLiteral("subscriptions");
            extern const QString path_channels         = QStringLiteral("channels");
            extern const QString path_videos            = QStringLiteral("videos");


            extern const QString tkn_auth_email        = QStringLiteral("Email");
            extern const QString tkn_auth_pass         = QStringLiteral("Passwd");
            extern const QString tkn_auth_captcha      = QStringLiteral("identifier-captcha-input");

            extern const QString tkn_profile_info      = QStringLiteral("ProfileInformation");
            extern const QString tkn_encoded_profile_info
                                                       = QStringLiteral("encoded_profile_information");

            extern const QString tkn_region_code       = QStringLiteral("regionCode");
            extern const QString tkn_snippet           = QStringLiteral("snippet");
            extern const QString tkn_id                = QStringLiteral("id");
            extern const QString tkn_q                 = QStringLiteral("q");
            extern const QString tkn_max_results       = QStringLiteral("maxResults");
            extern const QString tkn_page_token        = QStringLiteral("pageToken");
            extern const QString tkn_video_embedable   = QStringLiteral("videoEmbeddable");
            extern const QString tkn_type              = QStringLiteral("type");
            extern const QString tkn_items             = QStringLiteral("items");
            extern const QString tkn_part              = QStringLiteral("part");
            extern const QString tkn_key               = QStringLiteral("key");
            extern const QString tkn_url_encoded_fmt_stream_map
                                                       = QStringLiteral("url_encoded_fmt_stream_map");
            extern const QString tkn_adaptive_fmts     = QStringLiteral("adaptive_fmts");
            extern const QString tkn_use_cipher_signature
                                                       = QStringLiteral("use_cipher_signature");
            extern const QString tkn_order             = QStringLiteral("order");

            extern const QString tkn_code               = QStringLiteral("code");
            extern const QString tkn_client_id         = QStringLiteral("client_id");
            extern const QString tkn_redirect_uri      = QStringLiteral("redirect_uri");
            extern const QString tkn_response_type     = QStringLiteral("response_type");
            extern const QString tkn_scope             = QStringLiteral("scope");
            extern const QString tkn_access_token      = QStringLiteral("access_token");
            extern const QString tkn_expires_in        = QStringLiteral("expires_in");
            extern const QString tkn_refresh_token     = QStringLiteral("refresh_token");

            extern const QString val_redirect_url      = QStringLiteral("http://localhost:9999");
            extern const QString val_client_tkn        = QStringLiteral("AIzaSyBiX7ylcLASR4mX7DtQDoQrE5_x6-Todr8");
            extern const QString val_tkn               = QStringLiteral("970305815614-904q93bf04da3qdb09o82oucghob6urt.apps.googleusercontent.com");
            extern const QString val_sec_tkn           = QStringLiteral("SABrlovkX9hV5QbCY6p8QjSS");
            extern const QString val_name              = QStringLiteral("Youtube");
        }
    }
}
