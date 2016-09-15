#ifndef IQUERIABLE
#define IQUERIABLE

#include <qstring.h>
#include <qurl.h>

#include "modules/core/defines.h"
#include "modules/core/web/utils/web_manager.h"
#include "modules/core/misc/logger.h"
#include "queriable_arg.h"

#include "modules/core/web/grabber_keys.h"

#define IQUERY_DEF_FIELDS QStringList() << DEF_JSON_FIELD
#define QUERY_RETRY_AMOUNT 3

namespace Core {
    namespace Web {
        class IQueriable {
            bool sQuery(QueriableArg * arg) {
                Logger::obj().startMark();
                bool status = true;
                int code = -1;

                switch(arg -> call_type) {
                    case call_type_json: {

                        QJsonObject json;
                        Manager * manager = Manager::prepare();
                        manager -> setExtractParamsToPayload(arg -> extract_params_to_payload);
                        int retries = 0;

                        while(++retries <= QUERY_RETRY_AMOUNT) {
                            switch(arg -> call_method) {
                                case call_method_post: { json = manager -> jsonPost(arg -> request_url, arg -> headers, arg -> post_proc & proc_json_wrap); break; }
                                default: json = manager -> jsonGet(arg -> request_url, arg -> headers, arg -> post_proc & proc_json_wrap);
                            }

                            if (
                                manager -> statusCode() < 300 &&
                                (status = extractStatus(arg, json, code, arg -> error)) // check on retry requiring
                            ) break;
                            else {
                                // did not retry requests if auth missed
                                if (manager -> statusCode() == 401) {
                                    status = false;
                                    arg -> error = "Authorization missed";
                                    break;
                                }

                                qDebug() << "RETRY" << retries;
                            }
                        }

                        if (!status) {
                            QString err = arg -> error;
                            if( err.isEmpty()) err = QStringLiteral("Some shit happened");

                            Logger::obj().write(QStringLiteral("sQuery"), arg -> request_url, err, Logger::log_error);
                            sendError(arg -> error_receiver, err, code);
                            arg -> append(QJsonObject {{tkn_error, err}}, false);
                        } else {
                            QJsonValue val = arg -> fieldsAffection(json);

                            if (arg -> post_proc & proc_json_proc)
                                val = procJson(val, arg -> post_proc);

                            if (val.isArray())
                                arg -> append(val.toArray());
                            else
                                arg -> append(val.toObject());

                            arg -> forse_completing = endReached(json, arg);
                            Logger::obj().write(QStringLiteral("sQuery"), arg -> request_url, json.keys());
                        }
                    break;}

                    case call_type_html: { // TODO: html part required on additional check of completed state (set arg -> forse_completing), because at this time we have one additional request for each request series (lQuery)
                        Response * response = 0;
                        switch(arg -> call_method) {
                            case call_method_post: { response = Manager::prepare() -> postFollowed(arg -> request_url, arg -> headers); break; }
                            default: response = Manager::prepare() -> getFollowed(arg -> request_url, arg -> headers);
                        }
                        status = htmlToJson(arg, response, arg -> error, true);

                        if (!status) {
                            QString err = arg -> error;
                            if( err.isEmpty()) err = QStringLiteral("Some shit happened");

                            Logger::obj().write(QStringLiteral("sQuery"), arg -> request_url, err, Logger::log_error);
                            arg -> append(QJsonObject {{tkn_error, err}}, false);
                            sendError(arg -> error_receiver, err, code);
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
                    if (arg -> isCompleted()) break;
                    QThread::msleep(REQUEST_DELAY);
                }

                return status;
            }
        protected:
            enum QuerySourceType {               
//                qst_oauth_request, qst_oauth_token, qst_oauth_confirm
                qst_api, qst_site,
                qst_site_audio, qst_site_group, qst_site_user, qst_site_video, // od
                qst_api_base, qst_api_search, qst_site_base, qst_site_search, // fourshared
                qst_api_alt1, qst_api_alt2, qst_site_alt1, qst_site_alt2
            };

            bool request(QueriableArg * arg) {
                if (arg -> fields.isEmpty())
                    arg -> fields = QStringList() << DEF_JSON_FIELD;

                switch(arg -> call_amount) {
                    case call_solo: return sQuery(arg);
                    case call_poly: return lQuery(arg);
                    default: return false;
                }
            }

            bool sRequest(
                const QString & url, QJsonObject & json, const ApiCallType & call_type, const AdditionalProc & post_proc = proc_none,
                const QStringList & fields = IQUERY_DEF_FIELDS, const ApiCallMethod & call_method = call_method_get,
                const Headers & headers = Headers(), QObject * error_receiver = 0, bool extract_params_to_payload = true)
            {
                QJsonArray arr;
                QueriableArg arg(&arr, url, call_type, post_proc, fields, error_receiver, extract_params_to_payload);
                arg.changeCallMethod(call_method);
                arg.setRequestHeaders(headers);

                bool res = request(&arg);
                json = arr.isEmpty() ? QJsonObject() : arr.last().toObject();
                return res;
            }

            QJsonObject sRequest(
                const QString & url, const ApiCallType & call_type, QJsonArray * arr = 0, const AdditionalProc & post_proc = proc_none,
                const QStringList & fields = IQUERY_DEF_FIELDS, const ApiCallMethod & call_method = call_method_get,
                const Headers & headers = Headers(), QObject * error_receiver = 0, bool extract_params_to_payload = true)
            {
                QJsonArray res = saRequest(url, call_type, arr, post_proc, fields, call_method, headers, error_receiver, extract_params_to_payload);
                return res.isEmpty() ? QJsonObject() : res.last().toObject();
            }
            QJsonArray saRequest(
                const QString & url, const ApiCallType & call_type, QJsonArray * arr = 0, const AdditionalProc & post_proc = proc_none,
                const QStringList & fields = IQUERY_DEF_FIELDS, const ApiCallMethod & call_method = call_method_get,
                const Headers & headers = Headers(), QObject * error_receiver = 0, bool extract_params_to_payload = true)
            {
                QJsonArray temp_arr;
                if (!arr)
                    arr = &temp_arr;
                QueriableArg arg(arr, url, call_type, post_proc, fields, error_receiver, extract_params_to_payload);
                arg.changeCallMethod(call_method);
                arg.setRequestHeaders(headers);

                request(&arg);
                return *arr;
            }


            QJsonArray paRequest(
                const QString & url, const ApiCallType & call_type, const PolyQueryRules & poly_rules, QJsonArray * arr = 0,
                const AdditionalProc & post_proc = proc_none, const QStringList & fields = IQUERY_DEF_FIELDS,
                const ApiCallMethod & call_method = call_method_get, const Headers & headers = Headers(),
                QObject * error_receiver = 0, bool ignore_arr_content = true, bool extract_params_to_payload = true)
            {
                QJsonArray temp_arr;
                if (!arr)
                    arr = &temp_arr;

                QueriableArg arg(arr, url, call_type, post_proc, fields, error_receiver, extract_params_to_payload);
                arg.ignoreArrContent(ignore_arr_content);
                arg.changeCallMethod(call_method);
                arg.setRequestHeaders(headers);
                arg.setPolyLimitations(poly_rules);

                request(&arg);
                return *arr;
            }

            QueriableResponse pRequest(
                const QString & url, const ApiCallType & call_type, const PolyQueryRules & poly_rules, QJsonArray * arr = 0,
                const AdditionalProc & post_proc = proc_none, const QStringList & fields = IQUERY_DEF_FIELDS,
                const ApiCallMethod & call_method = call_method_get, const Headers & headers = Headers(),
                QObject * error_receiver = 0, bool ignore_arr_content = true, bool extract_params_to_payload = true)
            {
                QJsonArray temp_arr;
                if (!arr)
                    arr = &temp_arr;

                QueriableArg arg(arr, url, call_type, post_proc, fields, error_receiver, extract_params_to_payload);
                arg.ignoreArrContent(ignore_arr_content);
                arg.changeCallMethod(call_method);
                arg.setRequestHeaders(headers);
                arg.setPolyLimitations(poly_rules);

                request(&arg);

                bool is_iter_token = poly_rules.call_item_method == call_iter_method_token;

                return QueriableResponse(
                    *arr, is_iter_token ? arg.offset_token : QString::number(arg.start_offset),
                    arg.items_total_limit,
                    arg.requests_limit,
                    arr -> isEmpty() || (is_iter_token && arg.offset_token.isEmpty()) ||
                        (arg.requests_fact_count < arg.requests_limit &&
                         arg.items_fact_count < arg.items_total_limit)
                );
            }

            // for json
            // extract status and update request url if required
            virtual bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return true; }
            virtual bool endReached(QJsonObject & /*response*/, QueriableArg * /*arg*/) { return false; }

            // for html
            // extract content and update request url if required
            virtual bool htmlToJson(QueriableArg * /*arg*/, Response * /*reply*/, QString & /*message*/, bool /*removeReply*/ = false) { return false; }

            virtual QJsonValue procJson(const QJsonValue & json, const AdditionalProc & /*proc*/) { return json; }

            inline void sendError(QObject * errorReceiver, QString & message, int code = -1) {
                if (errorReceiver)
                    QMetaObject::invokeMethod(errorReceiver, "errorReceived", Q_ARG(int, code), Q_ARG(QString, message));
                else qDebug() << "ERROR: " << message;
            }

            virtual QString baseUrlStr(const QuerySourceType & stype, const QString & predicate = DEFAULT_PREDICATE_NAME) = 0;
            virtual QString baseUrlStr(const QuerySourceType & stype, const QString & predicate, const QUrlQuery & query) {
//                QUrl url(baseUrlStr(stype, predicate));
//                url.setQuery(query);
//                return url.toString();

                return baseUrlStr(stype, predicate) % (query.isEmpty() ? QString() : '?' % query.toString());
            }

            QString baseUrlStr(const QuerySourceType & stype, const QString & predicate, const std::initializer_list<std::pair<QString, QVariant> > & params) {
                QUrlQuery query = genDefaultParams(stype);

                if (params.size() > 0)
                    for (typename std::initializer_list<std::pair<QString, QVariant> >::const_iterator it = params.begin(); it != params.end(); ++it)
                        setParam(query, it -> first, it -> second);

                return baseUrlStr(stype, predicate, query);
            }


            virtual inline QString boolToStr(const bool & val) { return val ? QStringLiteral("true") : QStringLiteral("false"); }
            virtual inline QUrlQuery genDefaultParams(const QuerySourceType & /*stype*/ = qst_api) { return QUrlQuery(); }

            inline QString encodeStr(const QString & str) const { return QUrl::toPercentEncoding(str); }
            inline QString decodeStr(const QString & str) const { return QUrl::fromPercentEncoding(str.toLatin1()); }

            inline void setParam(QUrlQuery & query, const QString & name, const QVariant & value) {
                switch (value.type()) {
                    case QVariant::Int:             { setParam(query, name, value.toInt()); break;}
//                    case QVariant::LongLong:        { setParam(query, name, value.toLongLong()); break;}

//                    case QVariant::UInt:
//                    case QVariant::ULongLong:       { setParam(query, name, value.toULongLong()); break;}

                    case QVariant::Double:          { setParam(query, name, value.toDouble()); break;}

                    case QVariant::String:          { setParam(query, name, value.toString()); break;}
                    case QVariant::StringList:      { setParam(query, name, value.toStringList()); break;}

                    case QVariant::Bool:            { setParam(query, name, value.toBool()); break;}

                    // Char = QMetaType::QChar,
                    // Map = QMetaType::QVariantMap,
                    // List = QMetaType::QVariantList,
                    // ByteArray = QMetaType::QByteArray,
                    // BitArray = QMetaType::QBitArray,
                    default: ;
                }
            }

            inline void setParam(QUrlQuery & query, const QString & name, const bool value) {
                query.addQueryItem(name, boolToStr(value));
            }
            inline void setParam(QUrlQuery & query, const QString & name, const int value) {
                if (value == IGNORE_PARAM) return;
                query.addQueryItem(name, QString::number(value));
            }
//            inline void setParam(QUrlQuery & query, const QString & name, const quint64 value) {
//                query.addQueryItem(name, QString::number(value));
//            }
            inline void setParam(QUrlQuery & query, const QString & name, const double value) {
                if (value == IGNORE_PARAM) return;
                query.addQueryItem(name, QString::number(value));
            }
            inline void setParam(QUrlQuery & query, const QString & name, const QString & value) { query.addQueryItem(name, value); }
            inline void setParam(QUrlQuery & query, const QString & name, const QStringList & values, bool as_one_key = true, char join_symb = ',') {
                if (values.isEmpty()) return;

                if (as_one_key)
                    setParam(query, name, values.join(join_symb));
                else
                    for(QStringList::ConstIterator val = values.constBegin(); val != values.constEnd(); val++)
                        query.addQueryItem(name, *val);
            }
        public:
            Headers dntHeader() { return {{ QStringLiteral("DNT"), QStringLiteral("1")}}; }
        };
    }
}

#endif // IQUERIABLE
