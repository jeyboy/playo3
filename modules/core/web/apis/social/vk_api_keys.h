#ifndef VK_API_KEYS
#define VK_API_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Vk {
            struct Keys {
                const QString base_url              = QStringLiteral("https://api.vk.com/method/");
                const QString version_key           = QStringLiteral("v");
                const QString access_token_key      = QStringLiteral("access_token");
                const QString offset_key            = QStringLiteral("offset");
                const QString limit_key             = QStringLiteral("count");
                const QString response_key          = QStringLiteral("response");
                const QString finished_key          = QStringLiteral("finished");

                const QString error_key             = QStringLiteral("error");
                const QString error_msg_key         = QStringLiteral("error_msg");
                const QString error_code_key        = QStringLiteral("error_code");
                const QString captcha_img_key       = QStringLiteral("captcha_img");
                const QString captcha_sid_key       = QStringLiteral("captcha_sid");
                const QString captcha_key           = QStringLiteral("captcha_key");

                const QString predef1_key           = QStringLiteral("_1_");
                const QString predef2_key           = QStringLiteral("_2_");

                const QString execute_key           = QStringLiteral("execute");
                const QString code_key              = QStringLiteral("code");

                const QString albums_key            = QStringLiteral("albums");
                const QString audio_list_key        = QStringLiteral("audio_list");
                const QString groups_key            = QStringLiteral("groups");
                const QString friends_key           = QStringLiteral("friends");
                const QString albums_offset_key     = QStringLiteral("albums_offset");
                const QString albums_finished_key   = QStringLiteral("albums_finished");
                const QString url_key               = QStringLiteral("url");

                const QString id_key                = QStringLiteral("id");
                const QString owner_id_key          = QStringLiteral("owner_id");
                const QString artist_key            = QStringLiteral("artist");
                const QString title_key             = QStringLiteral("title");
                const QString duration_key          = QStringLiteral("duration");
                const QString genre_id_key          = QStringLiteral("genre_id");
            };
        }
    }
}

#endif // VK_API_KEYS
