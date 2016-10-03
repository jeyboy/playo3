#ifndef VK_PLAYLIST
#define VK_PLAYLIST

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Playlist : public Base {
            public:
                QJsonValue tracksPlaylistsByUser(const QUrlQuery & args) {
                    return tracksPlaylistsByUser(
                        args.queryItemValue(CMD_ID),
                        0,
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksPlaylistsByUser(const QString & user_id, QJsonArray * arr = 0, int offset = 0, int count = 10) {
                    SourceFlags perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_api, tkn_execute,
                                    { { tkn_code, query_track_playlists(user_id) } }
                                ).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE),
                                call_type_json, rules(offset, count), arr, proc_json_extract,
                                QStringList() << tkn_response << block_sets_audio,
                                call_method_get, Headers(), 0, false
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_set, cmd_mtd_tracks_playlists_by_user, response, {}, {{CMD_ID, user_id}});
                }

//                QJsonValue videoPlaylistsByUser(const QString & user_id, QJsonArray * arr = 0, int offset = 0) {
//                    SourceFlags perm = permissions(pr_media_content);

//                    switch(perm) {
//                        case perm_site:
//                        case perm_api: {
//                            return pRequest(
//                                baseUrlStr(
//                                    qst_api, tkn_execute,
//                                    { tkn_code, query_audio_albums.arg(user_id) }
//                                ).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE),
//                                call_type_json, rules(offset),
//                                arr, proc_json_extract, QStringList() << tkn_response << tkn_albums
//                            );
//                        }

//                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", true);
//                    }

//                    return QJsonArray();
//                }
            };
        }
    }
}

#endif // VK_PLAYLIST
