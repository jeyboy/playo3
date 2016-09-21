#ifndef YANDEX_ARTIST_H
#define YANDEX_ARTIST_H

#include "yandex_track.h"
#include "yandex_album.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Artist : public virtual Track, public virtual Album {
            protected:
                QJsonValue artistInfo(const QUrlQuery & args) {
                    return artistInfo(args.queryItemValue(CMD_ID));
                }
                QJsonValue artistInfo(const QString & artist_id) {
                    QJsonArray blocks;

                    blocks << tracksByArtist(artist_id, {{tkn_dir_name, LSTR("Tracks")}});
                    QueriableArg::arrAppend(blocks, albumsByArtist(artist_id).toArray());

                    blocks << artistRecommendations(artist_id, {{tkn_dir_name, LSTR("Similar")}});

//                    QJsonObject info = sRequest(
//                        baseUrlStr(
//                            qst_site, LSTR("artist.jsx"),
//                            {
//                                {LSTR("artist"), artist_id},
//                                {LSTR("what"), LSTR("tracks")}
//                            }
//                        ),
//                        call_type_json
//                    );

//                    //////////////////// TRACKS ///////////////////////
//                    blocks << prepareTracksBlock(info, {{tkn_dir_name, LSTR("Tracks")}});

//                    //////////////////// ALBUMS ///////////////////////
//                    QJsonArray albums = JSON_ARR(info, tkn_albums);
//                    blocks << prepareBlock(dmt_audio_set, prepareAlbums(albums), {{tkn_dir_name, LSTR("Albums")}});
//                    QJsonArray also_albums = JSON_ARR(info, LSTR("alsoAlbums"));
//                    blocks << prepareBlock(dmt_audio_set, prepareAlbums(also_albums), {{tkn_dir_name, LSTR("Compilations")}});

//                    //////////////////// SIMILAR ///////////////////////
//                    QJsonArray similar_artists = JSON_ARR(info, LSTR("similar"));
//                    blocks << prepareBlock(dmt_audio_set, prepareArtists(similar_artists), {{tkn_dir_name, LSTR("Similar")}});

                    // QJsonArray videos = JSON_ARR(info, LSTR("videos"));

                    return blocks;
                }

                QJsonValue artistRecommendations(const QUrlQuery & args) {
                    return artistRecommendations(args.queryItemValue(CMD_ID));
                }
                QJsonValue artistRecommendations(const QString & artist_id, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("artist.jsx"),
                            {
                                {LSTR("artist"), artist_id},
                                {LSTR("what"), LSTR("similar")}
                            }
                        ),
                        call_type_json
                    );

                    QJsonArray similar_artists = JSON_ARR(info, LSTR("similar"));
                    return prepareBlock(dmt_audio_set, prepareArtists(similar_artists), block_params);
                }

                QJsonValue artistsSearch(const QUrlQuery & args) { return artistsSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue artistsSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, LSTR("music-search.jsx"),
                                     {
                                         {LSTR("text"), limits.predicate},
                                         {LSTR("type"), tkn_artists}
                                     }
                                ),
                                call_type_json, pageRules(tkn_page, limits.start_offset, limits.requests_limit, limits.items_limit),
                                0, proc_json_extract, QStringList() << tkn_artists << tkn_items
                            );

                            prepareArtists(response.content);
                        break;}

                        default: Logger::obj().write(name(), "ARTIST SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio_set, cmd_mtd_artists_search, response, limits);
                }
            };
        }
    }
}

#endif // YANDEX_ARTIST_H
