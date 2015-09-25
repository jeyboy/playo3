#ifndef SOUNDCLOUD_API_KEYS
#define SOUNDCLOUD_API_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Soundcloud {
            extern const QString base_url;
            extern const QString offset_key;
            extern const QString limit_key;

            extern const QString audio_list_key;
            extern const QString playlist_key;
            extern const QString followings_key;
            extern const QString followers_key;
            extern const QString groups_key;

            extern const QString tracks_key;
            extern const QString id_key;
            extern const QString title_key;
            extern const QString name_key;
            extern const QString full_name_key;
            extern const QString username_key;

            extern const QString user_id_key;
            extern const QString download_url_key;
            extern const QString stream_url_key;
            extern const QString video_url_key;
            extern const QString original_format_key;
            extern const QString default_extension;
            extern const QString duration_key;
            extern const QString genre_id_key;
        }
    }
}

#endif // SOUNDCLOUD_API_KEYS
