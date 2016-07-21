#include "fourshared_api_keys.h"

namespace Core {
    namespace Web {
        namespace Fourshared {       
            extern const QString tkn_oauth_consumer     = QStringLiteral("oauth_consumer_key=");
            extern const QString tkn_offset             = QStringLiteral("offset");
            extern const QString tkn_limit              = QStringLiteral("limit");
            extern const QString tkn_files              = QStringLiteral("files");
            extern const QString tkn_category           = QStringLiteral("category");
            extern const QString tkn_query              = QStringLiteral("query");
            extern const QString tkn_download_page      = QStringLiteral("downloadPage");
            extern const QString tkn_name               = QStringLiteral("name");
            extern const QString tkn_size               = QStringLiteral("size");
            extern const QString tkn_genre              = QStringLiteral("genre");
            extern const QString tkn_artist             = QStringLiteral("artist");

            extern const QString tag_span               = QStringLiteral("span");
            extern const QString tag_filetype           = QStringLiteral("File type");
            extern const QString tag_bitrate            = QStringLiteral("Bit rate");
            extern const QString tag_discretion_rate    = QStringLiteral("Sample rate");
            extern const QString tag_genre              = QStringLiteral("Genre");
            extern const QString tag_year               = QStringLiteral("Year");

            extern const QString val_name               = QStringLiteral("4shared");
            extern const QString val_token              = QStringLiteral("22abeb63487b7f6b75051079b7e610b1");
            extern const QString val_json_ext           = QStringLiteral(".json");
            extern const QString val_lang_cookie        = QStringLiteral("4langcookie=en; domain=.4shared.com; path=/");
            extern const QString val_version            = QStringLiteral("1_2");

            extern const QString url_down_base          = QStringLiteral("http://4server.info/download/");
            extern const QString url_api_base           = QStringLiteral("https://api.4shared.com/v%1/").arg(val_version);
            extern const QString url_api_search         = QStringLiteral("https://search.4shared.com/v%1/").arg(val_version);

            extern const QString url_html_site_base     = QStringLiteral("http://www.4shared.com/");
            extern const QString url_html_site_search   = QStringLiteral("http://search.4shared.com/");
        }
    }
}
