#ifndef SOUNDCLOUD_API_KEYS
#define SOUNDCLOUD_API_KEYS

#include <qstringbuilder.h>

namespace Core {
    namespace Web {
        namespace Soundcloud {
            const QString base_url              = QStringLiteral("https://api.soundcloud.com/");

            const QString offset_key            = QStringLiteral("offset");
            const QString limit_key             = QStringLiteral("limit");

            const QString audio_list_key        = QStringLiteral("audio_list");
            const QString playlist_key          = QStringLiteral("playlists");
            const QString followings_key        = QStringLiteral("followings");
            const QString followers_key         = QStringLiteral("followers");
            const QString groups_key            = QStringLiteral("groups");

            const QString tracks_key            = QStringLiteral("tracks");
            const QString id_key                = QStringLiteral("id");
            const QString title_key             = QStringLiteral("title");
            const QString name_key              = QStringLiteral("name");
            const QString full_name_key         = QStringLiteral("full_name");
            const QString username_key          = QStringLiteral("username");

            const QString user_id_key           = QStringLiteral("user_id");
            const QString download_url_key      = QStringLiteral("download_url");
            const QString stream_url_key        = QStringLiteral("stream_url");
            const QString video_url_key         = QStringLiteral("video_url");
            const QString original_format_key   = QStringLiteral("original_format");
            const QString default_extension     = QStringLiteral("mp3");
            const QString duration_key          = QStringLiteral("duration");
            const QString genre_id_key          = QStringLiteral("genre_id");
        }
    }
}

#endif // SOUNDCLOUD_API_KEYS
