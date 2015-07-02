#ifndef ECHONEST_GENRE_API
#define ECHONEST_GENRE_API

#include "../iapi.h"

//POSSIBLE BUCKETS

//biographies 	returns up to the 15 most recent biographies found on the web related to the artist
//blogs 	returns up to the 15 most recent blogs found on the web related to the artist
//discovery 	returns the discovery score for the artist. This is a measure of how unexpectedly popular the artist is.
//discovery_rank 	returns the discovery rank for the artist
//doc_counts 	returns document counts for each of the various artist document types
//familiarity 	returns the familiarity for the artist
//familiarity_rank 	returns the familiarity rank for the artist
//genre 	returns all genres for an artist
//hotttnesss 	returns the hotttnesss for the artist
//hotttnesss_rank 	returns the hotttnesss rank for the artist
//images 	returns up to the 15 most recent images found on the web related to the artist
//artist_location 	returns information about the location of origin for the artist
//news 	returns up to the 15 most recent news articles found on the web related to the artist
//reviews 	returns up to the 15 most recent reviews found on the web related to the artist
//songs 	returns up to the 15 hotttest songs for the artist
//urls 	returns links to this artist's pages on various sites
//video 	returns up to the 15 most recent videos found on the web related to the artist
//years_active 	returns years active information for the artist
//id:rosetta-catalog 	returns catalog specific information about the artist for the given catalog. See Project Rosetta Stone for details.
//id:Taste-Profile-ID 	returns personal catalog specific information about the artist for the given catalog. See Project Rosetta Stone for details.


class EchonestGenreApi : public IApi {
    public:
        inline virtual ~EchonestGenreApi() {}

        inline QUrl genreArtistsUrl(QString & genre) {
            QUrlQuery query = genDefaultParams();
            setParam(query, "bucket", "hotttnesss");
            setParam(query, "name", genre);
            return baseUrl("genre/artists", query);
        }

        QJsonArray genreArtists(QString genre, int offset = 0, int count = DEFAULT_LIMIT_AMOUNT) {
            return lQuery(
                genreArtistsUrl(genre),
                QueryRules("artists", requestLimit(), count, offset)
            );
        }

        //{
        //    "response": {
        //        "artists": [
        //            {
        //                "hotttnesss": 0.601159,
        //                "id": "AR9PLH11187FB58A87",
        //                "name": "Thelonious Monk"
        //            },
        //            {
        //                "hotttnesss": 0.598763,
        //                "id": "ARIOZCU1187FB3A3DC",
        //                "name": "John Coltrane"
        //            }
        //        ],
        //        "status": {
        //            "code": 0,
        //            "message": "Success",
        //            "version": "4.2"
        //        }
        //    }
        //}

        inline QUrl genresListUrl() {
            QUrlQuery query = genDefaultParams();
            return baseUrl("genre/list", query);
        }

        QJsonArray genresList() {
            QJsonObject response;

            if (sQuery(genresListUrl(), response, wrap_extract))
                return response.value("genres").toArray();

            return QJsonArray();
        }

        inline QUrl genreInfoUrl(QString & genre) {
            QUrlQuery query = genDefaultParams();
            setParam(query, "bucket", "description");
            setParam(query, "name", genre);
            return baseUrl("genre/profile", query);
        }


        QJsonArray genreInfo(QString genre) {
            QJsonObject response;

            if (sQuery(genreInfoUrl(genre), response, wrap_extract))
                return response.value("genres").toArray();

            return QJsonArray();
        }
        //{
        //    "response": {
        //        "genres": [
        //            {
        //                "description": "Acid jazz, also called club jazz, is a style of jazz that takes cues from a number of genres, including funk,
        //hip-hop, house, and soul.",
        //                "name": "acid jazz"
        //            }
        //        ],
        //        "status": {
        //            "code": 0,
        //            "message": "Success",
        //            "version": "4.2"
        //        }
        //    }
        //}

        inline QUrl genresSerachUrl(QString & genre) {
            QUrlQuery query = genDefaultParams();
            setParam(query, "name", genre);
            return baseUrl("genre/search", query);
        }

        QJsonArray genresSerach(QString genre, int count = DEFAULT_LIMIT_AMOUNT) {
            return lQuery(
                genresSerachUrl(genre),
                QueryRules("genres", requestLimit(), count)
            );
        }

        //{
        //    "response": {
        //        "genres": [
        //            {
        //                "name": "a cappella"
        //            },
        //            {
        //                "name": "abstract hip hop"
        //            }
        //        ],
        //        "start": 0,
        //        "status": {
        //            "code": 0,
        //            "message": "Success",
        //            "version": "4.2"
        //        },
        //        "total": 777
        //    }
        //}


        inline QUrl genreSimilarUrl(QString & genre) {
            QUrlQuery query = genDefaultParams();
            setParam(query, "name", genre);
            return baseUrl("genre/similar", query);
        }

        QJsonArray genreSimilar(QString genre, int count = DEFAULT_LIMIT_AMOUNT) {
            return lQuery(
                genreSimilarUrl(genre),
                QueryRules("genres", requestLimit(), count)
            );
        }

        //{
        //    "response": {
        //        "genres": [
        //            {
        //                "name": "rock",
        //                "similarity": 0.768293
        //            },
        //            {
        //                "name": "hard rock",
        //                "similarity": 0.539634
        //            }
        //        ],
        //        "start": 0,
        //        "status": {
        //            "code": 0,
        //            "message": "Success",
        //            "version": "4.2"
        //        },
        //        "total": 19
        //    }
        //}
};

#endif // ECHONEST_GENRE_API
