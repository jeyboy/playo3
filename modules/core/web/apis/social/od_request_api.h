#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include "od_api_misc.h"
#include "modules/core/web/interfaces/teu_auth.h"

#define OD_OFFSET_LIMIT 2000
#define OD_SEARCH_LIMIT 200

namespace Core {
    namespace Web {
        namespace Od {
            class RequestApi : public Misc, public TeuAuth {
            protected:
                RequestApi() {}

                bool sessionIsValid() { return !hasError(Manager::prepare() -> getJson(initAudioUrl())); }

                inline QUrl authRequestUrl(const QString & email, const QString & pass) const {
                    return QUrl(url_base_auth % path_auth.arg(encodeStr(email), encodeStr(pass)));
                }

                inline QUrl initUrl() const { return QUrl(url_base_auth % path_auth2 % additional()); }

                inline QUrl audioUrl(const QString func, const QUrlQuery & query = QUrlQuery()) {
                    QUrl url(url_base_audio % func % tkn_coma_dot % genDefaultParams().toString());
                    url.setQuery(query);
                    return url;
                }

                inline QUrl addPlaylistUrl() { return audioUrl(path_audio_playlist_add); } // params : (name: '') (publicPlaylist: 'true')
                inline QUrl removePlaylistUrl() { return audioUrl(path_audio_playlist_del); } // params : (pid: '' (id of playlist))
                inline QUrl getPlaylistsUrl() { return audioUrl(path_audio_playlists); } // params: maybe has uid param ? need to check
                inline QUrl addToPlaylistsUrl() { return audioUrl(path_audio_to_playlist); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
                inline QUrl removeFromPlaylistsUrl() { return audioUrl(path_audio_del_from_playlist); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')

                inline QUrl searchUrl(const QString & predicate) { return audioUrl(path_audio_search, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs
                inline QUrl searchTracksUrl(const QString & predicate) { return audioUrl(path_audio_search_tracks, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs
                inline QUrl searchAlbumsUrl(const QString & predicate) { return audioUrl(path_audio_search_albums, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs
                inline QUrl searchArtistsUrl(const QString & predicate) { return audioUrl(path_audio_search_artists, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs

                inline QUrl albumAudiosUrl() { return audioUrl(path_audio_by_album_id); } // params : (albumId: album id)
                inline QUrl artistAudiosUrl() { return audioUrl(path_audio_by_artist_id); } // params : (artistId: artist id)


                inline QUrl collectionsAudioUrl() { return audioUrl(path_audio_collections); } // params: (collectionId: not used for index of collections) and pagination attrs
                inline QUrl downloadedAudioUrl() { return audioUrl(path_audio_downloaded); } // params : pagination attrs
                inline QUrl isDownloadedAudioUrl() { return audioUrl(path_audio_is_downloaded); } // params : (tid: track id)
                inline QUrl listenedAudioUrl() { return audioUrl(path_audio_history); } // params : pagination attrs
                inline QUrl popularAudioUrl() { return audioUrl(path_audio_popular_tracks); }

                // artists // tracks // tuners // collections // albums
                inline QUrl popAudioUrl() { return audioUrl(path_audio_popular); } // (locale: 'ru') and pagination attrs
                inline QUrl customAudioUrl() { return audioUrl(path_audio_info); } // param (ids: ids of (track / album / artist) splited by coma) ? // info request per ids for items
        //        inline QUrl formaddplUrl() { return audioUrl(path_audio_add_to_playlist_form); } // params: (tid: track id) // used for adding new item to playlist

                // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
                inline QUrl playAudioUrl(QString tid/*, int item_type = 5*/) { return audioUrl(path_audio_play, QUrlQuery(tkn_tid_eq % tid)); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
                inline QUrl play30AudioUrl() { return audioUrl(path_audio_play_30_sec); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))

                inline QUrl tunersUrl() { return audioUrl(path_audio_tuners_for_radio); } // params: (locale: 'ru')  need to check pagination
                inline QUrl radioUrl(QString /*tuner*/) { return audioUrl(path_audio_radio); } // params: (locale: 'ru') (tuner: taked from tunersUrl() returned list) and pagination attrs

                QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
                    if (predicate.isEmpty() || limitations.by_popularity())
                        return lQuery(popAudioUrl(), QueryRules(tkn_tracks, requestLimit()));
                    else {
                        if (limitations.by_artists())
                            return lQuery(searchArtistsUrl(predicate), QueryRules(tkn_artists, requestLimit(), OD_SEARCH_LIMIT));
                        else if (limitations.by_songs())
                            return lQuery(searchTracksUrl(predicate), QueryRules(tkn_tracks, requestLimit(), OD_SEARCH_LIMIT));
                        else
                            return lQuery(searchUrl(predicate), QueryRules(tkn_tracks, requestLimit(), OD_SEARCH_LIMIT));
                    }
                }
            public:
                inline virtual ~RequestApi() {}

                inline QUrl initAudioUrl() { return audioUrl(path_audio_init); }
                inline QUrl myAudioUrl(const QString & uid) { return audioUrl(tkn_my, QUrlQuery(tkn_uid_eq % uid)); } // params: (uid: sets for friend request) and pagination attrs
                QJsonObject userInfo(const QString & uid) {
                    if (uid.isEmpty()) {
                        qDebug() << initAudioUrl();
                        return Manager::prepare() -> getJson(initAudioUrl());
                    } else
                        return Manager::prepare() -> getJson(myAudioUrl(uid));
                }

                inline QUrl playlistAudioUrl(const QString & pid) { return audioUrl(tkn_my, QUrlQuery(tkn_pid_eq % pid)); } // params: (pid: playlist id) and pagination attrs
                QJsonArray playlistInfo(QString & pid, int count) {
                    return lQuery(
                        playlistAudioUrl(pid),
                        QueryRules(tkn_tracks, requestLimit(), qMin(count, OD_OFFSET_LIMIT))
                    );
                }
            };
        }
    }
}

#endif // OD_REQUEST_API
