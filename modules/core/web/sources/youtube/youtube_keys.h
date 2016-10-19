#ifndef YOUTUBE_KEYS
#define YOUTUBE_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Youtube {
            extern const QString url_api_base;
            extern const QString url_info;
            extern const QString url_embed;
            extern const QString path_search;

            extern const QString tkn_region_code;
            extern const QString tkn_snippet;
            extern const QString tkn_id;
            extern const QString tkn_q;
            extern const QString tkn_max_results;
            extern const QString tkn_page_token;
            extern const QString tkn_video_embedable;
            extern const QString tkn_type;
            extern const QString tkn_items;
            extern const QString tkn_part;
            extern const QString tkn_key;
            extern const QString tkn_url_encoded_fmt_stream_map;
            extern const QString tkn_adaptive_fmts;
            extern const QString tkn_use_cipher_signature;
            extern const QString tkn_order;

            extern const QString val_client_tkn;
            extern const QString val_tkn;
            extern const QString val_name;
        }
    }
}

#endif // YOUTUBE_API_KEYS
