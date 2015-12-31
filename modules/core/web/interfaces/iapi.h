#ifndef IAPI
#define IAPI

//http://musicmachinery.com/music-apis/

#include "modules/core/interfaces/isearchable.h"

//#include "misc/web_utils/json.h"

#define DEFAULT_LIMIT_AMOUNT 99999

namespace Core {
    struct QueryRules {
        QueryRules(QString _field, int _limit_per_request = 5, int _total_limit = DEFAULT_LIMIT_AMOUNT, int _offset = 0)
            : field(_field), count(_total_limit), offset(_offset), limit(qMin(_limit_per_request, _total_limit)) , fact_count(0) {}

        QString field;
        int count, offset, limit, fact_count;
    };

    class IApi : public ISearchable {
    public:
        inline virtual ~IApi() {}
    protected:
        enum JsonPostProc { none = 0, wrap = 1, extract = 2 };

        bool showingCaptcha(const QUrl & pict_url, QString & result);
        bool showingLogin(const QString & title, QString & login, QString & pass, const QString & err = QString());
        bool showingLoginWithCaptcha(const QString & title, const QUrl & pict_url, QString & login, QString & pass, QString & captcha, const QString & err = QString());

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

        bool sQuery(QUrl url, QJsonObject & response, JsonPostProc post_proc = none, QObject * errorReceiver = 0);

        QJsonArray lQuery(QUrl url, QueryRules rules, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
            QJsonArray res;
            return lQuery(url, rules, res, post_proc, errorReceiver);
        }

        QJsonArray & lQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc = none, QObject * errorReceiver = 0);

        inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
            if (errorReceiver)
                QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
            else qDebug() << "ERROR: " << message;
        }

        void setLimit(QUrlQuery & query, int limit = DEFAULT_LIMIT_AMOUNT, int offset = 0) {
            if (offset > 0) setParam(query, offsetKey(), QString::number(offset));
            setParam(query, limitKey(), QString::number(limit));
        }

        inline void setParam(QUrlQuery & query, const QString & name, int value) { query.addQueryItem(name, QString::number(value)); }
        inline void setParam(QUrlQuery & query, const QString & name, float value) { query.addQueryItem(name, QString::number(value)); }
        inline void setParam(QUrlQuery & query, const QString & name, const QString & value) { query.addQueryItem(name, value); }
        inline void setParam(QUrlQuery & query, const QString & name, const QStringList & values) {
            if (values.isEmpty()) return;
            for(QStringList::ConstIterator val = values.constBegin(); val != values.constEnd(); val++)
                query.addQueryItem(name, *val);
        }

        virtual QUrl buildUrl(QUrl tUrl, int offset, int limit) {
            QUrl url(tUrl);
            QUrlQuery query = QUrlQuery(url);
            setLimit(query, limit, offset);
            url.setQuery(query);
            return url;
        }
    };
}

#endif // IAPI
