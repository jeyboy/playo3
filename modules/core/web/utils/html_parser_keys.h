#ifndef HTML_PARSER_KEYS
#define HTML_PARSER_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Html {
            extern const QString doctype_key;

            extern const QString any_elem_token;
            extern const QString text_block_token;
            extern const QString href_token;
            extern const QString comment_block_token;

            extern const QString nbsp_token;
            extern const QString amp_token;
            extern const QString lt_token;
            extern const QString gt_token;

            extern const QString id_token;
            extern const QString class_token;
            extern const QString type_token;
            extern const QString input_token;
            extern const QString select_token;
            extern const QString img_token;
            extern const QString br_token;
            extern const QString split_token;
            extern const QString def_value_key;
            extern const QString action_key;
            extern const QString name_key;

            extern const QString a_tag;
            extern const QString script_tag;
            extern const QString head_tag;
            extern const QString meta_tag;
            extern const QString link_key;

            extern const QString charset_key;
            extern const QString charset_attr_key;
            extern const QString http_equiv_key;
            extern const QString content_type_key;
            extern const QString content_key;
        }
    }
}

#endif // HTML_PARSER_KEYS
