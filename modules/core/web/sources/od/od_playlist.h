#ifndef OD_PLAYLIST
#define OD_PLAYLIST

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Playlist : public virtual Base {
            protected:
//                inline QString addPlaylistUrl() { return audioUrlStr(path_audio_playlist_add); } // params : (name: '') (publicPlaylist: 'true')
//                inline QString removePlaylistUrl() { return audioUrlStr(path_audio_playlist_del); } // params : (pid: '' (id of playlist))
//                inline QString getPlaylistsUrl() { return audioUrlStr(path_audio_playlists); } // params: maybe has uid param ? need to check
//                inline QString addToPlaylistsUrl() { return audioUrlStr(path_audio_to_playlist); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
//                inline QString removeFromPlaylistsUrl() { return audioUrlStr(path_audio_del_from_playlist); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')

//                inline QString formaddplUrl() { return audioUrlStr(path_audio_add_to_playlist_form); } // params: (tid: track id) // used for adding new item to playlist

//                inline QString playlistAudioUrl(const QString & pid) { return audioUrlStr(tkn_my, QUrlQuery(tkn_pid_eq % pid)); } // params: (pid: playlist id) and pagination attrs
            public:
                QJsonValue playlistsInfo(const QStringList & playlist_ids, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) { // TODO: not tested
                    QJsonArray block_content = sRequest(audioInfoUrl(playlist_ids), call_type_json).value(tkn_albums).toArray();

                    return prepareBlock(dmt_audio_set, block_content, block_params);
                }
            };
        }
    }
}

#endif // OD_PLAYLIST
