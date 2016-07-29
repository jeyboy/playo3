#ifndef OD_PLAYLIST
#define OD_PLAYLIST

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Playlist : public Base {
            protected:
                inline QString addPlaylistUrl() { return audioUrlStr(path_audio_playlist_add); } // params : (name: '') (publicPlaylist: 'true')
                inline QString removePlaylistUrl() { return audioUrlStr(path_audio_playlist_del); } // params : (pid: '' (id of playlist))
                inline QString getPlaylistsUrl() { return audioUrlStr(path_audio_playlists); } // params: maybe has uid param ? need to check
                inline QString addToPlaylistsUrl() { return audioUrlStr(path_audio_to_playlist); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
                inline QString removeFromPlaylistsUrl() { return audioUrlStr(path_audio_del_from_playlist); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')

                inline QString searchAlbumsUrl(const QString & predicate) { return audioUrlStr(path_audio_search_albums, {{ tkn_q, predicate }}); } // params : (q: predicate) and pagination attrs

                inline QString formaddplUrl() { return audioUrlStr(path_audio_add_to_playlist_form); } // params: (tid: track id) // used for adding new item to playlist

//                inline QString playlistAudioUrl(const QString & pid) { return audioUrlStr(tkn_my, QUrlQuery(tkn_pid_eq % pid)); } // params: (pid: playlist id) and pagination attrs

                QJsonArray playlistInfo(const QString & playlist_id, int count, int offset) { //TODO: need to check
                    return pRequest(
                        audioUrlStr(
                            tkn_my,
                            {{ tkn_pid, playlist_id }}
                        ),
                        call_type_json, rules(offset, count),
                        0, proc_json_extract, QStringList() << tkn_tracks
                    );
                }

                QJsonValue playlistsInfo(const QStringList & playlist_ids) {
                    return sRequest(audioInfoUrl(playlist_ids), call_type_json).
                            value(tkn_albums).toArray();
                }
            };
        }
    }
}

#endif // OD_PLAYLIST
