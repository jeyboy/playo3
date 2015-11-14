#ifndef ECHONEST_SONG_API
#define ECHONEST_GENRE_API

#include "modules/core/web/interfaces/iapi.h"

//POSSIBLE BUCKETS

//audio_summary 	returns summary audio parameters for the song
//artist_discovery 	returns the discovery score for the song's artist. This is a measure of how unexpectedly popular the artist is.
//artist_discovery_rank 	returns the discovery rank for the song's artist
//artist_familiarity 	returns the familiarity for the song's artist
//artist_familiarity_rank 	returns the familiarity rank for the song's artist
//artist_hotttnesss 	returns the hotttnesss for the song's artist
//artist_hotttnesss_rank 	returns the hotttnesss rank for the song's artist
//artist_location 	returns information about the location of origin for the song's artist
//song_currency 	returns the currency score of the song. This is a measure of how recently popular the song is.
//song_currency_rank 	returns the currency rank of the song.
//song_discovery 	returns the discovery score of the song. This is a measure of how unexpectedly popular the song is.
//song_discovery_rank 	returns the discovery rank of the song.
//song_hotttnesss 	returns the hotttnesss of the song
//song_hotttnesss_rank 	returns the hotttnesss rank of the song
//song_type 	returns a list of song types for the song. Possible song types returned are: 'christmas', 'live' 'studio', 'acoustic', and 'electric'
//tracks 	returns detailed track information for the song. You must also specify a Rosetta id space such as 7digital-US.
//id:rosetta-catalog 	returns catalog specific information about the song for the given catalog. See Project Rosetta Stone for details.
//id:Personal-Catalog-ID 	returns personal catalog specific information about the song for the given catalog. See Project Rosetta Stone for details.

namespace Core {
    namespace Web {
        namespace Echonest {
            class SongApi : virtual public IApi {
                public:
                    inline virtual ~SongApi() {}

                    //    description 	no 	yes 	Some examples are: alt-rock,-emo, harp^2. 	a description of the artist.
                    //    style 	no 	yes 	jazz, metal^2 	a musical style or genre like rock, jazz, or funky. See the method list_terms for details on what styles are currently available
                    //    mood 	no 	yes 	happy, sad^.5 	a mood like happy or sad. See the method list_terms for details on what moods are currently available

                    //    combined 	no 	no 	radiohead karma police 	query both artist and title fields
                    //    min_longitude 	no 	no 	-180.0 < longitude < 180.0 	the minimum longitude of the primary artist location
                    //    max_longitude 	no 	no 	-180.0 < longitude < 180.0 	the maximum longitude of the primary artist location
                    //    min_latitude 	no 	no 	-90.0 < latitude < 90.0 	the minimum latitude of the primary artist location
                    //    max_latitude 	no 	no 	-90.0 < latitude < 90.0 	the maximum latitude of the primary artist location
                    //    rank_type 	no 	no 	relevance, familiarity 	For search by description, style or mood indicates whether results should be ranked by query relevance or by artist familiarity

