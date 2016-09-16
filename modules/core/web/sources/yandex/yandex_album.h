#ifndef YANDEX_ALBUM_H
#define YANDEX_ALBUM_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Album : public virtual Base {
            public:
                // id, title, year, cover, coverUri, genre, artists, labels: [{id, name}], volumes: [[{id, realId, title, durationMs, fileSize, storageDir, available}, ..]]
                inline QString albumInfoUrl(const QString & albumId) { return url_site_v1 + QStringLiteral("albums.jsx?albumIds=%1").arg(albumId); }

                inline QString artistAlbumsUrl(const QString & artistId) { return url_site_v1 + QStringLiteral("artist.jsx?artist=%1&what=albums&sort=&dir=").arg(artistId); }

                QJsonValue albumsSearch(const QUrlQuery & args) { return albumsSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue albumsSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, LSTR("music-search.jsx"),
                                     {
                                         {LSTR("text"), limits.predicate},
                                         {LSTR("type"), tkn_albums}
                                     }
                                ),
                                call_type_json, pageRules(tkn_page, limits.start_offset, limits.requests_limit, limits.items_limit),
                                0, proc_json_extract, QStringList() << tkn_albums << tkn_items
                            );

                            prepareAlbums(response.content);
                        break;}

                        default: Logger::obj().write(name(), "ALBUMS SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio_set, cmd_mtd_albums_search, response, limits);
                }
            };
        }
    }
}

#endif // YANDEX_ALBUM_H
