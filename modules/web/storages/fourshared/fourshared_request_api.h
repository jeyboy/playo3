#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "../../iapi.h"

namespace Fourshared {
    class RequestApi : public IApi {
        enum CategoryTypes {
            music = 1, video = 2, photo = 3, archive = 4,
            book = 5,  program = 6, web = 7, mobile = 8,
            android = 10
        };

        inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, "category", (int)cType); }
        inline void setSearchPredicate(QUrlQuery & query, CategoryTypes cType = music) { setParam(query, "category", (int)cType); }
    public:
        QUrl audioSearchUrl(QString & predicate, CategoryTypes cType = music) {
            QUrlQuery query = genDefaultParams();
            setCategory(query);
            setSearchPredicate(query, predicate);

            return baseUrl(QStringLiteral("tracks"), query);
        }
        QJsonArray audioSearch(QString & predicate, int count = 5) {
            return lQuery(
                audioSearchUrl(predicate, genre, popular),
                QueryRules(QStringLiteral("response"), requestLimit(), qMin(count, SOUNDCLOUD_OFFSET_LIMIT)),
                wrap
            );
        }
    };
}

#endif // FOURSHARED_REQUEST_API
