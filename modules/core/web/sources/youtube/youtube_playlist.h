#ifndef YOUTUBE_PLAYLIST_H
#define YOUTUBE_PLAYLIST_H

#include "youtube_defines.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Playlist : public virtual Base {
            public:
                bool initUserPredefinedPlaylistIds(const QString & username = QString()) {
                    QUrlQuery query = baseQuery(qst_api, {{tkn_part, LSTR("contentDetails")}});

                    if (username.isEmpty())
                        query.addQueryItem(LSTR("mine"), const_true);
                    else
                        query.addQueryItem(LSTR("forUsername"), username);

                    QJsonArray res = saRequest(
                        baseUrlStr(qst_api, path_channels, query),
                        call_type_json, 0, proc_json_extract, YOUTUBE_ITEMS, call_method_get, authHeaders()
                    );

                    if (res.isEmpty()) return false;

                    QJsonObject first_channel_relations = JSON_OBJ2(res.first().toObject(), LSTR("contentDetails"), LSTR("relatedPlaylists"));

                    attrs[YOUTUBE_PLAYLIST_FAVORITE_UID] = JSON_STR(first_channel_relations, LSTR("favorites"));
                    attrs[YOUTUBE_PLAYLIST_HISTORY_UID] = JSON_STR(first_channel_relations, LSTR("watchHistory"));
                    attrs[YOUTUBE_PLAYLIST_LATER_UID] = JSON_STR(first_channel_relations, LSTR("watchLater"));
                    attrs[YOUTUBE_PLAYLIST_LIKED_UID] = JSON_STR(first_channel_relations, LSTR("likes"));
                    attrs[YOUTUBE_PLAYLIST_UPLOADS_UID] = JSON_STR(first_channel_relations, LSTR("uploads"));

                    return true;
                }

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
