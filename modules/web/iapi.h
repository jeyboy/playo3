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

#define DEFAULT_LIMIT_AMOUNT 99999
#define REQUEST_DELAY 100 // ms // 250

class IApi {
public:
    static inline int extractCount(QJsonArray & array) { return array.takeAt(0).toObject().value("count").toInt(); }
protected:
    virtual QString baseUrlStr(QString & predicate) = 0;
    QUrl baseUrl(QString predicate, QUrlQuery & query) {
        QUrl url(baseUrlStr(predicate));
        url.setQuery(query);
        return url;
    }

    virtual QUrlQuery genDefaultParams() = 0;

    virtual bool endReached(QJsonObject & response, int offset) = 0;
    virtual int requestLimit() const = 0;
    inline virtual void iterateOffset(int & offset, QJsonObject & /*response*/, QUrl & /*url*/) { offset += requestLimit(); }

    virtual QString offsetKey() const = 0;
    virtual QString limitKey() const = 0;

    virtual bool extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message) = 0;
    virtual QJsonObject & extractBody(QJsonObject & response) = 0;

    QJsonObject sQuery(QUrl url, bool wrapJson = false, QObject * errorReceiver = 0, CustomNetworkAccessManager * manager = 0, bool skipExtraction = false) {
        QJsonObject res;
        sQuery(url, res, wrapJson, errorReceiver, manager, skipExtraction);
        return res;
    }

    bool sQuery(QUrl url, QJsonObject & response, bool wrapJson = false, QObject * errorReceiver = 0, CustomNetworkAccessManager * manager = 0, bool skipExtraction = false) {
        bool isNew = !manager ? CustomNetworkAccessManager::validManager(manager) : false;
        response = manager -> getToJson(QNetworkRequest(url), wrapJson);
        if (isNew) delete manager;

        bool status = extractStatus(url, response, code, message);
        if (!status) sendError(errorReceiver, message, code);
        else if (!skipExtraction) extractBody(response);
        return status;
    }

    QJsonArray lQuery(QUrl url, int limit, QString key, bool wrapJson = false, int offset = 0, QObject * errorReceiver = 0, CustomNetworkAccessManager * manager = 0) {
        QJsonArray res;
        return lQuery(url, limit, key, res, wrapJson, offset, errorReceiver, manager);
    }

    QJsonArray & lQuery(QUrl url, int limit, QString key, QJsonArray & result, bool wrapJson = false, int offset = 0, QObject * errorReceiver = 0, CustomNetworkAccessManager * manager = 0) {
        bool isNew = !manager ? CustomNetworkAccessManager::validManager(manager) : false;

        int count = 0;
        QJsonObject response, countObj;

        while (sQuery(buildUrl(url, offset, limit), response, wrapJson, errorReceiver, manager)) {
            QJsonValue val = response.value(key);
            bool invalid = val.isArray();

            if (invalid) {
                QJsonArray ar = val.toArray();
                invalid = ar.isEmpty();
                count += ar.size();
            }

            if (!invalid) result.append(val);

            iterateOffset(offset, response, url);
            if (offset >= limit || endReached(response, offset)) break;
            QThread::msleep(REQUEST_DELAY);
        }

        if (isNew) delete manager;
        countObj.insert("count", count);
        result.prepend(countObj);

        return result;
    }

    inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
        if (errorReceiver)
            QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
        else qDebug() << message;
    }

    void setLimit(QUrlQuery & query, int limit = DEFAULT_LIMIT_AMOUNT, int offset = 0) {
        if (offset > 0) setParam(query, offsetKey(), QString::number(offset));
        setParam(query, limitKey(), QString::number(limit));
    }

    inline void setParam(QUrlQuery & query, QString name, int value) { query.addQueryItem(name, QString::number(value)); }
    inline void setParam(QUrlQuery & query, QString name, float value) { query.addQueryItem(name, QString::number(value)); }
    inline void setParam(QUrlQuery & query, QString name, QString value) { query.addQueryItem(name, value); }
    inline void setParam(QUrlQuery & query, QString name, QStringList values) {
        if (values.isEmpty()) return;
        for(QStringList::Iterator val = values.begin(); val != values.end(); val++)
            query.addQueryItem(name, *val);
    }

    virtual QUrl buildUrl(QUrl tUrl, int offset, int limit) {
        QUrl url(tUrl);
        QUrlQuery query = QUrlQuery(url);
        setLimit(query, limit, offset);
        url.setQuery(query);
        return url;
    }
private:
    int code;
    QString message;
};

#endif // IAPI
