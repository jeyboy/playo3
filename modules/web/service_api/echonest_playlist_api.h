#ifndef ECHONEST_PLAYLIST_API
#define ECHONEST_PLAYLIST_API

#include "iapi.h"

class EchonestPlaylistApi : public IApi {
    public:
        //type 	no 	no 	artist-radio, song-radio , genre-radio 	the type of the playlist to be generated. See below for details on each of the types - the type of playlist to be generated

        //    Playlist Types:

        //            artist-radio - plays songs for the given artists and similar artists
        //            song-radio - plays songs similar to the song specified.
        //            genre-radio - plays songs from artists matching the given genre

        //bucket id:catalog-name, tracks
        inline QUrl playlistBasicUrl(QString & type, QStringList & artists, QStringList & genres, QStringList & songs_ids, int limit = 100) {
            QUrl url(baseUrl("playlist/basic"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, qMin(limit, requestLimit()), 0);

            setParam(query, "artist", artists);
            setParam(query, "genre", genres);
            setParam(query, "song_id", songs_ids);

            if (!type.isEmpty())
                setParam(query, "type", type);
            url.setQuery(query);
            return url;
        }

        QJsonArray playlistBasic(QString type = QString(), QStringList artists = QStringList(),
                                 QStringList genres = QStringList(), QStringList songs_ids = QStringList(), int limit = 100) {
            QJsonObject response;

            if (proceedQuery(playlistBasicUrl(type, artists, genres, songs_ids, limit), response))
                return response.value("response").toObject().value("songs").toArray();

            return QJsonArray();
        }

        //{
        //    "response": {
        //        "songs": [
        //            {
        //                "artist_foreign_ids": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:artist:7516"
        //                    }
        //                ],
        //                "artist_id": "AR633SY1187B9AC3B9",
        //                "artist_name": "Weezer",
        //                "id": "SOBSLVH12A8C131F38",
        //                "title": "Island In The Sun",
        //                "tracks": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:track:20637990",
        //                        "foreign_release_id": "7digital-US:release:1914387",
        //                        "id": "TRTXLYU13A79B0B112",
        //                        "preview_url": "http://previews.7digital.com/clips/34/20637990.clip.mp3",
        //                        "release_image": "http://cdn.7static.com/static/img/sleeveart/00/019/143/0001914387_200.jpg"
        //                    }
        //                ]
        //            },
        //            {
        //                "artist_foreign_ids": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:artist:94744"
        //                    }
        //                ],
        //                "artist_id": "ARQRQRA12131B4B5A8",
        //                "artist_name": "Cake",
        //                "id": "SOZXNWX131343927EA",
        //                "title": "Love You Madly",
        //                "tracks": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:track:3156513",
        //                        "foreign_release_id": "7digital-US:release:282089",
        //                        "id": "TRDFWLB12E5ADFBA5F",
        //                        "preview_url": "http://previews.7digital.com/clips/34/3156513.clip.mp3",
        //                        "release_image": "http://cdn.7static.com/static/img/sleeveart/00/002/820/0000282089_200.jpg"
        //                    }
        //                ]
        //            }
        //        ],
        //        "status": {
        //            "code": 0,
        //            "message": "Success",
        //            "version": "4.2"
        //        }
        //    }
        //}





//      type artist, artist-radio, song-radio, catalog, catalog-radio, genre-radio
//      song_selection 	PARAMETER-SUFFIX where PARAMETER can be one of song_hotttnesss, song_discovery, song_currency, acousticness, tempo, energy, danceability, liveness, speechiness, duration, loudness, valence, and SUFFIX is one of '-top' or '-bottom'
//      variety	0 - 1 (default = 0.5) 	the maximum variety of artists to be represented in the playlist. A higher number will allow for more variety in the artists.
//      distribution focused, wandering 	Controls the distribution of artists in the playlist. A focused distribution yields a playlist of songs that are tightly clustered around the seeds, whereas a wandering distribution yields a playlist from a broader range of artists.
//      adventurousness	0 - 1 (default = 0.2) 	Controls the trade between known music and unknown music. A value of 0 means no adventurousness, only known and preferred music will be played. A value of 1 means high adventurousness, mostly unknown music will be played. A value of auto indicates that the adventurousness should be automatically determined based upon the taste profile of the user. This parameter only applies to catalog and catalog-radio type playlists.
//      seed_catalog CAKSMUX1321A708AA4 	ID of seed catalog for the playlist
//      song_type 	no 	yes 	christmas, live, studio, acoustic, electric.
//      bucket 	no 	yes 	audio_summary, artist_discovery, artist_discovery_rank, artist_familiarity, artist_familiarity_rank, artist_hotttnesss, artist_hotttnesss_rank, artist_location, song_currency, song_currency_rank, song_hotttnesss, song_hotttnesss_rank, song_type, tracks, id:Rosetta-space



//      type 	no 	no 	artist-description 	the type of the playlist to be generated. If set to 'artist-description' plays songs from artists matching the given description, style or mood
//      description 	no 	yes 	alt-rock,-emo,harp^2 	for playlists of type artist-description select artists that tend to match this given description
//      style 	no 	yes 	jazz, metal^2 	for playlists of type artist-description select artists that tend to match this given style
//      mood 	no 	yes 	happy, sad^.5 	for playlists of type artist-description select artists that tend to match this given mood



//      Generates a 20 song playlist of popular music from the 70s sorted by increasing tempo
//      http://developer.echonest.com/api/v4/playlist/static?api_key=TSCA6XDZTJQ1OOJSV&description=70s&description=disco&type=artist-description&artist_min_familiarity=.7&sort=tempo-asc&results=20

        inline QUrl playlistStaticUrl(QString & type, QString & song_selection_creteria, float variety, QString & distribution, float adventurousness,
                                      QString & seed_catalog, QStringList & artists, QStringList & genres, QStringList & songs_ids,
                                      QStringList & song_types, int limit = 100) {
            QUrl url(baseUrl("playlist/static"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, qMin(limit, requestLimit()), 0);

            setParam(query, "artist", artists);
            setParam(query, "genre", genres);
            setParam(query, "song_id", songs_ids);

            if (!song_types.isEmpty()) setParam(query, "song_types", song_types);
            if (!seed_catalog.isEmpty()) setParam(query, "seed_catalog", seed_catalog);
            if (!type.isEmpty()) setParam(query, "type", type);
            if (!song_selection_creteria.isEmpty()) setParam(query, "song_selection", song_selection_creteria);
            if (!distribution.isEmpty()) setParam(query, "distribution", distribution);
            setParam(query, "variety", QString::number(variety));
            setParam(query, "adventurousness", QString::number(adventurousness));

            url.setQuery(query);
            return url;
        }

        QJsonArray playlistStatic(QString type = QString(), QString song_selection_creteria = QString(), float variety = .5,
                                  QString distribution = QString(), float adventurousness = .2,
                                  QString seed_catalog = QString(), QStringList artists = QStringList(),
                                  QStringList genres = QStringList(), QStringList songs_ids = QStringList(),
                                  QStringList song_types = QStringList(), int limit = 100) {
            QJsonObject response;

            if (proceedQuery(playlistStaticUrl(type, song_selection_creteria, variety, distribution, adventurousness,
                                               seed_catalog, artists, genres, songs_ids, song_types, limit), response))
                return response.value("response").toObject().value("songs").toArray();

            return QJsonArray();
        }

        //{
        //    "response": {
        //        "status": {
        //            "version": "4.2",
        //            "code": 0,
        //            "message": "Success"
        //        },
        //        "songs": [
        //            {
        //                "title": "Dreamin'",
        //                "artist_name": "Weezer",
        //                "artist_id": "AR633SY1187B9AC3B9",
        //                "id": "SOHBJKR1280EC909D4"
        //            },
        //            {
        //                "title": "This Is A Call'",
        //                "artist_name": "Foo Fighters",
        //                "artist_id": "AR6XPWV1187B9ADAEB",
        //                "id": "SOMPOYK1280ED5098B"
        //            },
        //            {
        //                "title": "Life in a glasshouse",
        //                "artist_name": "Radiohead",
        //                "artist_id": "ARH6W4X1187B99274F",
        //                "id": "SOAPCKQ127F3E1B7A8"
        //             }
        //        ]
        //    }
        //}



        // max_duration 	no 	no 	0.0 < duration < 3600.0 (seconds) 	the maximum duration of any song on the playlist
        // min_duration 	no 	no 	0.0 < duration < 3600.0 (seconds) 	the minimum duration of any song on the playlist

        // max_danceability 	no 	no 	0.0 < danceability < 1.0 	the maximum danceability of any song
        // min_danceability 	no 	no 	0.0 < danceability < 1.0 	the minimum danceability of any song

        // max_liveness 	no 	no 	0.0 < liveness < 1.0 	the maximum liveness of any song
        // min_liveness 	no 	no 	0.0 < liveness < 1.0 	the minimum liveness of any song

        // max_speechiness 	no 	no 	0.0 < speechiness < 1.0 	the maximum speechiness of any song
        // min_speechiness 	no 	no 	0.0 < speechiness < 1.0 	the minimum speechiness of any song

        // max_acousticness 	no 	no 	0.0 < acousticness < 1.0 	the maximum acousticness of any song. This is a beta feature.
        // min_acousticness 	no 	no 	0.0 < acousticness < 1.0 	the minimum acousticness of any song. This is a beta feature.

        // artist_max_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the maximum artist familiarity for songs in the playlist
        // artist_min_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the minimum artist familiarity for songs in the playlist

        // artist_max_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the maximum artist hotttness for songs in the playlist
        // artist_min_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the minimum artist hotttnesss for songs in the playlist

        // artist_start_year_before 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year before the given value
        // artist_start_year_after 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year after the given value

        // artist_end_year_before 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year before the given value
        // artist_end_year_after 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year after the given value

        // song_max_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the maximum hotttnesss for songs in the playlist
        // song_min_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the minimum hotttnesss for songs in the playlist

        // bucket 	no 	yes 	audio_summary, artist_discovery, artist_discovery_rank, artist_familiarity, artist_familiarity_rank, artist_hotttnesss, artist_hotttnesss_rank, artist_location, song_currency, song_currency_rank, song_hotttnesss, song_hotttnesss_rank, song_type, tracks, id:Rosetta-space 	indicates what data should be returned in the playlist (for json and xml types only)



        // sort 	tempo-asc, duration-asc, loudness-asc, speechiness-asc, acousticness-asc, liveness-asc, artist_familiarity-asc, artist_hotttnesss-asc, artist_start_year-asc, artist_start_year-desc, artist_end_year-asc, artist_end_year-desc, song_hotttnesss-asc, latitude-asc, longitude-asc, mode-asc, key-asc, tempo-desc, duration-desc, loudness-desc, liveness-desc, speechiness-desc, acousticness-desc, artist_familiarity-desc, artist_hotttnesss-desc, song_hotttnesss-desc, latitude-desc, longitude-desc, mode-desc, key-desc, energy-asc, energy-desc, danceability-asc, danceability-desc
        // genre_preset	core-best, core-shuffled, in_rotation-best, in_rotation-shuffled, emerging-best, emerging-shuffled
        // max_tempo 	no 	no 	0.0 < tempo < 500.0 (BPM) 	the maximum tempo for any included songs
        // min_tempo 	no 	no 	0.0 < tempo < 500.0 (BPM) 	the minimum tempo for any included songs
        // max_loudness 	no 	no 	-100.0 < loudness < 100.0 (dB) 	the maximum loudness of any song on the playlist
        // min_loudness 	no 	no 	-100.0 < loudness < 100.0 (dB) 	the minimum loudness of any song on the playlist
        // max_energy 	no 	no 	0.0 < energy < 1.0 	the maximum energy of any song
        // min_energy 	no 	no 	0.0 < energy < 1.0 	the minimum energy of any song

        inline QUrl playlistStaticExUrl(QString & type, QString & song_selection_creteria, float variety, QString & distribution, float adventurousness,
                                      QString & seed_catalog, QStringList & artists, QStringList & genres, QStringList & songs_ids,
                                      QStringList & song_types, QString & sort, QString & genre_preset, float min_tempo = 0, float max_tempo = 500,
                                      float min_loudness = -100, float max_loudness = 100, float min_energy = 0, float max_energy = 1,
                                      int limit = 100) {
            QUrl url(baseUrl("playlist/static"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, qMin(limit, requestLimit()), 0);

            setParam(query, "artist", artists);
            setParam(query, "genre", genres);
            setParam(query, "song_id", songs_ids);

            setParam(query, "min_tempo", min_tempo);
            setParam(query, "max_tempo", max_tempo);

            setParam(query, "min_loudness", min_loudness);
            setParam(query, "max_loudness", max_loudness);

            setParam(query, "min_energy", min_energy);
            setParam(query, "max_energy", max_energy);

            if (!genre_preset.isEmpty()) setParam(query, "genre_preset", genre_preset);
            if (!sort.isEmpty()) setParam(query, "sort", sort);
            if (!song_types.isEmpty()) setParam(query, "song_types", song_types);
            if (!seed_catalog.isEmpty()) setParam(query, "seed_catalog", seed_catalog);
            if (!type.isEmpty()) setParam(query, "type", type);
            if (!song_selection_creteria.isEmpty()) setParam(query, "song_selection", song_selection_creteria);
            if (!distribution.isEmpty()) setParam(query, "distribution", distribution);
            setParam(query, "variety", QString::number(variety));
            setParam(query, "adventurousness", QString::number(adventurousness));

            url.setQuery(query);
            return url;
        }

        QJsonArray playlistStaticEx(QString type = QString(), QString song_selection_creteria = QString(), float variety = .5,
                                  QString distribution = QString(), float adventurousness = .2,
                                  QString seed_catalog = QString(), QStringList artists = QStringList(),
                                  QStringList genres = QStringList(), QStringList songs_ids = QStringList(),
                                  QStringList song_types = QStringList(), int limit = 100) {
            QJsonObject response;

            if (proceedQuery(playlistStaticUrl(type, song_selection_creteria, variety, distribution, adventurousness,
                                               seed_catalog, artists, genres, songs_ids, song_types, limit), response))
                return response.value("response").toObject().value("songs").toArray();

            return QJsonArray();
        }
};

#endif // ECHONEST_PLAYLIST_API
