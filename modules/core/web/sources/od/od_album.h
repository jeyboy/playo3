#ifndef OD_ALBUM_H
#define OD_ALBUM_H

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Album : public virtual Base {
            protected:
                QJsonValue albumsSearch(const QUrlQuery & args) { return albumsSearch(SearchLimit::fromICmdParams(args)); }

                QJsonValue albumsSearch(const SearchLimit & limits) { // need to check
                    QueriableResponse response = pRequest(
                        audioUrlStr(
                            path_audio_search_albums,
                            { {tkn_q, limits.predicate} }
                        ),
                        call_type_json,
                        rules(limits.start_offset, qMin(OD_SEARCH_LIMIT, limits.items_limit), limits.requests_limit),
                        0, proc_json_extract, QStringList() << tkn_albums
                    );

                    return prepareBlock(dmt_set, cmd_mtd_albums_search, response, limits);
                }
            };
        }
    }
}

#endif // OD_ALBUM_H