                    //    artist_id 	no 	no 	ARH6W4X1187B99274F 	the artist ID. An Echo Nest ID or a Rosetta ID (See Project Rosetta Stone)
                    //    song_type 	no 	yes 	christmas, live, studio 	controls the type of songs returned. Supported song_types are: 'christmas', 'live', 'studio', 'acoustic' and 'electric'. A song type can optionally be followed by ':' and a state, where the state can be one of 'true', 'false' or 'any'. If no state is given, the desired state is defaulted to 'true'.
                    //    max_tempo 	no 	no 	0.0 < tempo < 500.0 (BPM) 	the maximum tempo for the song
                    //    min_tempo 	no 	no 	0.0 < tempo < 500.0 (BPM) 	the minimum tempo for the song
                    //    max_duration 	no 	no 	0.0 < duration < 3600.0 (seconds) 	the maximum duration of any song
                    //    min_duration 	no 	no 	0.0 < duration < 3600.0 (seconds) 	the minimum duration of any song
                    //    max_loudness 	no 	no 	-100.0 < loudness < 100.0 (dB) 	the maximum loudness of any song
                    //    min_loudness 	no 	no 	-100.0 < loudness < 100.0 (dB) 	the minimum loudness of any song
                    //    artist_max_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the maximum familiarity of any song
                    //    artist_min_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the minimum familiarity of any song
                    //    artist_start_year_before 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year before the given value
                    //    artist_start_year_after 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year after the given value
                    //    artist_end_year_before 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year before the given value
                    //    artist_end_year_after 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year after the given value
                    //    song_max_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the maxiumum hotttnesss of any song
                    //    song_min_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the minimum hotttnesss of any song
                    //    artist_max_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the maxiumum hotttnesss of any song's artist
                    //    artist_min_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the minimum hotttnesss of any song's artist
                    //    max_danceability 	no 	no 	0.0 < danceability < 1.0 	the maximum danceability of any song
                    //    min_danceability 	no 	no 	0.0 < danceability < 1.0 	the minimum danceability of any song
                    //    max_energy 	no 	no 	0.0 < energy < 1.0 	the maximum energy of any song
                    //    min_energy 	no 	no 	0.0 < energy < 1.0 	the minimum energy of any song
                    //    max_liveness 	no 	no 	0.0 < liveness < 1.0 	the maximum liveness of any song
                    //    min_liveness 	no 	no 	0.0 < liveness < 1.0 	the minimum liveness of any song
                    //    max_speechiness 	no 	no 	0.0 < speechiness < 1.0 	the maximum speechiness of any song
                    //    min_speechiness 	no 	no 	0.0 < speechiness < 1.0 	the minimum speechiness of any song
                    //    max_acousticness 	no 	no 	0.0 < acousticness < 1.0 	the maximum acousticness of any song. This is a beta feature.
                    //    min_acousticness 	no 	no 	0.0 < acousticness < 1.0 	the minimum acousticness of any song. This is a beta feature.
                    //    key 	no 	no 	(c, c-sharp, d, e-flat, e, f, f-sharp, g, a-flat, a, b-flat, b) 0 - 11 	the key of songs in the playlist
                    //    bucket 	no 	yes 	audio_summary, artist_discovery, artist_discovery_rank, artist_familiarity, artist_familiarity_rank, artist_hotttnesss, artist_hotttnesss_rank, artist_location, song_currency, song_currency_rank, song_hotttnesss, song_hotttnesss_rank, song_type, tracks, id:Rosetta-space 	indicates what data should be returned for each song. If specifying the "tracks" bucket, a bucket with an id space must also be specified. See Project Rosetta Stone for details on retrieving information associated with a particular catalog.
                    //    sort 	no 	no 	tempo-asc, duration-asc, loudness-asc, speechiness-asc, acousticness-asc, liveness-asc, artist_familiarity-asc, artist_hotttnesss-asc, artist_start_year-asc, artist_start_year-desc, artist_end_year-asc, artist_end_year-desc, song_hotttnesss-asc, latitude-asc, longitude-asc, mode-asc, key-asc, tempo-desc, duration-desc, loudness-desc, liveness-desc, speechiness-desc, acousticness-desc, artist_familiarity-desc, artist_hotttnesss-desc, song_hotttnesss-desc, latitude-desc, longitude-desc, mode-desc, key-desc, energy-asc, energy-desc, danceability-asc, danceability-desc 	indicates how the songs results s


                    //mode - (minor, major) 0, 1
                    inline QUrl songSearchUrl(int mode, QString & artist, QString & title, QStringList & tags,
                            QStringList & styles, QStringList & moods) {
                        QUrlQuery query = genDefaultParams();

                        if (!artist.isEmpty()) setParam(query, QStringLiteral("artist"), artist);
                        if (!title.isEmpty()) setParam(query, QStringLiteral("title"), title);

                        if (mode == 0 || mode == 1)
                            setParam(query, QStringLiteral("mode"), QString::number(mode));


                        setParam(query, QStringLiteral("style"), styles);
                        setParam(query, QStringLiteral("description"), tags);
                        setParam(query, QStringLiteral("mood"), moods);

                        return baseUrl(QStringLiteral("song/search"), query);
                    }

                    QJsonArray songSearch(int mode = -1, QString artist = QString(), QString title = QString(), QStringList tags = QStringList(),
                                          QStringList styles = QStringList(), QStringList moods = QStringList(), int count = DEFAULT_LIMIT_AMOUNT) {

                        return lQuery(
                            songSearchUrl(mode, artist, title, tags, styles, moods),
                            QueryRules("songs", requestLimit(), count)
                        );
                    }

                    //{
                    //  "response": {
                    //    "status": {
                    //      "code": 0,
                    //      "message": "Success",
                    //      "version": "4.2"
                    //    },
                    //    "songs": [
                    //      {
                    //        "artist_id": "ARH6W4X1187B99274F",
                    //        "id": "SOCZZBT12A6310F251",
                    //        "artist_name": "Radiohead",
                    //        "title": "Karma Police"
                    //      }
                    //    ]
                    //  }
                    //}
            };
        }
    }
}

#endif // ECHONEST_GENRE_API
