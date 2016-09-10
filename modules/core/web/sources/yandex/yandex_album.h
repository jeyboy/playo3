#ifndef YANDEX_ALBUM_H
#define YANDEX_ALBUM_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Album : public Base {
            public:
                // id, title, year, cover, coverUri, genre, artists, labels: [{id, name}], volumes: [[{id, realId, title, durationMs, fileSize, storageDir, available}, ..]]
                inline QString albumInfoUrl(const QString & albumId) { return url_site_v1 + QStringLiteral("albums.jsx?albumIds=%1").arg(albumId); }

                inline QString artistAlbumsUrl(const QString & artistId) { return url_site_v1 + QStringLiteral("artist.jsx?artist=%1&what=albums&sort=&dir=").arg(artistId); }
            };
        }
    }
}

#endif // YANDEX_ALBUM_H
