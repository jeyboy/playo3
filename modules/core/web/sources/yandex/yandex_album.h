#ifndef YANDEX_ALBUM_H
#define YANDEX_ALBUM_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Album : public virtual Base {
            public:
                QJsonValue albumsInfo(const QStringList & album_ids) {// not tested
                    SourceFlags perm = permissions(sf_album_by_id);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            int limiter = album_ids.size() / YANDEX_IDS_PER_REQUEST + (int)(album_ids.size() % YANDEX_IDS_PER_REQUEST != 0);
                            for(int step = 0; step < limiter; step++) {
                                std::initializer_list<std::pair<QString, QVariant> > params = {
                                    { LSTR("albumIds"), (QStringList)album_ids.mid(step * YANDEX_IDS_PER_REQUEST, YANDEX_IDS_PER_REQUEST) }
                                };

                                saRequest(
                                    baseUrlStr(qst_site, LSTR("albums.jsx"), params),
                                    call_type_json, &block_content, proc_json_patch, IQUERY_DEF_FIELDS, call_method_post
                                );
                            }
                        break;}

                        default: Logger::obj().write(name(), "album info is not accessable", Logger::log_error);
                    }

                    prepareAlbums(block_content);
                    return prepareBlock(dmt_audio_set, block_content);
                }

                QJsonValue albumsByArtist(const QString & artist_id) {
                    QJsonArray blocks;

                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("artist.jsx"),
                            {
                                {LSTR("artist"), artist_id},
                                {LSTR("what"), tkn_albums}
                            }
                        ), call_type_json
                    );

                    blocks << prepareAlbumsBlock(info, tkn_album, {{tkn_dir_name, LSTR("Albums")}});
                    blocks << prepareAlbumsBlock(info, LSTR("alsoAlbum"), {{tkn_dir_name, LSTR("Compilations")}});

                    return blocks;
                }

                QJsonValue albumsSearch(const QUrlQuery & args) { return albumsSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue albumsSearch(const SearchLimit & limits) {
                    SourceFlags perm = permissions(sf_album_by_title);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api:
                        case sf_site: {
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
