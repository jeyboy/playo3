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

//#include "misc/web_utils/json.h"

#define DEFAULT_LIMIT_AMOUNT 999999

class IApi {
protected:
    virtual QString baseUrlStr(QString & predicate) = 0;
    QUrl baseUrl(QString predicate, QUrlQuery & query) {
        QUrl url(baseUrlStr(predicate));
        url.setQuery(query);
        return url;
    }

    virtual QUrlQuery genDefaultParams() = 0;

    virtual int extractAmount(QJsonObject & response) = 0;
    virtual int requestLimit() = 0;

    virtual QString offsetKey() = 0;
    virtual QString limitKey() = 0;

    virtual void extractStatus(QJsonObject & response, int & code, QString & message) = 0;
    virtual QJsonObject & extractBody(QJsonObject & response) = 0;

    bool proceedQuery(QUrl url, QJsonObject & response, QObject * errorReceiver = 0, CustomNetworkAccessManager * manager = 0) {
        bool isNew = !manager ? CustomNetworkAccessManager::validManager(manager) : false;
        response = manager -> getToJson(QNetworkRequest(url));
        if (isNew) delete manager;
        ectractStatus(response, code, message);
        bool status = code == 0;

        if (!status) sendError(errorReceiver, message, code);

        return status;
    }


    QJsonArray proceedQuery(QUrl url, int limit, QString key, int offset = 0, QObject * errorReceiver = 0) {
        QJsonArray res;
        return proceedQuery(url, limit, key, res, offset, errorReceiver);
    }

    QJsonArray & proceedQuery(QUrl url, int limit, QString key, QJsonArray & result, int offset = 0, QObject * errorReceiver = 0) {
        CustomNetworkAccessManager * manager;
        bool isNew = CustomNetworkAccessManager::validManager(manager), status = true;
        QJsonObject response;

        while (status = proceedQuery(buildUrl(url, offset, limit), response, errorReceiver, manager)) {
            result.append(extractBody(response).value(key));

            offset += requestLimit();
            if (offset >= limit || offset >= extractAmount(response)) break;
        }

        if (isNew) delete manager;
        return result;
    }

    inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
        if (errorReceiver)
            QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
        else qDebug() << message;
    }

    void setLimit(QUrlQuery & query, int limit = DEFAULT_LIMIT_AMOUNT, int offset = 0) {
        if (offset > 0) EchonestGenreApi::setParam(query, offsetName(), QString::number(offset));
        EchonestGenreApi::setParam(query, limitName(), QString::number(qMin(limit, requestLimit())));
    }


    inline void setParam(QUrlQuery & query, QString name, int value) { query.addQueryItem(name, QString::number(value)); }
    inline void setParam(QUrlQuery & query, QString name, float value) { query.addQueryItem(name, QString::number(value)); }
    inline void setParam(QUrlQuery & query, QString name, QString value) { query.addQueryItem(name, value); }
    inline void setParam(QUrlQuery & query, QString name, QStringList values) {
        if (values.isEmpty()) return;
        for(QStringList::Iterator val = values.begin(); val != values.end(); val++)
            query.addQueryItem(name, *val);
    }

    QUrl buildUrl(QUrl tUrl, int offset, int limit) {
        QUrl url(tUrl);
        QUrlQuery query = QUrlQuery(url);
        setLimit(query, limit, offset);
        url.setQuery(query);
        return query;
    }
private:
    int code;
    QString message;
};

#endif // IAPI
