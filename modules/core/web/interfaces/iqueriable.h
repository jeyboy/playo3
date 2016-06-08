#ifndef IQUERIABLE
#define IQUERIABLE

#include <qstring.h>
#include <qurl.h>

#include "modules/core/web/utils/web_manager.h"
#include "modules/core/misc/logger.h"
#include "queriable_arg.h"

namespace Core {
    namespace Web {
        class IQueriable {
            bool sQuery(QueriableArg * arg) {
                Logger::obj().startMark();
                bool status = true;
                int code = -1;
                QString message;
                qDebug() << "REQUEST" << arg -> request_url;

                switch(arg -> call_type) {
                    case call_type_json: {
                        QJsonObject json = Web::Manager::prepare() -> getJson(arg -> request_url, arg -> post_proc & proc_json_wrap);

                        status = extractStatus(arg, json, code, message);
                        if (!status) {
                            Logger::obj().write(QStringLiteral("sQuery"), arg -> request_url, message, true);
                            sendError(arg -> error_receiver, message, code);
                        } else {
                            if (arg -> post_proc & proc_json_extract) {
                                QJsonValue val = json;

                                for(QStringList::Iterator field = arg -> fields.begin(); field != arg -> fields.end(); field++)
                                    val = val.toObject().value(*field);

                                if (val.isArray())
                                    arg -> append(val.toArray());
                                else
                                    arg -> append(val.toObject());
                            }
                            else arg -> append(json);

                            arg -> forse_completing = endReached(json, arg);
                            Logger::obj().write(QStringLiteral("sQuery"), arg -> request_url, json.keys());
                        }
                    break;}

                    case call_type_html: {
                        Response * response = 0;
                        switch(arg -> call_method) {
                            case call_method_post: { response = Manager::prepare() -> followedPost(arg -> request_url); break; }
                            default: response = Manager::prepare() -> followedGet(arg -> request_url);
                        }
                        status = htmlToJson(arg, response, message, true);

                        if (!status) {
                            Logger::obj().write(QStringLiteral("sQuery"), arg -> request_url, message, true);
                            sendError(arg -> error_receiver, message, code);
                        }
                    break;}

                    default: return false;
                }

                Logger::obj().endMark(QStringLiteral("sQuery") % (status ? "true" : "false"), arg -> request_url);
                return status;
            }

            bool lQuery(QueriableArg * arg) {
                bool status = true;

                while ( (status &= sQuery(arg)) ) {
                    if (arg -> isCompleted()) return status;
                    QThread::msleep(REQUEST_DELAY);
                }

                return status;
            }
        protected:
            bool request(QueriableArg * arg) {
                if (arg -> fields.isEmpty())
                    arg -> fields = QStringList() << DEF_JSON_FIELD;

                switch(arg -> call_amount) {
                    case call_solo: return sQuery(arg);
                    case call_poly: return lQuery(arg);
                    default: return false;
                }
            }

            bool sRequest(const QString & url, QJsonObject & json, const ApiCallType & call_type,
                                const AdditionalProc & post_proc = proc_none, const QStringList & fields = QStringList() << DEF_JSON_FIELD, QObject * error_receiver = 0)
            {
                QJsonArray arr;
                QueriableArg arg(&arr, url, call_type, post_proc, fields, error_receiver);

                bool res = request(&arg);
                json = arr.isEmpty() ? QJsonObject() : arr.last().toObject();
                return res;
            }

            QJsonObject sRequest(const QString & url, const ApiCallType & call_type, QJsonArray * arr = 0,
                                const AdditionalProc & post_proc = proc_none, const QStringList & fields = QStringList() << DEF_JSON_FIELD, QObject * error_receiver = 0)
            {
                QJsonArray res = saRequest(url, call_type, arr, post_proc, fields, error_receiver);
                return res.isEmpty() ? QJsonObject() : res.last().toObject();
            }
            QJsonArray saRequest(const QString & url, const ApiCallType & call_type, QJsonArray * arr = 0,
                                const AdditionalProc & post_proc = proc_none, const QStringList & fields = QStringList() << DEF_JSON_FIELD, QObject * error_receiver = 0)
            {
                QJsonArray temp_arr;
                if (!arr)
                    arr = &temp_arr;
                QueriableArg arg(arr, url, call_type, post_proc, fields, error_receiver);

                request(&arg);
                return *arr;
            }


            QJsonArray pRequest(const QString & url, const ApiCallType & call_type, const PolyQueryRules & poly_rules, QJsonArray * arr = 0,
                                const AdditionalProc & post_proc = proc_none, const QStringList & fields = QStringList() << DEF_JSON_FIELD, QObject * error_receiver = 0, bool ignore_arr_content = true)
            {
                QJsonArray temp_arr;
                if (!arr)
                    arr = &temp_arr;

                QueriableArg arg(arr, url, call_type, post_proc, fields, error_receiver);
                arg.ignoreArrContent(ignore_arr_content);
                arg.setPolyLimitations(poly_rules);

                request(&arg);
                return *arr;
            }

            QString baseUrlStr(const QString & predicate, const QUrlQuery & query) {
                QUrl url(baseUrlStr(predicate));
                url.setQuery(query);
                return url.toString();
            }

            virtual QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) = 0;

            enum QueryParamsType {json, html};

            virtual inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = json) { return QUrlQuery(); }

            // for json
            // extract status and update request url if required
            virtual bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return false; }
            virtual bool endReached(QJsonObject & /*response*/, QueriableArg * /*arg*/) { return false; }

            // for html
            // extract content and update request url if required
            virtual bool htmlToJson(QueriableArg * /*arg*/, Response * /*reply*/, QString & /*message*/, bool /*removeReply*/ = false) { return false; }

            inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
                if (errorReceiver)
                    QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
                else qDebug() << "ERROR: " << message;
            }

            inline QString encodeStr(const QString & str) const { return QUrl::toPercentEncoding(str); }
            inline QString decodeStr(const QString & str) const { return QUrl::fromPercentEncoding(str.toLatin1()); }
        };
    }
}

#endif // IQUERIABLE
