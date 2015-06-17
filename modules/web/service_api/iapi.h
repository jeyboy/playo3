#ifndef IAPI
#define IAPI

//http://musicmachinery.com/music-apis/

#include "misc/call_initiator.h"

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray.h>

class IApi { // format=json
    protected:
        virtual QString baseUrl(QString predicate) = 0;
        virtual QString token() const = 0;

        virtual int extractAmount(QJsonObject & response) = 0;
        virtual int requestLimit() = 0;

        virtual bool proceedQuery(QUrl url, QJsonObject & response) = 0;

        virtual void setLimit(QUrlQuery & query, int limit, int offset = 0) = 0;
        inline void setToken(QUrlQuery & query) { setParam(query, "api_key", token()); }
        inline void setParam(QUrlQuery & query, QString name, int value) { query.addQueryItem(name, QString::number(value)); }
        inline void setParam(QUrlQuery & query, QString name, float value) { query.addQueryItem(name, QString::number(value)); }
        inline void setParam(QUrlQuery & query, QString name, QString value) { query.addQueryItem(name, value); }
        inline void setParam(QUrlQuery & query, QString name, QStringList values) {
            if (values.isEmpty()) return;
            for(QStringList::Iterator val = values.begin(); val != values.end(); val++)
                query.addQueryItem(name, *val);
        }
        inline QUrlQuery buildDefaultParams() {
            QUrlQuery query;
            setToken(query);
            return query;
        }
};

#endif // IAPI
