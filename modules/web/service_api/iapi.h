#ifndef IAPI
#define IAPI

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

class IApi { // format=json
    public:
        virtual QString baseUrl(QString predicate) = 0;
        virtual QString token() = 0;
    protected:
        inline void setToken(QUrlQuery & query) { setParam(query, "api_key", token()); }
        inline void setLimit(QUrlQuery & query, int limit, int offset = 0) {
            if (offset > 0)
                setParam(query, "start", QString::number(offset));
            setParam(query, "results", QString::number(limit));
        }
        inline void setParam(QUrlQuery & query, QString name, QString value) { query.addQueryItem(name, value); }
        inline QUrlQuery buildDefaultParams() {
            QUrlQuery query;
            setToken(query);
            return query;
        }
};

#endif // IAPI
