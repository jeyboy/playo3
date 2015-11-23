#include "html_parser_keys.h"

namespace Core {
    namespace Web {
        namespace Html {
            extern const QString doctype_key            = QStringLiteral("!DOCTYPE");

            extern const QString any_elem_token         = QStringLiteral("*");
            extern const QString text_block_token       = QStringLiteral("text");
            extern const QString href_token             = QStringLiteral("href");
            extern const QString comment_block_token    = QStringLiteral("comment");

            extern const QString nbsp_token             = QStringLiteral("nbsp");
            extern const QString amp_token              = QStringLiteral("amp");
            extern const QString lt_token               = QStringLiteral("lt");
            extern const QString gt_token               = QStringLiteral("gt");

            extern const QString id_token               = QStringLiteral("id");
            extern const QString class_token            = QStringLiteral("class");
            extern const QString type_token             = QStringLiteral("type");
            extern const QString input_token            = QStringLiteral("input");
            extern const QString select_token           = QStringLiteral("select");
            extern const QString img_token              = QStringLiteral("img");
            extern const QString br_token               = QStringLiteral("br");
            extern const QString split_token            = QStringLiteral(" ");
            extern const QString def_value_key          = QStringLiteral("value");
            extern const QString action_key             = QStringLiteral("action");
            extern const QString name_key               = QStringLiteral("name");

            extern const QString a_tag                  = QStringLiteral("a");
            extern const QString script_tag             = QStringLiteral("script");
            extern const QString head_tag               = QStringLiteral("head");
            extern const QString meta_tag               = QStringLiteral("meta");
            extern const QString link_key               = QStringLiteral("link");

            extern const QString charset_key            = QStringLiteral("charset");
            extern const QString charset_attr_key       = QStringLiteral("charset=");
            extern const QString http_equiv_key         = QStringLiteral("http-equiv");
            extern const QString content_type_key       = QStringLiteral("content-type");
            extern const QString content_key            = QStringLiteral("content");
        }
    }
}
