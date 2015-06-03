#ifndef ECHONEST_GENRE_API
#define ECHONEST_GENRE_API

#include "iapi.h"

class EchonestGenreApi : public IApi {
    public:
        inline int artistsByGenreLimit() const { return 100; }
        inline QUrl artistsByGenreUrl(QString & genre, int offset = 0) {
            QUrl url(baseUrl("genre/artists"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, artistsByGenreLimit(), offset);
            setParam(query, "bucket", "hotttnesss");
            setParam(query, "name", genre);
            url.setQuery(query);
            return url;
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

        inline int serachGenreLimit() const { return 100; }
        inline QUrl serachGenresUrl(QString & genre, int offset = 0) {
            QUrl url(baseUrl("genre/search"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, serachGenreLimit(), offset);
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


        inline int similarGenresLimit() const { return 100; }
        inline QUrl similarGenresUrl(QString & genre, int offset = 0) {
            QUrl url(baseUrl("genre/similar"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, serachGenreLimit(), offset);
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
