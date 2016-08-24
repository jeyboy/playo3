#ifndef OD_TRACK
#define OD_TRACK

#include "od_defines.h"

#define TRACKS_AMOUNT_LIMIT 100

namespace Core {
    namespace Web {
        namespace Od {
            class Track : public virtual Base {
            public:
                QString trackUrl(const QString & track_id) {
                    QJsonObject obj = sRequest(
                        audioUrlStr(
                            path_audio_play,
                            { {tkn_tid, track_id} }
                        ),
                        call_type_json
                    );

                    QUrl url(obj.value(tkn_play).toString());
                    QUrlQuery query = QUrlQuery(url.query());
                    query.addQueryItem(tkn_client_hash, calcMagicNumber(query.queryItemValue(tkn_md5)));
                    url.setQuery(query);
                    return url.toString();
                }

                bool trackIsDownloaded(const QString & track_id) { //TODO: not finished
                    QJsonObject obj = sRequest(
                        audioUrlStr(
                            path_audio_is_downloaded,
                            { {tkn_tid, track_id} }
                        ),
                        call_type_json
                    );

                    return false;
                }

                QJsonValue tracksInfo(const QStringList & uids) { // need to split ids by groups with max size eql to 40 ids
                    QJsonArray block_content = sRequest(audioInfoUrl(uids), call_type_json)
                        .value(tkn_tracks).toArray();
                    return prepareBlock(dmt_audio, block_content);
                }

//                // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
//                inline QString playAudioUrl(QString tid/*, int item_type = 5*/) { return audioUrlStr(path_audio_play, QUrlQuery(tkn_tid_eq % tid)); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
//                inline QString play30AudioUrl() { return audioUrlStr(path_audio_play_30_sec); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))
//                inline QString isDownloadedAudioUrl() { return audioUrlStr(path_audio_is_downloaded); } // params : (tid: track id)

//                inline QString searchTracksUrl(const QString & predicate) { return audioUrlStr(path_audio_search_tracks, QUrlQuery(tkn_q_eq % predicate)); } // params : (q: predicate) and pagination attrs

                QJsonValue tracksSearch(const QUrlQuery & args) { return tracksSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue tracksSearch(const SearchLimit & limits) {
                    PolyQueryRules prules = rules(limits.start_offset, qMax(TRACKS_AMOUNT_LIMIT, qMin(200, limits.items_limit)), limits.requests_limit);
                    QueriableResponse response;

                    if (limits.by_artists())
                        response = pRequest(
                            audioUrlStr(
                                path_audio_search_artists,
                                { {tkn_q, limits.predicate} }
                            ),
                            call_type_json, prules, 0,
                            proc_json_extract, QStringList() << tkn_artists
                        );
                    else //if (limits.by_songs_name())
                        response = pRequest(
                            audioUrlStr(
                                path_audio_search_tracks,
                                { {tkn_q, limits.predicate} }
                            ),
                            call_type_json, prules, 0,
                            proc_json_extract, QStringList() << tkn_tracks
                        );
//                    else // this block is related to serach by artists
//                        response = pRequest(
//                            audioSearchUrl(limits.predicate),
//                            call_type_json, prules, 0,
//                            proc_json_extract, QStringList() << tkn_tracks
//                        );

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_search, response, limits);
                }

//                QJsonValue tracksByArtist(const QUrlQuery & args) {
//                    return tracksByArtist(
//                        args.queryItemValue(CMD_ID),
//                        args.queryItemValue(CMD_OFFSET).toInt(),
//                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
//                    );
//                }
//                QJsonValue tracksByArtist(const QString & artist_id, int offset = 0, int count = TRACKS_AMOUNT_LIMIT) {
//                    QueriableResponse response = pRequest(
//                                audioUrlStr(
//                                    path_audio_by_artist_id,
//                                    { {QStringLiteral("artistId"), artist_id} }
//                                ),
//                                call_type_json, rules(offset, qMax(count, TRACKS_AMOUNT_LIMIT)), 0,
//                                proc_json_extract, QStringList() << tkn_tracks // similarArtists
//                            );

//                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_artist, response, {{CMD_ID, artist_id}});
//                }

                QJsonValue tracksByArtist(const QUrlQuery & args) {
                    return tracksByArtist(args.queryItemValue(CMD_ID));
                }
                QJsonValue tracksByArtist(const QString & artist_id) {
                    QJsonObject response = sRequest(
                                audioUrlStr(
                                    path_audio_by_artist_id,
                                    { {QStringLiteral("artistId"), artist_id} }
                                ),
                                call_type_json
                            );

                    QJsonArray blocks;

                    QJsonArray similar_artists = response.value(QStringLiteral("similarArtists")).toArray();
                    if (!similar_artists.isEmpty()) {
                        prepareArtists(similar_artists);
                        blocks << prepareBlock(dmt_audio_set, similar_artists, {{tkn_dir_name, QStringLiteral("Similar Artists")}});
                    }

                    QJsonArray albums = response.value(QStringLiteral("masterAlbums")).toArray();
                    prepareAlbums(albums);
                    blocks << prepareBlock(dmt_audio_set, albums, {{tkn_dir_name, QStringLiteral("Albums")}});

                    blocks << prepareBlock(dmt_audio, response.value(tkn_tracks));

                    return blocks;
                }

