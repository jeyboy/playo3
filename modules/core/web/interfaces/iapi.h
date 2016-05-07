#ifndef IAPI
#define IAPI

#include "modules/core/interfaces/isearchable.h"

//#include "misc/web_utils/json.h"

#define DEFAULT_LIMIT_AMOUNT 99999

namespace Core {
//    struct QueryRules {
//        QueryRules(QString _field, int _limit_per_request = 5, int _total_limit = DEFAULT_LIMIT_AMOUNT, int _offset = 0)
//            : field(_field), count(_total_limit), offset(_offset), limit(qMin(_limit_per_request, _total_limit)) , fact_count(0) {}

//        QString field;
//        int count, offset, limit, fact_count;
//    };

    class IApi : public ISearchable {
    public:
        inline virtual ~IApi() {}
    protected:
//        enum JsonPostProc { none = 0, wrap = 1, extract = 2 };

        bool showingCaptcha(const QUrl & pict_url, QString & result);
        bool showingLogin(const QString & title, QString & login, QString & pass, const QString & err = QString());
        bool showingLoginWithCaptcha(const QString & title, const QUrl & pict_url, QString & login, QString & pass, QString & captcha, const QString & err = QString());

//        QJsonObject sJsonQuery(QUrl url, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
//            QJsonObject res;
//            sQuery(url, res, post_proc, errorReceiver);
//            return res;
//        }

//        bool sJsonQuery(QUrl url, QJsonObject & response, JsonPostProc post_proc = none, QObject * errorReceiver = 0);

//        QJsonArray lJsonQuery(QUrl url, QueryRules rules, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
//            QJsonArray res;
//            return lQuery(url, rules, res, post_proc, errorReceiver);
//        }

//        QJsonArray & lJsonQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc = none, QObject * errorReceiver = 0);

//        inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
//            if (errorReceiver)
//                QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
//            else qDebug() << "ERROR: " << message;
//        }

        void setLimit(QUrlQuery & query, int limit = DEFAULT_LIMIT_AMOUNT, QVariant offset = 0) {
            if (offset.type() == QVariant::Int) {
                int of = offset.toInt();
                if (of > 0) setParam(query, offsetKey(), QString::number(of));
            } else if (offset.type() == QVariant::String) {
                QString of = offset.toString();
                if (!of.isEmpty()) setParam(query, offsetKey(), of);
            }
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

        virtual QUrl buildUrl(QUrl tUrl, int offset, int limit, const QJsonObject & /*prev_response*/) {
            QUrl url(tUrl);
            QUrlQuery query = QUrlQuery(url);
            setLimit(query, limit, offset);
            url.setQuery(query);
            return url;
        }
    };
}

#endif // IAPI
