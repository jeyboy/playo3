#ifndef IAPI
#define IAPI

#include "misc/call_initiator.h"

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray>

class IApi { // format=json
    public:
        virtual QString baseUrl(QString predicate) = 0;
        virtual QString token() = 0;

        virtual int extractAmount(QJsonObject & response);

        virtual void registerQuery(QUrl url, CallInitiator & initiator) = 0;
        virtual bool proceedQuery(QUrl url, QJsonObject & response) = 0;
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
