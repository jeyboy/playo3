#include "html_parser_keys.h"

namespace Core {
    namespace Web {
        namespace Html {
            extern const QString tag_doctype            = QStringLiteral("!DOCTYPE");
            extern const QString tag_input              = QStringLiteral("input");
            extern const QString tag_select             = QStringLiteral("select");
            extern const QString tag_img                = QStringLiteral("img");
            extern const QString tag_br                 = QStringLiteral("br");
            extern const QString tag_a                  = QStringLiteral("a");
            extern const QString tag_script             = QStringLiteral("script");
            extern const QString tag_head               = QStringLiteral("head");
            extern const QString tag_meta               = QStringLiteral("meta");
            extern const QString tag_link               = QStringLiteral("link");

            extern const QString tkn_split              = QStringLiteral(" ");
            extern const QString tkn_any_elem           = QStringLiteral("*");
            extern const QString tkn_text_block         = QStringLiteral("text");
            extern const QString tkn_comment_block      = QStringLiteral("comment");
            extern const QString tkn_nbsp               = QStringLiteral("nbsp");
            extern const QString tkn_amp                = QStringLiteral("amp");
            extern const QString tkn_lt                 = QStringLiteral("lt");
            extern const QString tkn_gt                 = QStringLiteral("gt");
            extern const QString tkn_charset            = QStringLiteral("charset");
            extern const QString tkn_charset_attr       = QStringLiteral("charset=");
            extern const QString tkn_http_equiv         = QStringLiteral("http-equiv");
            extern const QString tkn_content_type       = QStringLiteral("content-type");
            extern const QString tkn_content            = QStringLiteral("content");

            extern const QString attr_name              = QStringLiteral("name");
            extern const QString attr_action            = QStringLiteral("action");
            extern const QString attr_href              = QStringLiteral("href");
            extern const QString attr_id                = QStringLiteral("id");
            extern const QString attr_class             = QStringLiteral("class");
            extern const QString attr_type              = QStringLiteral("type");
            extern const QString attr_default           = QStringLiteral("value");
        }
    }
}
