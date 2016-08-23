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

                void prepareAlbums(QJsonArray & albums) {
                    QJsonArray res;
                    for(QJsonArray::Iterator album = albums.begin(); album != albums.end(); album++) {
                        QJsonObject album_obj = (*album).toObject();
                        QString uid = idToStr(album_obj.value(tkn_id));

                        album_obj.insert(
                            tkn_loadable_cmd,
                             Cmd::build(
                                sourceType(), cmd_mtd_tracks_by_album,
                                {{CMD_ID, uid}}
                             ).toString()
                        );
                        res << album_obj;
                    }

                    albums = res;
                }
            };
        }
    }
}

#endif // OD_ALBUM_H
