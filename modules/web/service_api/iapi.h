#ifndef IAPI
#define IAPI

#include "misc/call_initiator.h"

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray>

class IApi { // format=json
    protected:
        virtual QString baseUrl(QString predicate) = 0;
        virtual QString token() const = 0;

        virtual int extractAmount(QJsonObject & response);
        virtual int requestLimit() const;

//        virtual void registerQuery(QUrl url, CallInitiator & initiator) = 0;
        virtual bool proceedQuery(QUrl url, QJsonObject & response) = 0;

        virtual void setLimit(QUrlQuery & query, int limit, int offset = 0);
        inline void setToken(QUrlQuery & query) { setParam(query, "api_key", token()); }
        inline void setParam(QUrlQuery & query, QString name, QString value) { query.addQueryItem(name, value); }
        inline QUrlQuery buildDefaultParams() {
            QUrlQuery query;
            setToken(query);
            return query;
        }
};

#endif // IAPI
