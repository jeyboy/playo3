#ifndef IAPI
#define IAPI

//http://musicmachinery.com/music-apis/

#include "modules/core/interfaces/isearchable.h"

//#include "misc/web_utils/json.h"

#define DEFAULT_LIMIT_AMOUNT 99999

namespace Core {
    struct QueryRules {
        QueryRules(QString _field, int _limit_per_request = 5, int _total_limit = DEFAULT_LIMIT_AMOUNT, int _offset = 0)
            : field(_field), count(_total_limit), offset(_offset), limit(_limit_per_request) , fact_count(0) {}

        QString field;
        int count, offset, limit, fact_count;
    };

    class IApi : public ISearchable {
    public:
        static inline int extractCount(QJsonArray & array) { return array.takeAt(0).toObject().value(QStringLiteral("count")).toInt(); }
        inline virtual ~IApi() {}
    protected:
        enum JsonPostProc { none = 0, wrap = 1, extract = 2 };

        virtual bool endReached(QJsonObject & response, int offset) = 0;
        virtual int requestLimit() const = 0;
        inline virtual void iterateOffset(int & offset, QJsonObject & /*response*/, QUrl & /*url*/) { offset += requestLimit(); }

        virtual QString offsetKey() const = 0;
        virtual QString limitKey() const = 0;

        virtual bool extractStatus(QUrl & url, QJsonObject & response, int & code, QString & message) = 0;
        virtual QJsonObject & extractBody(QJsonObject & response) = 0;

        QJsonObject sQuery(QUrl url, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
            QJsonObject res;
            sQuery(url, res, post_proc, errorReceiver);
            return res;
        }

        bool sQuery(QUrl url, QJsonObject & response, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
            response = Web::Manager::prepare() -> getJson(url, post_proc & wrap);

            bool status = extractStatus(url, response, code, message);
            if (!status) {
                emit Logger::obj().write(QStringLiteral("sQuery"), url.toString(), message, true);
                sendError(errorReceiver, message, code);
            } else {
                if (post_proc & extract) extractBody(response);
                emit Logger::obj().write(QStringLiteral("sQuery"), url.toString(), response.keys());
            }
            return status;
        }

        QJsonArray lQuery(QUrl url, QueryRules rules, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
            QJsonArray res;
            return lQuery(url, rules, res, post_proc, errorReceiver);
        }

        QJsonArray & lQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
            QJsonObject response;

            while (sQuery(buildUrl(url, rules.offset, rules.limit), response, post_proc, errorReceiver)) {
                QJsonValue val = rules.field.isEmpty() ? QJsonValue(response) : response.value(rules.field);
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
}

#endif // IAPI
