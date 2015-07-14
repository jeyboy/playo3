#ifndef IAPI
#define IAPI

//http://musicmachinery.com/music-apis/

#include "misc/logger.h"
#include "misc/web_utils/web_manager.h"

#include <qstringbuilder.h>

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray.h>

//#include "misc/web_utils/json.h"

#define DEFAULT_LIMIT_AMOUNT 99999
#define REQUEST_DELAY 260 // ms // 250

struct QueryRules {
    QueryRules(QString _field, int _limit = 5, int _count = DEFAULT_LIMIT_AMOUNT, int _offset = 0)
        : field(_field), count(_count), offset(_offset), limit(_limit) , fact_count(0) {}

    QString field;
    int count, offset, limit, fact_count;
};

class IApi {
public:
    static inline int extractCount(QJsonArray & array) { return array.takeAt(0).toObject().value(QStringLiteral("count")).toInt(); }
protected:
    enum JsonPostProc { none = 0, wrap = 1, extract = 2, wrap_extract = wrap | extract };

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

    QJsonObject sQuery(QUrl url, JsonPostProc post_proc = none, QObject * errorReceiver = 0, WebManager * manager = 0) {
        QJsonObject res;
        sQuery(url, res, post_proc, errorReceiver, manager);
        return res;
    }

    bool sQuery(QUrl url, QJsonObject & response, JsonPostProc post_proc = none, QObject * errorReceiver = 0, WebManager * manager = 0) {
        bool isNew = !manager ? WebManager::valid(manager) : false;
        response = manager -> getToJson(QNetworkRequest(url), post_proc & wrap);
        if (isNew) delete manager;

        bool status = extractStatus(url, response, code, message);
        if (!status) {
            Logger::instance() -> writeToStream(QStringLiteral("sQuery"), url.toString(), message, true);
            sendError(errorReceiver, message, code);
        } else {
            if (post_proc & extract) extractBody(response);
            Logger::instance() -> writeToStream(QStringLiteral("sQuery"), url.toString(), response.keys());
        }
        return status;
    }

    QJsonArray lQuery(QUrl url, QueryRules rules, JsonPostProc post_proc = none, QObject * errorReceiver = 0, WebManager * manager = 0) {
        QJsonArray res;
        return lQuery(url, rules, res, post_proc, errorReceiver, manager);
    }

    QJsonArray & lQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc = none, QObject * errorReceiver = 0, WebManager * manager = 0) {
        bool isNew = !manager ? WebManager::valid(manager) : false;

        QJsonObject response;

        while (sQuery(buildUrl(url, rules.offset, rules.limit), response, post_proc, errorReceiver, manager)) {
            QJsonValue val = response.value(rules.field);
            bool invalid = val.isArray();

            if (invalid) {
                QJsonArray ar = val.toArray();
                invalid = ar.isEmpty();
                rules.fact_count += ar.size();
            }

            if (!invalid) result.append(val);

            iterateOffset(rules.offset, response, url);
            if (rules.offset >= rules.count || endReached(response, rules.offset)) break;
            QThread::msleep(REQUEST_DELAY);
        }

        if (isNew) delete manager;
        setCount(result, rules.fact_count);

        return result;
    }

    inline void setCount(QJsonArray & ar, int count) {
        QJsonObject countObj;
        countObj.insert("count", count);
        ar.prepend(countObj);
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
