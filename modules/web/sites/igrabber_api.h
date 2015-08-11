#ifndef IGRABBER_API
#define IGRABBER_API

#include "misc/logger.h"

#include "misc/web_utils/web_manager.h"
#include "misc/web_utils/html_parser.h"
#include "media/genres/web/target_genres.h"
#include "media/duration.h"
#include "grabber_keys.h"

#include <qdebug.h>

#include <qurl.h>
#include <qurlquery.h>
#include <qstringlist.h>

#include <qjsonobject.h>
#include <qjsonarray.h>

#define DEFAULT_PREDICATE_NAME QString()
#define GRAB_DELAY 200 // ms
#define MAX_PAGE 999
#define STYLES_MAX_PAGE 50

class IGrabberApi {
public:
    enum ByTypeArg {
        sets,
        charts,
        soundtracks,
        by_genres,
        by_years,
        other,
        hits,
        fresh
    };

    virtual QJsonArray search(QString & /*predicate*/, QString & /*genre*/, int /*genre_id*/, bool /*popular*/, bool /*by_artist*/, int /*count*/) { return QJsonArray(); }

    virtual TargetGenres genresList() const { return genres; }

    virtual QJsonArray byGenre(QString /*genre*/, int /*genre_id*/) { return QJsonArray(); }

    virtual QJsonArray byChar(QChar /*target_char*/) { return QJsonArray(); }

    virtual QJsonArray byType(ByTypeArg /*target_type*/) { return QJsonArray(); }

    virtual QJsonArray popular() { return QJsonArray(); }

    QString refresh(QString refresh_page) {
        if (refresh_page.isEmpty()) return QString();

        WebManager * manager = 0;
        bool isNew = WebManager::valid(manager);
        QNetworkReply * response = manager -> getSync(QUrl(refresh_page));
        QString res = refresh_postprocess(response);
        delete response;
        if (isNew) delete manager;
        return res;
    }

//    virtual QJsonArray related(QUrl /*target_page*/) { return QJsonArray(); }
protected:
    TargetGenres genres;

    virtual QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) = 0;
    QUrl baseUrl(QString predicate, QUrlQuery & query) {
        QUrl url(baseUrlStr(predicate));
        url.setQuery(query);
        return url;
    }

    virtual QUrlQuery genDefaultParams() { return QUrlQuery(); }

//    virtual bool endReached(QJsonObject & response, int offset) = 0;
////    virtual int requestLimit() const = 0;
////    inline virtual void iterateOffset(int & offset, QJsonObject & /*response*/, QUrl & /*url*/) { offset += requestLimit(); }

//    virtual QString offsetKey() const = 0;
//    virtual QString limitKey() const = 0;

    virtual QString refresh_postprocess(QNetworkReply * /*response*/) { return QString(); }

    virtual bool toJson(QNetworkReply * reply, QJsonArray & json, bool removeReply = false) = 0;

    void sQuery(QUrl url, QJsonArray & res, WebManager * manager = 0) {
        bool isNew = !manager ? WebManager::valid(manager) : false;
        QNetworkReply * response = manager -> getSync(url);

        Logger::instance() -> startMark();
        toJson(response, res);
        Logger::instance() -> endMark(QStringLiteral("Grabber"), url.toString());
        delete response;
        if (isNew) delete manager;
    }

//    QJsonArray lQuery(QUrl url, QueryRules rules, JsonPostProc post_proc = none, QObject * errorReceiver = 0, Web * manager = 0) {
//        QJsonArray res;
//        return lQuery(url, rules, res, post_proc, errorReceiver, manager);
//    }

//    QJsonArray & lQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc = none, QObject * errorReceiver = 0, Web * manager = 0) {
//        bool isNew = !manager ? Web::validManager(manager) : false;

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
