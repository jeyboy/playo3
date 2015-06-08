#ifndef ECHONEST_GENRE_API
#define ECHONEST_GENRE_API

#include "iapi.h"

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
        inline QUrl artistsByGenreUrl(QString & genre, int offset = 0) {
            QUrl url(baseUrl("genre/artists"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, requestLimit(), offset);
            setParam(query, "bucket", "hotttnesss");
            setParam(query, "name", genre);
            url.setQuery(query);
            return url;
        }

        QJsonArray artistsByGenre(QString genre) {
            QJsonObject response;
            QJsonArray artists;
            int offset = 0;

            while (proceedQuery(artistsByGenreUrl(genre, offset), response)) {
                artists.append(response.value("response").toObject().value("artists"));

                offset += artistsByGenreLimit();

                if (offset >= extractAmount(response))
                    break;
            }

            return artists;
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

        inline QUrl genresUrl() {
            QUrl url(baseUrl("genre/list"));
            QUrlQuery query = buildDefaultParams();
            url.setQuery(query);
            return url;
        }

        QJsonArray genres() {
            QJsonObject response;

            if (proceedQuery(genresUrl(), response)) {
                return response.value("response").toObject().value("genres").toArray();
            } else return QJsonArray();
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

        inline QUrl genreInfoUrl(QString & genre) {
            QUrl url(baseUrl("genre/profile"));
            QUrlQuery query = buildDefaultParams();
            setParam(query, "bucket", "description");
            setParam(query, "name", genre);
            url.setQuery(query);
            return url;
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

        inline QUrl serachGenresUrl(QString & genre, int offset = 0) {
            QUrl url(baseUrl("genre/search"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, requestLimit(), offset);
            setParam(query, "name", genre);
            url.setQuery(query);
            return url;
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


        inline QUrl similarGenresUrl(QString & genre, int offset = 0) {
            QUrl url(baseUrl("genre/similar"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, requestLimit(), offset);
            setParam(query, "name", genre);
            url.setQuery(query);
            return url;
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