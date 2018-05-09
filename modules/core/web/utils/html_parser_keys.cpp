#include "html_parser_keys.h"

namespace Core {
    namespace Web {
        namespace Html {
            extern const QString tag_doctype            = QLatin1String("!DOCTYPE");
            extern const QString tag_input              = QLatin1String("input");
            extern const QString tag_select             = QLatin1String("select");
            extern const QString tag_img                = QLatin1String("img");
            extern const QString tag_br                 = QLatin1String("br");
            extern const QString tag_a                  = QLatin1String("a");
            extern const QString tag_script             = QLatin1String("script");
            extern const QString tag_head               = QLatin1String("head");
            extern const QString tag_meta               = QLatin1String("meta");
            extern const QString tag_xml                = QLatin1String("?xml");
            extern const QString tag_link               = QLatin1String("link");

            extern const QString tkn_split              = QLatin1String(" ");
            extern const QString tkn_any_elem           = QLatin1String("*");
            extern const QString tkn_text_block         = QLatin1String("text");
            extern const QString tkn_comment_block      = QLatin1String("comment");
            extern const QString tkn_service_block      = QLatin1String("service");

            extern const QString tkn_charset            = QLatin1String("charset");
            extern const QString tkn_encoding           = QLatin1String("encoding");
            extern const QString tkn_charset_attr       = QLatin1String("charset=");
            extern const QString tkn_http_equiv         = QLatin1String("http-equiv");
            extern const QString tkn_content_type       = QLatin1String("content-type");
            extern const QString tkn_content            = QLatin1String("content");

            extern const QString attr_name              = QLatin1String("name");
            extern const QString attr_action            = QLatin1String("action");
            extern const QString attr_href              = QLatin1String("href");
            extern const QString attr_src               = QLatin1String("src");
            extern const QString attr_id                = QLatin1String("id");
            extern const QString attr_class             = QLatin1String("class");
            extern const QString attr_type              = QLatin1String("type");
            extern const QString attr_default           = QLatin1String("value");
        }
    }
}
