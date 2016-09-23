#ifndef YANDEX_PLAYLIST_H
#define YANDEX_PLAYLIST_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Playlist : public virtual Base { // has tags array in response
            public:
                // curl 'https://music.yandex.ua/handlers/playlists-list.jsx' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' -H 'Referer: https://music.yandex.ua/users/music.partners/playlists' -H 'X-Current-UID: 363617853' -H 'X-Retpath-Y: https://music.yandex.ua/users/music.partners/playlists' -H 'X-Requested-With: XMLHttpRequest' --data 'ids=1687&owner=139954184&sign=u98cd41597697b6707717f67edc5d24f0&lang=ru&external-domain=music.yandex.ua&overembed=false'
                QJsonValue playlistsInfo(const QUrlQuery & args) {
                    return playlistsInfo(
                        args.queryItemValue(CMD_OWNER),
                        args.queryItemValue(CMD_ID)
                    );
                }
                QJsonValue playlistsInfo(const QString & owner_uid, const QString & playlist_ids) {
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            int limiter = playlist_ids.size() / YANDEX_IDS_PER_REQUEST + (int)(playlist_ids.size() % YANDEX_IDS_PER_REQUEST != 0);
                            for(int step = 0; step < limiter; step++) {
                                std::initializer_list<std::pair<QString, QVariant> > params = {
                                    { LSTR("ids"), (QStringList)playlist_ids.mid(step * YANDEX_IDS_PER_REQUEST, YANDEX_IDS_PER_REQUEST) },
                                    { tkn_owner, owner_uid }
                                };

                                saRequest(
                                    baseUrlStr(qst_site, LSTR("playlists-list.jsx"), params),
                                    call_type_json, &block_content, proc_json_patch, IQUERY_DEF_FIELDS, call_method_post
                                );
                            }
                        break;}

                        default: Logger::obj().write(name(), "playlists info is not accessable", Logger::log_error);
                    }

                    preparePlaylists(block_content);
                    return prepareBlock(dmt_audio_set, block_content);
                }

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
