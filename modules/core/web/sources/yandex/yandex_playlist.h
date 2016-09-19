#ifndef YANDEX_PLAYLIST_H
#define YANDEX_PLAYLIST_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Playlist : public virtual Base {
            public:
//                // title, kind, trackCount, tracks, owner: {uid, login, name, verified}, cover: {type, dir, version, uri}, trackIds, modified
//                inline QString playlistUrl(const QString & owner, const QString & kinds) { return url_site_v1 + QStringLiteral("playlist.jsx?owner=%1&kinds=%2&light=true").arg(owner, kinds); }

//                QJsonValue playlistsInfo(const QUrlQuery & args) {
//                    return QJsonObject();
//                }

                QJsonValue playlistsSearch(const QUrlQuery & args) { return playlistsSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue playlistsSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, LSTR("music-search.jsx"),
                                     {
                                         {LSTR("text"), limits.predicate},
                                         {LSTR("type"), tkn_playlists}
                                     }
                                ),
                                call_type_json, pageRules(tkn_page, limits.start_offset, limits.requests_limit, limits.items_limit),
                                0, proc_json_extract, QStringList() << tkn_playlists << tkn_items
                            );

                            preparePlaylists(response.content);
                        break;}

                        default: Logger::obj().write(name(), "PLAYLISTS SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio_set, cmd_mtd_playlists_search, response, limits);
                }

                QJsonValue playlistsByUser(const QUrlQuery & args) { return playlistsByUser(args.queryItemValue(CMD_ID)); }
                QJsonValue playlistsByUser(const QString & user_id) {
                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("library.jsx"),
                            {
                                {tkn_owner, user_id},
                                {tkn_filter, tkn_playlists}
                            }
                        ),
                        call_type_json
                    );

                    return preparePlaylistsBlock(info);
                }
            };
        }
    }
}

#endif // YANDEX_PLAYLIST_H
