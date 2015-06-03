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

        QJsonObject artistsByGenre(QString genre) {

        }
};

#endif // ECHONEST_GENRE_API
