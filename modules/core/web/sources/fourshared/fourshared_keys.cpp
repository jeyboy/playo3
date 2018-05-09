#include "fourshared_keys.h"

namespace Core {
    namespace Web {
        namespace Fourshared {       
            extern const QString tkn_oauth_consumer     = QLatin1String("oauth_consumer_key=");
            extern const QString tkn_offset             = QLatin1String("offset");
            extern const QString tkn_limit              = QLatin1String("limit");
            extern const QString tkn_files              = QLatin1String("files");
            extern const QString tkn_category           = QLatin1String("category");
            extern const QString tkn_query              = QLatin1String("query");
            extern const QString tkn_download_page      = QLatin1String("downloadPage");
            extern const QString tkn_id                 = QLatin1String("id");
            extern const QString tkn_name               = QLatin1String("name");
            extern const QString tkn_size               = QLatin1String("size");
            extern const QString tkn_genre              = QLatin1String("genre");
            extern const QString tkn_artist             = QLatin1String("artist");

            extern const QString tag_span               = QLatin1String("span");
            extern const QString tag_filetype           = QLatin1String("File type");
            extern const QString tag_bitrate            = QLatin1String("Bit rate");
            extern const QString tag_discretion_rate    = QLatin1String("Sample rate");
            extern const QString tag_genre              = QLatin1String("Genre");
            extern const QString tag_year               = QLatin1String("Year");

            extern const QString val_name               = QLatin1String("4shared");
            extern const QString val_token              = QLatin1String("22abeb63487b7f6b75051079b7e610b1");
            extern const QString val_json_ext           = QLatin1String(".json");
            extern const QString val_lang_cookie        = QLatin1String("4langcookie=en; domain=.4shared.com; path=/");
            extern const QString val_version            = QLatin1String("1_2");

            extern const QString url_down_base          = QLatin1String("http://4server.info/download/");
            extern const QString url_api_base           = QStringLiteral("https://api.4shared.com/v%1/").arg(val_version);
            extern const QString url_api_search         = QStringLiteral("https://search.4shared.com/v%1/").arg(val_version);

            extern const QString url_html_site_base     = QLatin1String("http://www.4shared.com/");
            extern const QString url_html_site_search   = QLatin1String("http://search.4shared.com/");
        }
    }
}
