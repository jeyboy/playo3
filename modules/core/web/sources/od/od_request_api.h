#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include "od_misc.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core { // NOT FINISHED
    namespace Web {
        namespace Od {
            class RequestApi : public ISource, public Sociable, public Misc {
            protected:
                RequestApi() {}

//                inline void setPagination(QUrlQuery & query, int per_request = OD_LIMIT_PER_REQUEST) {
//                    setParam(query, tkn_offset, OFFSET_TEMPLATE);
//                    setParam(query, tkn_limit, qMin(per_request, OD_LIMIT_PER_REQUEST));
//                }

                inline QString addPlaylistUrl() { return audioUrl(path_audio_playlist_add); } // params : (name: '') (publicPlaylist: 'true')
                inline QString removePlaylistUrl() { return audioUrl(path_audio_playlist_del); } // params : (pid: '' (id of playlist))
                inline QString getPlaylistsUrl() { return audioUrl(path_audio_playlists); } // params: maybe has uid param ? need to check
                inline QString addToPlaylistsUrl() { return audioUrl(path_audio_to_playlist); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
                inline QString removeFromPlaylistsUrl() { return audioUrl(path_audio_del_from_playlist); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')

                inline QString searchUrl(const QString & predicate) { return audioUrl(path_audio_search, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs
                inline QString searchTracksUrl(const QString & predicate) { return audioUrl(path_audio_search_tracks, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs
                inline QString searchAlbumsUrl(const QString & predicate) { return audioUrl(path_audio_search_albums, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs
                inline QString searchArtistsUrl(const QString & predicate) { return audioUrl(path_audio_search_artists, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs

                inline QString albumAudiosUrl() { return audioUrl(path_audio_by_album_id); } // params : (albumId: album id)
                inline QString artistAudiosUrl() { return audioUrl(path_audio_by_artist_id); } // params : (artistId: artist id)


                inline QString collectionsAudioUrl() { return audioUrl(path_audio_collections); } // params: (collectionId: not used for index of collections) and pagination attrs
                inline QString downloadedAudioUrl() { return audioUrl(path_audio_downloaded); } // params : pagination attrs
                inline QString isDownloadedAudioUrl() { return audioUrl(path_audio_is_downloaded); } // params : (tid: track id)
                inline QString listenedAudioUrl() { return audioUrl(path_audio_history); } // params : pagination attrs
                inline QString popularAudioUrl() { return audioUrl(path_audio_popular_tracks); }

                // artists // tracks // tuners // collections // albums
                inline QString popAudioUrl() { return audioUrl(path_audio_popular); } // (locale: 'ru') and pagination attrs
                inline QString customAudioUrl(const QStringList & ids) { return audioUrl(path_audio_info, QUrlQuery(tkn_ids % ids.join(','))); } // param (ids: ids of (track / album / artist) splited by coma) ? // info request per ids for items
        //        inline QString formaddplUrl() { return audioUrl(path_audio_add_to_playlist_form); } // params: (tid: track id) // used for adding new item to playlist

                // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
                inline QString playAudioUrl(QString tid/*, int item_type = 5*/) { return audioUrl(path_audio_play, QUrlQuery(tkn_tid_eq % tid)); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
                inline QString play30AudioUrl() { return audioUrl(path_audio_play_30_sec); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))

                inline QString tunersUrl() { return audioUrl(path_audio_tuners_for_radio); } // params: (locale: 'ru')  need to check pagination
                inline QString radioUrl(QString /*tuner*/) { return audioUrl(path_audio_radio); } // params: (locale: 'ru') (tuner: taked from tunersUrl() returned list) and pagination attrs

                QJsonValue popular(const SearchLimit & /*limits*/) {
                    return pRequest(
                        popAudioUrl(),
                        call_type_json,
                        rules(),
                        0,
                        proc_json_extract,
                        QStringList() << tkn_tracks
                    );

//                    return lQuery(popAudioUrl(), QueryRules(tkn_tracks, requestLimit(), OD_SEARCH_LIMIT));
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() || limits.by_popularity())
                        return popular(limits);
                    else {
                        PolyQueryRules prules = rules(limits.start_offset, qMin(200, limits.items_limit));

                        if (limits.by_artists())
                            return pRequest(
                                searchArtistsUrl(limits.predicate),
                                call_type_json,
                                prules,
                                0,
                                proc_json_extract,
                                QStringList() << tkn_artists
                            );

//                            return lQuery(searchArtistsUrl(limits.predicate), QueryRules(tkn_artists, qMin(requestLimit(), limits.total_limit), qMin(OD_SEARCH_LIMIT, limits.total_limit)));
                        else if (limits.by_songs_name())
                            return pRequest(
                                searchTracksUrl(limits.predicate),
                                call_type_json,
                                prules,
                                0,
                                proc_json_extract,
                                QStringList() << tkn_tracks
                            );


//                            return lQuery(searchTracksUrl(limits.predicate), QueryRules(tkn_tracks, qMin(requestLimit(), limits.total_limit), qMin(OD_SEARCH_LIMIT, limits.total_limit)));
                        else
                            return pRequest(
                                searchUrl(limits.predicate),
                                call_type_json,
                                prules,
                                0,
                                proc_json_extract,
                                QStringList() << tkn_tracks
                            );

//                            return lQuery(searchUrl(limits.predicate), QueryRules(tkn_tracks, qMin(requestLimit(), limits.total_limit), qMin(OD_SEARCH_LIMIT, limits.total_limit)));
                    }
                }
            public:
                inline virtual ~RequestApi() {}

                inline QString initAudioUrl() { return audioUrl(path_audio_init); }
                inline QString myAudioUrl(const QString & uid) { return audioUrl(tkn_my, QUrlQuery(tkn_uid_eq % uid)); } // params: (uid: sets for friend request) and pagination attrs
                QJsonObject userInfo(const QString & uid) {
                    if (uid.isEmpty()) {
                        qDebug() << initAudioUrl();
                        return Manager::prepare() -> jsonGet(initAudioUrl());
                    } else
                        return Manager::prepare() -> jsonGet(myAudioUrl(uid));
                }

                inline QString playlistAudioUrl(const QString & pid) { return audioUrl(tkn_my, QUrlQuery(tkn_pid_eq % pid)); } // params: (pid: playlist id) and pagination attrs
                QJsonArray playlistInfo(const QString & pid, int count) {
                    return pRequest(
                        playlistAudioUrl(pid),
                        call_type_json,
                        rules(0, count),
                        0,
                        proc_json_extract,
                        QStringList() << tkn_tracks
                    );
                }

                QJsonArray audioInfo(const QStringList & uids) { // take only tracks - response also contains artists and albums arrays
                    return sRequest(
                        customAudioUrl(uids),
                        call_type_json
                    ).value(tkn_tracks).toArray();
                }
            };
        }
    }
}

#endif // OD_REQUEST_API
