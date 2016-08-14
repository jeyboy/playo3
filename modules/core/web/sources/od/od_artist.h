#ifndef OD_ARTIST
#define OD_ARTIST

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Artist : public virtual Base {
            protected:
//                inline QString searchArtistsUrl(const QString & predicate) { return audioUrlStr(path_audio_search_artists, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs

                QJsonValue artistsInfo(const QStringList & uids) {
                    return sRequest(
                        audioInfoUrl(uids),
                        call_type_json
                    ).value(tkn_artists).toArray();
                }

                QJsonValue artistsSearch(const QUrlQuery & args) { return artistsSearch(SearchLimit::fromICmdParams(args)); }

                QJsonValue artistsSearch(const SearchLimit & limits) { // need to check
                    QueriableResponse response = pRequest(
                        audioUrlStr(
                            path_audio_search_artists,
                            { {tkn_q, limits.predicate} }
                        ),
                        call_type_json,
                        rules(limits.start_offset, qMin(OD_SEARCH_LIMIT, limits.items_limit), limits.requests_limit),
                        0, proc_json_extract, QStringList() << tkn_artists
                    );

                    return prepareBlock(dmt_artist, cmd_mtd_artists_search, response, limits);
                }
            };
        }
    }
}

#endif // OD_ARTIST
