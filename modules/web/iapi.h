#ifndef IAPI
#define IAPI

//http://musicmachinery.com/music-apis/

#include "misc/call_initiator.h"
#include "misc/web_utils/custom_network_access_manager.h"

#include <qdebug.h>

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray.h>

class IApi { // format=json
    protected:
        virtual QString baseUrl(QString predicate) = 0;
        virtual void appendParams(QUrlQuery & query) = 0;

        virtual int extractAmount(QJsonObject & response) = 0;
        virtual int requestLimit() = 0;

        virtual bool proceedQuery(QUrl url, QJsonObject & response, QObject * errorReceiver = 0) = 0;

        virtual void setLimit(QUrlQuery & query, int limit = 99999, int offset = 0) = 0;

        inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
            if (errorReceiver)
                QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
            else qDebug() << message;
        }

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
            appendParams(query);
            return query;
        }
};

#endif // IAPI
