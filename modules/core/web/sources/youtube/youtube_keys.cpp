#include "youtube_keys.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            extern const QString url_api_base          = QStringLiteral("https://www.googleapis.com/youtube/v3/");
            extern const QString url_info              = QStringLiteral("http://www.youtube.com/get_video_info?video_id=%1");
//            extern const QString url_video             = QStringLiteral("http://youtube.com/get_video?video_id=%1&t=%2&fmt=18&asv=2");
            extern const QString url_embed             = QStringLiteral("https://www.youtube.com/watch?v=%1&feature=player_embedded");
            extern const QString url_auth              = QStringLiteral("https://accounts.google.com/o/oauth2/auth");

            extern const QString path_search           = QStringLiteral("search");
            extern const QString path_video_cats       = QStringLiteral("videoCategories");
            extern const QString path_quide_cats       = QStringLiteral("guideCategories");

            extern const QString tkn_auth_email        = QStringLiteral("Email");
            extern const QString tkn_auth_pass         = QStringLiteral("Passwd");
            extern const QString tkn_auth_captcha      = QStringLiteral("identifier-captcha-input");

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

            extern const QString tkn_client_id         = QStringLiteral("client_id");
            extern const QString tkn_redirect_uri      = QStringLiteral("redirect_uri");
            extern const QString tkn_response_type     = QStringLiteral("response_type");
            extern const QString tkn_scope             = QStringLiteral("scope");


            extern const QString val_client_tkn        = QStringLiteral("AIzaSyApdFaU0JogzzuUA2a8GeGURe2H9Kw0XFs");
            extern const QString val_tkn               = QStringLiteral("995040225879-vlpceeo02mk7idjb06013gr4mfp2pbbc.apps.googleusercontent.com");
            extern const QString val_name              = QStringLiteral("Youtube");
        }
    }
}
