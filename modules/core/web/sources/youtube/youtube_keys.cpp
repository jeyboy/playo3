#include "youtube_keys.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            extern const QString url_api_base          = QLatin1String("https://www.googleapis.com/youtube/v3/");
            extern const QString url_info              = QLatin1String("http://www.youtube.com/get_video_info?video_id=%1");
//            extern const QString url_video             = QLatin1String("http://youtube.com/get_video?video_id=%1&t=%2&fmt=18&asv=2");
            extern const QString url_embed             = QLatin1String("https://www.youtube.com/watch?v=%1&feature=player_embedded");
            extern const QString url_auth              = QLatin1String("https://accounts.google.com/o/oauth2/auth");
            extern const QString url_token             = QLatin1String("https://accounts.google.com/o/oauth2/token");

            extern const QString path_search           = QLatin1String("search");
            extern const QString path_video_cats       = QLatin1String("videoCategories");
            extern const QString path_quide_cats       = QLatin1String("guideCategories");
            extern const QString path_subscriptions    = QLatin1String("subscriptions");
            extern const QString path_channels         = QLatin1String("channels");
            extern const QString path_videos            = QLatin1String("videos");


            extern const QString tkn_auth_email        = QLatin1String("Email");
            extern const QString tkn_auth_pass         = QLatin1String("Passwd");
            extern const QString tkn_auth_captcha      = QLatin1String("identifier-captcha-input");

            extern const QString tkn_profile_info      = QLatin1String("ProfileInformation");
            extern const QString tkn_encoded_profile_info
                                                       = QLatin1String("encoded_profile_information");

            extern const QString tkn_region_code       = QLatin1String("regionCode");
            extern const QString tkn_snippet           = QLatin1String("snippet");
            extern const QString tkn_id                = QLatin1String("id");
            extern const QString tkn_q                 = QLatin1String("q");
            extern const QString tkn_max_results       = QLatin1String("maxResults");
            extern const QString tkn_page_token        = QLatin1String("pageToken");
            extern const QString tkn_video_embedable   = QLatin1String("videoEmbeddable");
            extern const QString tkn_type              = QLatin1String("type");
            extern const QString tkn_items             = QLatin1String("items");
            extern const QString tkn_part              = QLatin1String("part");
            extern const QString tkn_key               = QLatin1String("key");
            extern const QString tkn_url_encoded_fmt_stream_map
                                                       = QLatin1String("url_encoded_fmt_stream_map");
            extern const QString tkn_adaptive_fmts     = QLatin1String("adaptive_fmts");
            extern const QString tkn_use_cipher_signature
                                                       = QLatin1String("use_cipher_signature");
            extern const QString tkn_order             = QLatin1String("order");

            extern const QString tkn_code               = QLatin1String("code");
            extern const QString tkn_client_id         = QLatin1String("client_id");
            extern const QString tkn_redirect_uri      = QLatin1String("redirect_uri");
            extern const QString tkn_response_type     = QLatin1String("response_type");
            extern const QString tkn_scope             = QLatin1String("scope");
            extern const QString tkn_access_token      = QLatin1String("access_token");
            extern const QString tkn_expires_in        = QLatin1String("expires_in");
            extern const QString tkn_refresh_token     = QLatin1String("refresh_token");

            extern const QString val_redirect_url      = QLatin1String("http://localhost:9999");
            extern const QString val_client_tkn        = QLatin1String("AIzaSyBiX7ylcLASR4mX7DtQDoQrE5_x6-Todr8");
            extern const QString val_tkn               = QLatin1String("970305815614-904q93bf04da3qdb09o82oucghob6urt.apps.googleusercontent.com");
            extern const QString val_sec_tkn           = QLatin1String("SABrlovkX9hV5QbCY6p8QjSS");
            extern const QString val_name              = QLatin1String("Youtube");
        }
    }
}
