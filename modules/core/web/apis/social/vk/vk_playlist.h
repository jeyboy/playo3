#ifndef VK_PLAYLIST
#define VK_PLAYLIST

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Playlist : public Base {
            public:
                void tracksPlaylistByUser(QString & user_id, QJsonArray * arr = 0, int offset = 0) {
                    pRequest(
                        baseUrlStr(
                            qst_api_def, tkn_execute,
                            { { tkn_code, query_audio_albums.arg(user_id) } }
                        ).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE),
                        call_type_json, rules(offset), arr, proc_json_extract,
                        QStringList() << tkn_response << tkn_albums,
                        call_method_get, Headers(), 0, false
                    );
                }
            };
        }
    }
}

#endif // VK_PLAYLIST
