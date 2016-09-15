#ifndef YANDEX_ARTIST_H
#define YANDEX_ARTIST_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Artist : public virtual Base {
            protected:
                // array of {id, name, cover, genres, ratings}
                inline QString artistInfoUrl(const QString & artistId) { return url_site_v1 + (QStringLiteral("artist.jsx?artist=%1").arg(artistId)); }

                inline QString artistSimilarUrl(const QString & artistId) { return url_site_v1 + QStringLiteral("artist.jsx?artist=%1&what=similar&sort=&dir=").arg(artistId); }

//                QJsonValue artistsInfo(const QStringList & uids) {
//                    return sRequest(
//                        audioInfoUrl(uids),
//                        call_type_json
//                    ).value(tkn_artists).toArray();
//                }

//                QJsonValue artistsSearch(const QUrlQuery & args) { return artistsSearch(SearchLimit::fromICmdParams(args)); }
//                QJsonValue artistsSearch(const SearchLimit & limits) { // need to check
//                    QueriableResponse response = pRequest(
//                        audioUrlStr(
//                            path_audio_search_artists,
//                            { {tkn_q, limits.predicate} }
//                        ),
//                        call_type_json,
//                        rules(limits.start_offset, qMin(OD_SEARCH_LIMIT, limits.items_limit), limits.requests_limit),
//                        0, proc_json_extract, QStringList() << tkn_artists
//                    );

//                    return prepareBlock(dmt_artist, cmd_mtd_artists_search, response, limits);
//                }


                //curl 'https://music.yandex.ua/handlers/artist.jsx?artist=36830&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.14533055633210124' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=N9ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/genre/rock/artists' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/genre/rock/artists'
                // what=tracks/albums and etc // returns 150 records

                QJsonValue artistInfo(const QUrlQuery & args) {
                    return artistInfo(args.queryItemValue(CMD_ID));
                }

                QJsonValue artistInfo(const QString & artist_id) {
                    QJsonArray blocks;

                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("artist.jsx"),
                            {
                                {LSTR("artist"), artist_id},
                                {LSTR("what"), LSTR("tracks")}
                            }
                        ),
                        call_type_json
                    );

                    //////////////////// TRACKS ///////////////////////
                    QJsonArray tracks = JSON_ARR(info, tkn_tracks);
                    QJsonArray track_ids = JSON_ARR(info, LSTR("trackIds"));
                    QJsonObject tracks_block = prepareBlock(dmt_audio, prepareTracks(tracks), {{tkn_dir_name, LSTR("Tracks")}});

                    if (tracks.size() < track_ids.size()) {
                        QString ids;

                        for(QJsonArray::Iterator id = track_ids.begin() + tracks.size(); id != track_ids.end(); id++)
                            ids = ids % (ids.isEmpty() ? QString() : LSTR(",")) % (*id).toString();

                        tracks_block.insert(
                            Web::tkn_more_cmd,
                            Cmd::build(sourceType(), cmd_mtd_tracks_info, {{CMD_ID, ids}}).toString()
                        );
                    }

                    blocks << tracks_block;

                    //////////////////// ALBUMS ///////////////////////
                    QJsonArray albums = JSON_ARR(info, tkn_albums);
                    blocks << prepareBlock(dmt_audio_set, prepareAlbums(albums), {{tkn_dir_name, LSTR("Albums")}});
                    QJsonArray also_albums = JSON_ARR(info, LSTR("alsoAlbums"));
                    blocks << prepareBlock(dmt_audio_set, prepareAlbums(also_albums), {{tkn_dir_name, LSTR("Compilations")}});

                    //////////////////// SIMILAR ///////////////////////
                    QJsonArray similar_artists = JSON_ARR(info, LSTR("similar"));
                    blocks << prepareBlock(dmt_audio_set, prepareArtists(similar_artists), {{tkn_dir_name, LSTR("Similar")}});

                    // QJsonArray videos = JSON_ARR(info, LSTR("videos"));

                    return blocks;
                }
            };
        }
    }
}

#endif // YANDEX_ARTIST_H
