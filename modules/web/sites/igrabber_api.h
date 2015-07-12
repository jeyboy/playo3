#ifndef IGRABBER_API
#define IGRABBER_API

////http://musicmachinery.com/music-apis/

#include "misc/logger.h"

#include "misc/web_utils/custom_network_access_manager.h"
#include "misc/web_utils/html_parser.h"
#include "media/genres/web/target_genres.h"

#include <qdebug.h>

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray.h>


//#define DEFAULT_LIMIT_AMOUNT 99999
//#define REQUEST_DELAY 260 // ms // 250

//struct QueryRules {
//    QueryRules(QString _field, int _limit = 5, int _count = DEFAULT_LIMIT_AMOUNT, int _offset = 0)
//        : field(_field), offset(_offset), limit(_limit), count(_count), fact_count(0) {}

//    QString field;
//    int count, offset, limit, fact_count;
//};

#define AUDIO_KEY_NAME QLatin1String("audio")
#define DEFAULT_PREDICATE_NAME QLatin1String("")
#define GRAB_DELAY 200 // ms

class IGrabberApi {
protected:
    virtual QString baseUrlStr(QString & predicate = DEFAULT_PREDICATE_NAME) = 0;
    QUrl baseUrl(QString predicate, QUrlQuery & query) {
        QUrl url(baseUrlStr(predicate));
        url.setQuery(query);
        return url;
    }

    virtual QUrlQuery genDefaultParams() { return QUrlQuery(); }

    virtual bool endReached(QJsonObject & response, int offset) = 0;
//    virtual int requestLimit() const = 0;
//    inline virtual void iterateOffset(int & offset, QJsonObject & /*response*/, QUrl & /*url*/) { offset += requestLimit(); }

    virtual QString offsetKey() const = 0;
    virtual QString limitKey() const = 0;

    virtual TargetGenres genresList() { return TargetGenres(); }

    virtual QJsonObject byGenre(QString /*genre*/, int /*genre_code*/ = 0) { return QJsonObject(); }

    virtual QJsonObject byChar(QChar /*target_char*/) { return QJsonObject(); }

    virtual QJsonObject byType(QString /*target_type*/) { return QJsonObject(); }

    virtual QJsonObject search(QString & /*predicate*/) { return QJsonObject(); }

    virtual QJsonObject popular() { return QJsonObject(); }

    void toJson(QNetworkReply * reply, QJsonObject & json) = 0;

    QJsonObject sQuery(QUrl url, CustomNetworkAccessManager * manager = 0) {
        QJsonObject res;
        bool isNew = !manager ? CustomNetworkAccessManager::validManager(manager) : false;
        QNetworkReply * response = manager -> getSync(QNetworkRequest(url));

        Logger::instance() -> startMark();
        toJson(response, res);
        Logger::instance() -> endMark("Grabber", url.toString());
        delete response;
        if (isNew) delete manager;
        return res;
    }

//    QJsonArray lQuery(QUrl url, QueryRules rules, JsonPostProc post_proc = none, QObject * errorReceiver = 0, CustomNetworkAccessManager * manager = 0) {
//        QJsonArray res;
//        return lQuery(url, rules, res, post_proc, errorReceiver, manager);
//    }

//    QJsonArray & lQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc = none, QObject * errorReceiver = 0, CustomNetworkAccessManager * manager = 0) {
//        bool isNew = !manager ? CustomNetworkAccessManager::validManager(manager) : false;

//        QJsonObject response;

//        while (sQuery(buildUrl(url, rules.offset, rules.limit), response, post_proc, errorReceiver, manager)) {
//            QJsonValue val = response.value(rules.field);
//            bool invalid = val.isArray();

//            if (invalid) {
//                QJsonArray ar = val.toArray();
//                invalid = ar.isEmpty();
//                rules.fact_count += ar.size();
//            }

//            if (!invalid) result.append(val);

//            iterateOffset(rules.offset, response, url);
//            if (rules.offset >= rules.count || endReached(response, rules.offset)) break;
//            QThread::msleep(REQUEST_DELAY);
//        }

//        if (isNew) delete manager;
//        setCount(result, rules.fact_count);

//        return result;
//    }

//    inline void setCount(QJsonArray & ar, int count) {
//        QJsonObject countObj;
//        countObj.insert("count", count);
//        ar.prepend(countObj);
//    }

//    inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
//        if (errorReceiver)
//            QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
//        else qDebug() << message;
//    }

//    void setLimit(QUrlQuery & query, int limit = DEFAULT_LIMIT_AMOUNT, int offset = 0) {
//        if (offset > 0) setParam(query, offsetKey(), QString::number(offset));
//        setParam(query, limitKey(), QString::number(limit));
//    }

//    inline void setParam(QUrlQuery & query, QString name, int value) { query.addQueryItem(name, QString::number(value)); }
//    inline void setParam(QUrlQuery & query, QString name, float value) { query.addQueryItem(name, QString::number(value)); }
//    inline void setParam(QUrlQuery & query, QString name, QString value) { query.addQueryItem(name, value); }
//    inline void setParam(QUrlQuery & query, QString name, QStringList values) {
//        if (values.isEmpty()) return;
//        for(QStringList::Iterator val = values.begin(); val != values.end(); val++)
//            query.addQueryItem(name, *val);
//    }

//    virtual QUrl buildUrl(QUrl tUrl, int offset, int limit) {
//        QUrl url(tUrl);
//        QUrlQuery query = QUrlQuery(url);
//        setLimit(query, limit, offset);
//        url.setQuery(query);
//        return url;
//    }
};

#endif // IGRABBER_API
