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
            };
        }
    }
}

#endif // YANDEX_ARTIST_H
