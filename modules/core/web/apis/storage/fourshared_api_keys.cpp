#include "fourshared_api_keys.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
        extern const QString name_key               = QStringLiteral("4shared");
        extern const QString oauth_consumer_key     = QStringLiteral("oauth_consumer_key=");
        extern const QString def_token_key          = QStringLiteral("22abeb63487b7f6b75051079b7e610b1");
        extern const QString json_ext_key           = QStringLiteral(".json");
        extern const QString lang_cookie            = QStringLiteral("4langcookie=en; domain=.4shared.com; path=/");

        extern const QString down_base_url          = QStringLiteral("http://4server.info/download/");
        extern const QString base_url               = QStringLiteral("https://api.4shared.com/v0/");
        extern const QString offset_key             = QStringLiteral("offset");
        extern const QString limit_key              = QStringLiteral("limit");

        extern const QString files_token_key        = QStringLiteral("files");
        extern const QString category_token_key     = QStringLiteral("category");
        extern const QString query_token_key        = QStringLiteral("query");
        extern const QString download_page_key      = QStringLiteral("downloadPage");
        extern const QString name_token_key         = QStringLiteral("name");
        extern const QString size_token_key         = QStringLiteral("size");

        extern const QString span_tag               = QStringLiteral("span");
        extern const QString filetype_tag           = QStringLiteral("File type");
        extern const QString bitrate_tag            = QStringLiteral("Bit rate");
        extern const QString discretion_rate_tag    = QStringLiteral("Sample rate");
        extern const QString genre_tag              = QStringLiteral("Genre");
        extern const QString year_tag               = QStringLiteral("Year");
        }
    }
}
