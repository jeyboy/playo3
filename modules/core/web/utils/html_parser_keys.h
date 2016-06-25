#ifndef HTML_PARSER_KEYS
#define HTML_PARSER_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Html {
            extern const QString tag_doctype;
            extern const QString tag_input;
            extern const QString tag_select;
            extern const QString tag_img;
            extern const QString tag_br;
            extern const QString tag_a;
            extern const QString tag_script;
            extern const QString tag_head;
            extern const QString tag_xml;
            extern const QString tag_meta;
            extern const QString tag_link;

            extern const QString tkn_split;
            extern const QString tkn_any_elem;
            extern const QString tkn_text_block;
            extern const QString tkn_comment_block;
            extern const QString tkn_service_block;
            extern const QString tkn_nbsp;
            extern const QString tkn_amp;
            extern const QString tkn_lt;
            extern const QString tkn_gt;
            extern const QString tkn_quot;
            extern const QString tkn_charset;
            extern const QString tkn_encoding;
            extern const QString tkn_charset_attr;
            extern const QString tkn_http_equiv;
            extern const QString tkn_content_type;
            extern const QString tkn_content;

            extern const QString attr_name;
            extern const QString attr_action;
            extern const QString attr_href;
            extern const QString attr_id;
            extern const QString attr_class;
            extern const QString attr_type;
            extern const QString attr_default;
        }
    }
}

#endif // HTML_PARSER_KEYS
