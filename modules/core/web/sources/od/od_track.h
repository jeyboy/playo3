#ifndef OD_TRACK
#define OD_TRACK

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Track : public Base {
            protected:
                QString trackUrl(const QString & track_id) {
                    QJsonObject obj = Manager::prepare() -> jsonGet(
                        audioUrlStr(
                            path_audio_play,
                            { {tkn_tid, track_id} }
                        )
                    );
//                    if (hasError(obj)) {
//                        connectUser();
//                        obj = Manager::prepare() -> jsonGet(playAudioUrl(refresh_page));
//                        qDebug() << "RECONECTION";
//                    }
                    QUrl url(obj.value(tkn_play).toString());
                    QUrlQuery query = QUrlQuery(url.query());
                    query.addQueryItem(tkn_client_hash, calcMagicNumber(query.queryItemValue(tkn_md5)));
                    url.setQuery(query);
                    return url.toString();
                }

                bool trackIsDownloaded(const QString & track_id) { //TODO: not finished
                    QJsonObject obj = Manager::prepare() -> jsonGet(
                        audioUrlStr(
                            path_audio_is_downloaded,
                            { {tkn_tid, track_id} }
                        )
                    );

                    return false;
                }

                QJsonValue tracksInfo(const QStringList & uids) {
                    return sRequest(
                        audioInfoUrl(uids),
                        call_type_json
                    ).value(tkn_tracks).toArray();
                }

//                // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
//                inline QString playAudioUrl(QString tid/*, int item_type = 5*/) { return audioUrlStr(path_audio_play, QUrlQuery(tkn_tid_eq % tid)); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
//                inline QString play30AudioUrl() { return audioUrlStr(path_audio_play_30_sec); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))
//                inline QString isDownloadedAudioUrl() { return audioUrlStr(path_audio_is_downloaded); } // params : (tid: track id)

//                inline QString searchTracksUrl(const QString & predicate) { return audioUrlStr(path_audio_search_tracks, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs



                QJsonValue tracksSearch(const SearchLimit & limits) {
                    PolyQueryRules prules = rules(limits.start_offset, qMin(200, limits.items_limit));

                    if (limits.by_artists())
                        return pRequest(
                            audioUrlStr(
                                path_audio_search_artists,
                                { {tkn_q, limits.predicate} }
                            ),
                            call_type_json, prules, 0,
                            proc_json_extract, QStringList() << tkn_artists
                        );
                    else if (limits.by_songs_name())
                        return pRequest(
                            audioUrlStr(
                                path_audio_search_tracks,
                                { {tkn_q, limits.predicate} }
                            ),
                            call_type_json, prules, 0,
                            proc_json_extract, QStringList() << tkn_tracks
                        );
                    else
                        return pRequest(
                            audioSearchUrl(limits.predicate),
                            call_type_json, prules, 0,
                            proc_json_extract, QStringList() << tkn_tracks
                        );
                }

                QJsonValue tracksByArtist(const QString & artist_id) { //TODO: not finished
                    return pRequest(
                        audioUrlStr(
                            path_audio_by_artist_id,
                            { {QStringLiteral("artistId"), artist_id} }
                        ),
                        call_type_json, rules(), 0,
                        proc_json_extract, QStringList() << tkn_artists
                    );
                }

                QJsonValue tracksByPlaylist(const QString & playlist_id) { //TODO: not finished
                    return pRequest(
                        audioUrlStr(
                            path_audio_by_album_id,
                            { {QStringLiteral("albumId"), playlist_id} }
                        ),
                        call_type_json, rules(), 0,
                        proc_json_extract, QStringList() << tkn_artists
                    );
                }

//                inline QString myAudioUrl(const QString & uid) { return audioUrlStr(tkn_my, QUrlQuery(tkn_uid_eq % uid)); } // params: (uid: sets for friend request) and pagination attrs

                QJsonValue tracksByUser(const QString & user_id, int count = DEFAULT_ITEMS_LIMIT, int offset = 0) { //TODO: not finished
                    return pRequest(
                        audioUrlStr(tkn_my, {{tkn_uid, user_id}}),
                        call_type_json, rules(offset, count), 0,
                        proc_json_extract, QStringList() << tkn_tracks
                    );
                }
            };
        }
    }
}

#endif // OD_TRACK
