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
        protected:
            QJsonObject sRequest(const QString & url, const ApiCallType & call_type,
                                const AdditionalProc & post_proc = proc_none, const QString & field = QString(), QObject * error_receiver = 0)
            {
                QJsonArray arr = saRequest(url, call_type, post_proc, field, error_receiver);
                return arr.isEmpty() ? QJsonObject() : arr.last().toObject();
            }
            QJsonArray saRequest(const QString & url, const ApiCallType & call_type,
                                const AdditionalProc & post_proc = proc_none, const QString & field = QString(), QObject * error_receiver = 0)
            {
                QJsonArray arr;
                QueriableArg arg(&arr, url, call_type, post_proc, field, error_receiver);

                request(&arg);
                return arr;
            }


            QJsonArray pRequest(const QString & url, const ApiCallType & call_type, const PolyQueryRules & poly_rules,
                                const AdditionalProc & post_proc = proc_none, QJsonArray */*&*/ arr = 0, const QString & field = QString(), QObject * error_receiver = 0)
            {
                QJsonArray temp_arr;
                if (!arr)
                    arr = &temp_arr;

                QueriableArg arg(arr, url, call_type, post_proc, field, error_receiver);

                arg.setPolyLimitations(poly_rules);

                request(&arg);
                return *arr;
            }

            bool request(QueriableArg * arg) {
                switch(arg -> call_amount) {
                    case call_solo: return sQuery(arg);
                    case call_poly: return lQuery(arg);
                    default: return false;
                }
            }

            QUrl baseUrl(const QString & predicate, const QUrlQuery & query) {
                QUrl url(baseUrlStr(predicate));
                url.setQuery(query);
                return url;
            }

            virtual QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) = 0;
            virtual inline QUrlQuery genDefaultParams() { return QUrlQuery(); }

            // override for any type of poly
            virtual QueriableArg * buildUrl(QueriableArg * arg) { return arg; } // by default url builded automatically

            // for json
            // extract status and update request url if required
            virtual bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return false; }
            virtual bool endReached(QJsonObject & /*response*/, int /*offset*/) { return false; }

            // for html
            // extract content and update request url if required
            virtual bool htmlToJson(QueriableArg * /*arg*/, Response * /*reply*/, QString & /*message*/, bool /*removeReply*/ = false) { return false; }

            bool sQuery(QueriableArg * arg) {
                Logger::obj().startMark();
                bool status = true;
                int code = -1;
                QString message;

                switch(arg -> call_type) {
                    case call_type_json: {
                        QJsonObject json = Web::Manager::prepare() -> getJson(arg -> request_url, arg -> post_proc & proc_json_wrap);

                        status = extractStatus(arg, json, code, message);
                        if (!status) {
                            Logger::obj().write(QStringLiteral("sQuery"), arg -> request_url, message, true);
                            sendError(arg -> error_receiver, message, code);
                        } else {
                            arg -> append(
                                (arg -> post_proc & proc_json_extract) ?
                                    json.value(arg -> field).toObject() :
                                    json
                            );
                            arg -> forse_completing = endReached(json, arg -> items_fact_count);
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

                while ( (status &= sQuery(buildUrl(arg))) ) {
                    if (arg -> isCompleted()) return status;
                    QThread::msleep(REQUEST_DELAY);
                }

                return status;
            }


            //// GRAB
//            bool sGrabQuery(QueriableArg * arg) {
//                Logger::obj().startMark();
//                Response * response = 0;

//                switch(arg -> call_method) {
//                    case call_method_post: { response = Manager::prepare() -> followedPost(arg -> url); break; }
//                    default: response = Manager::prepare() -> followedGet(arg -> url);
//                }

//                bool res = htmlToJson(response, arg, true);
//                Logger::obj().endMark(QStringLiteral("Grabber-") % QString::number(items.size()) % (res ? "true" : "false"), url.toString());
//                return res;
//            }

    //        inline QJsonArray lGrabQuery(QString url, toJsonType jtype, int count, int start = 1) {
    //            QJsonArray items;
    //            return lQuery(url, items, jtype, count, start);
    //        }

//            QJsonArray & lGrabQuery(QString url, QJsonArray & result, toJsonType jtype, int count, int start = 1, int total = MAX_TOTAL) {
//                int page_limit = count + (start - 1);
//                while (sQuery(QUrl(url.arg(QString::number(start))), result, jtype)) {
//                    qDebug() << "LLL" << start << page_limit << result.size() << total;
//                    if (result.isEmpty() || start++ >= page_limit || result.size() >= total) break;
//                    QThread::msleep(REQUEST_DELAY);
//                }

//                return result;
//            }

//            //// JSON
//            QJsonObject sJsonQuery(QUrl url, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
//                QJsonObject res;
//                sQuery(url, res, post_proc, errorReceiver);
//                return res;
//            }

//            bool sJsonQuery(QUrl url, QJsonObject & response, JsonPostProc post_proc = none, QObject * errorReceiver = 0);
//            bool IApi::sQuery(QUrl url, QJsonObject & response, JsonPostProc post_proc, QObject * errorReceiver) {
//                int code;
//                QString message;
//                Logger::obj().write(QStringLiteral("sQuery"), url.toString(), QStringLiteral("sending"));
//                response = Web::Manager::prepare() -> getJson(url, post_proc & wrap);

//                bool status = extractStatus(url, response, code, message);
//                if (!status) {
//                    Logger::obj().write(QStringLiteral("sQuery"), url.toString(), message, true);
//                    sendError(errorReceiver, message, code);
//                } else {
//                    if (post_proc & extract) extractBody(response);
//                    Logger::obj().write(QStringLiteral("sQuery"), url.toString(), response.keys());
//                }
//                return status;
//            }

//            QJsonArray lJsonQuery(QUrl url, QueryRules rules, JsonPostProc post_proc = none, QObject * errorReceiver = 0) {
//                QJsonArray res;
//                return lQuery(url, rules, res, post_proc, errorReceiver);
//            }

//            QJsonArray & lJsonQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc = none, QObject * errorReceiver = 0);

//            QJsonArray & IApi::lQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc, QObject * errorReceiver) {
//                QJsonObject response;

//                while (sQuery(buildUrl(url, rules.offset, rules.limit, response), response, post_proc, errorReceiver)) {
//                    QJsonValue val = rules.field.isEmpty() ? QJsonValue(response) : response.value(rules.field);
//                    bool invalid = val.isArray();

//                    if (invalid) {
//                        QJsonArray ar = val.toArray();
//                        invalid = ar.isEmpty();
//                        rules.fact_count += ar.size();
//                    }

//                    if (!invalid)
//                        concatJsonArrays(result, val.toArray());

//                    iterateOffset(rules.offset, response, url);
//                    if (rules.offset >= rules.count || endReached(response, rules.offset)) break;
//                    else QThread::msleep(REQUEST_DELAY);
//                }

//                return result;
//            }
//            ///










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
