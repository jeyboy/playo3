#ifndef VK_PLAYLIST
#define VK_PLAYLIST

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Playlist : public Base {
            public:               
                QJsonValue tracksPlaylistsByUser(const QString & user_id, QJsonArray * arr = 0, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return paRequest(
                                baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    { { tkn_code, query_track_playlists(user_id) } }
                                ).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE),
                                call_type_json, rules(offset), arr, proc_json_extract,
                                QStringList() << tkn_response << block_sets_audio,
                                call_method_get, Headers(), 0, false
                            );
                        }

                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
                    }

                    return QJsonArray();
                }

//                QJsonValue videoPlaylistsByUser(const QString & user_id, QJsonArray * arr = 0, int offset = 0) {
//                    Permissions perm = permissions(pr_media_content);

//                    switch(perm) {
//                        case perm_site:
//                        case perm_api: {
//                            return pRequest(
//                                baseUrlStr(
//                                    qst_api_def, tkn_execute,
//                                    { tkn_code, query_audio_albums.arg(user_id) }
//                                ).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE),
//                                call_type_json, rules(offset),
//                                arr, proc_json_extract, QStringList() << tkn_response << tkn_albums
//                            );
//                        }

//                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
//                    }

//                    return QJsonArray();
//                }
            };
        }
    }
}

#endif // VK_PLAYLIST
