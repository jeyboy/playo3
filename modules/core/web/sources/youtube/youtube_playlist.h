#ifndef YOUTUBE_PLAYLIST_H
#define YOUTUBE_PLAYLIST_H

#include "youtube_defines.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Playlist : public virtual Base {
            public:
                QJsonValue playlistsByPredicate(const QUrlQuery & args) {
                    return playlistsByPredicate(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue playlistsByPredicate(const QString & predicate, const QString & token = QString()) {
                    SourceFlags perm = permissions(sf_channel_by_title);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            QUrlQuery query = videoQuery({
                                {tkn_type, LSTR("playlist")},
                                {tkn_q, predicate}
                            });

                            QueriableResponse response = pRequest(
                                baseUrlStr(qst_api, path_search, query), call_type_json,
                                rules(token), 0, proc_json_extract, YOUTUBE_ITEMS
                            );

                            return prepareBlock(dmt_video, cmd_mtd_playlists_by_predicate, response);
                        break;}
                        default: Logger::obj().write(name(), "videoPlaylistsByName", Logger::log_error);
                    }

                    return QJsonObject();
                }
            };
        }
    }
}

#endif // YOUTUBE_PLAYLIST_H