                QJsonValue tracksByCollection(const QUrlQuery & args) {
                    return tracksByCollection(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByCollection(const QString & collection_id, int offset = 0, int count = TRACKS_AMOUNT_LIMIT) {
                    QueriableResponse response = pRequest(
                        audioUrlStr(
                            path_audio_collection,
                            { {QStringLiteral("collectionId"), collection_id} }
                        ),
                        call_type_json, rules(offset, qMax(count, TRACKS_AMOUNT_LIMIT)), 0,
                        proc_json_extract, QStringList() << tkn_tracks
                    );

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_collection, response, {{CMD_ID, collection_id}});
                }

                QJsonValue tracksByTuner(const QUrlQuery & args) {
                    return tracksByTuner(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByTuner(const QString & tuner_id, int offset = 0, int count = TRACKS_AMOUNT_LIMIT, const QString & locale = QStringLiteral("ru")) { //TODO: need to check
                    QueriableResponse response = pRequest( // artists // tracks //albums
                        audioUrlStr(
                            path_audio_radio,
                            {
                                { tkn_tuner, tuner_id },
                                { tkn_locale, locale }
                            }
                        ),
                        call_type_json, rules(offset, qMax(TRACKS_AMOUNT_LIMIT, count)), 0,
                        proc_json_extract, QStringList() << tkn_tracks
                    );

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_tuner, response, {{CMD_ID, tuner_id}});
                }

                QJsonValue tracksByAlbum(const QUrlQuery & args) {
                    return tracksByAlbum(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByAlbum(const QString & album_id, int offset = 0, int count = TRACKS_AMOUNT_LIMIT) {
                    QueriableResponse response = pRequest(
                        audioUrlStr(
                            path_audio_by_album_id,
                            { {QStringLiteral("albumId"), album_id} }
                        ),
                        call_type_json, rules(offset, qMax(TRACKS_AMOUNT_LIMIT, count)), 0,
                        proc_json_extract, QStringList() << tkn_tracks
                    );

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_album, response, {{CMD_ID, album_id}});
                }

                QJsonValue tracksByPlaylist(const QUrlQuery & args) {
                    return tracksByPlaylist(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByPlaylist(const QString & playlist_id, int offset = 0, int count = TRACKS_AMOUNT_LIMIT) {
                    QueriableResponse response = pRequest(
                        audioUrlStr(
                            tkn_my,
                            {{ tkn_pid, playlist_id }}
                        ),
                        call_type_json, rules(offset, qMax(TRACKS_AMOUNT_LIMIT, count)),
                        0, proc_json_extract, QStringList() << tkn_tracks
                    );

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_playlist, response, {{CMD_ID, playlist_id}});
                }

                QJsonValue tracksByUser(const QUrlQuery & args) {
                    return tracksByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByUser(const QString & user_id, int offset = 0, int count = TRACKS_AMOUNT_LIMIT) {
                    QueriableResponse response = pRequest(
                        audioUrlStr(tkn_my, {{tkn_uid, user_id}}),
                        call_type_json, rules(offset, qMax(TRACKS_AMOUNT_LIMIT, count)), 0,
                        proc_json_extract, QStringList() << tkn_tracks
                    );

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_user, response, {{CMD_ID, user_id}});
                }

                QJsonValue userRecommendations(const QUrlQuery & /*args*/) {
                    return userRecommendations(
//                        args.queryItemValue(CMD_ID)
//                        args.queryItemValue(CMD_OFFSET).toInt(),
//                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue userRecommendations() {
                    QJsonObject response = sRequest(
                                audioUrlStr(
                                    path_audio_similar_by_playlist,
                                    { {tkn_count, 30} }
                                ),
                                call_type_json
                            );

                    QJsonArray blocks;

                    QJsonArray similar_artists = response.value(tkn_artists).toArray();
                    if (!similar_artists.isEmpty()) {
                        prepareArtists(similar_artists);
                        blocks << prepareBlock(dmt_audio_set, similar_artists, {{tkn_dir_name, QStringLiteral("Artists")}});
                    }

                    QJsonArray albums = response.value(tkn_albums).toArray();
                    if (!albums.isEmpty()) {
                        prepareAlbums(albums);
                        blocks << prepareBlock(dmt_audio_set, albums, {{tkn_dir_name, QStringLiteral("Albums")}});
                    }

                    blocks << prepareBlock(
                        dmt_audio, response.value(tkn_tracks),
                        {{
                            tkn_more_cmd, Cmd::build(
                              sourceType(), cmd_mtd_user_recommendations, {}
                            ).toString()
                        }}
                    );

                    return blocks;
                }
            };
        }
    }
}

#endif // OD_TRACK
