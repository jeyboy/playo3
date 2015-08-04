#ifndef FOURSHARED_REQUEST_API
#define FOURSHARED_REQUEST_API

#include "../../iapi.h"
#include "fourshared_api_keys.h"

#define FOURSHARED_OFFSET_LIMIT 1000

namespace Fourshared {
    class RequestApi : public IApi {
        enum CategoryTypes {
            music = 1, video = 2, photo = 3, archive = 4,
            book = 5,  program = 6, web = 7, mobile = 8,
            android = 10
        };

        inline void setCategory(QUrlQuery & query, CategoryTypes cType) { setParam(query, "category", (int)cType); }
        inline void setSearchPredicate(QUrlQuery & query, QString & predicate) { setParam(query, "query", predicate); }
    public:
        QUrl audioSearchUrl(QString & predicate, CategoryTypes cType = music) {
            QUrlQuery query = genDefaultParams();
            setCategory(query, cType);
            setSearchPredicate(query, predicate);

            return baseUrl(QStringLiteral("files"), query);
        }
        QJsonArray audioSearch(QString & predicate, int count = 5) {
            QJsonArray res = lQuery(
                audioSearchUrl(predicate),
                QueryRules(QStringLiteral("files"), requestLimit(), qMin(count, FOURSHARED_OFFSET_LIMIT)),
                wrap
            );

            return prepareAudios(res);
        }
    private:
        QJsonArray prepareAudios(QJsonArray & items) {
            QJsonArray ar;

            for(QJsonArray::Iterator item = items.begin(); item != items.end(); item++) {
                QJsonObject obj;

//                obj

                ar << obj;
            }

            return ar;
        }
    };       
}

#endif // FOURSHARED_REQUEST_API
