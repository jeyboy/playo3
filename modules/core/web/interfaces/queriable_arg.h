#ifndef QUERIABLE_ARG
#define QUERIABLE_ARG

#include <qstring.h>
#include <qobject.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "quariable_defines.h"

namespace Core {
    namespace Web {
        enum ApiCallType { call_type_html, call_type_json };
        enum ApiCallAmount { call_solo, call_poly };
        enum ApiCallMethod { call_method_get, call_method_post };
        enum ApiCallIterType { call_iter_type_page, call_iter_type_item };
        enum ApiCallIterMethod { call_iter_method_offset, call_iter_method_token };
        enum AdditionalProc {
            proc_none = 0, proc_json_wrap = 1, proc_json_extract = 2, proc_patch = proc_json_wrap | proc_json_extract, // patch used if returned [] as response
            proc_tracks1 = 4, proc_tracks2, proc_tracks3, proc_artists1, proc_artists2, proc_artists3, proc_genres1,
            proc_genres2, proc_genres3, proc_video1, proc_video2, proc_video3
        };

        struct PolyQueryRules {
            PolyQueryRules(
               ApiCallIterType _call_iter, ApiCallIterMethod _call_item_method = call_iter_method_offset,
               int _items_total_limit = DEFAULT_ITEMS_LIMIT, int _requests_limit = DEFAULT_REQUESTS_LIMIT,
               QString _limit_field = QString(), int _per_request_limit = 25,
               QString _offset_field = QString(), int _start_offset = 0, const QString _start_offset_token = QString()
            ) {
                call_iter = _call_iter;
                call_item_method = _call_item_method;
                items_total_limit = _items_total_limit;
                requests_limit = _requests_limit;
                start_offset = _start_offset;
                per_request_limit = _per_request_limit;
                offset_field = _offset_field;
                limit_field = _limit_field;
                offset_token = _start_offset_token;
            }

            PolyQueryRules(
               ApiCallIterType _call_iter, int _start_offset = 0,
               int _items_total_limit = DEFAULT_ITEMS_LIMIT, int _requests_limit = DEFAULT_REQUESTS_LIMIT
            ) {
                call_iter = _call_iter;
                call_item_method = call_iter_method_offset;
                items_total_limit = _items_total_limit;
                requests_limit = _requests_limit;
                start_offset = _start_offset;
                per_request_limit = 0;
                offset_field = QString();
                limit_field = QString();
                offset_token = QString();
            }

            ApiCallIterMethod call_item_method;
            ApiCallIterType call_iter;
            int items_total_limit;
            int requests_limit;
            int start_offset;
            int per_request_limit;
            QString offset_field;
            QString offset_token;
            QString limit_field;
        };

        struct QueriableArg {
            QueriableArg(QJsonArray * arr, const QString & url, const ApiCallType & call_type, const AdditionalProc & post_proc = proc_none,
                const QStringList & fields = QStringList(), QObject * error_receiver = 0)
                : url_template(url), request_url(url), call_type(call_type), call_amount(call_solo), call_method(call_method_get), post_proc(post_proc),
                  arr(arr), fields(fields), items_fact_count(arr -> size()), requests_fact_count(0), error_receiver(error_receiver), last_result_is_empty(false), forse_completing(false)
            {}

            void setOffsetPolyLimitations(
                ApiCallIterType _call_iter, int _items_total_limit = DEFAULT_ITEMS_LIMIT, int _requests_limit = DEFAULT_REQUESTS_LIMIT,
                const QString & _offset_field = QString(), int _start_offset = 0,
                const QString & _limit_field = QString(), int _per_request_limit = 1)
            {
                if (!_offset_field.isEmpty())
                    url_template = _attachField(url_template, _offset_field, OFFSET_TEMPLATE);

                if (!_limit_field.isEmpty())
                    url_template = _attachField(url_template, _limit_field, QString::number(_per_request_limit));

                call_amount = call_poly;
                requests_limit = _requests_limit;
                call_iter = _call_iter;
                items_total_limit = _items_total_limit;
                start_offset = _start_offset;
                call_item_method = call_iter_method_offset;
                per_request_limit = _per_request_limit;
                prepareRequestUrl();
            }

            void setTokenPolyLimitations(
                int _items_total_limit = DEFAULT_ITEMS_LIMIT, int _requests_limit = DEFAULT_REQUESTS_LIMIT,
                const QString & _token_field = QString(), const QString & _start_token = QString(),
                const QString & _limit_field = QString(), int _per_request_limit = 1)
            {
                if (!_limit_field.isEmpty())
                    url_template = _attachField(url_template, _limit_field, QString::number(_per_request_limit));

                call_amount = call_poly;
                requests_limit = _requests_limit;
                call_iter = call_iter_type_page;
                items_total_limit = _items_total_limit;
                call_item_method = call_iter_method_token;
                per_request_limit = _per_request_limit;

                if (!_start_token.isEmpty())
                    prepareRequestUrlByToken(_token_field, _start_token);
            }

            void setPolyLimitations(const PolyQueryRules & rules) {
                switch(rules.call_item_method) {
                    case call_iter_method_token: {
                        setTokenPolyLimitations(
                            rules.items_total_limit,
                            rules.requests_limit,
                            rules.offset_field,
                            rules.offset_token,
                            rules.limit_field,
                            rules.per_request_limit
                        );
                    break;}

                    case call_iter_method_offset: {
                        setOffsetPolyLimitations(
                            rules.call_iter,
                            rules.items_total_limit,
                            rules.requests_limit,
                            rules.offset_field,
                            rules.start_offset,
                            rules.limit_field,
                            rules.per_request_limit
                        );
                    break;}
                }
            }

            void iterateCounters() {
                requests_fact_count++;
                start_offset += (call_iter == call_iter_type_page ? 1 : arr -> size() - items_fact_count);
                items_fact_count = arr -> size();
                prepareRequestUrl();
            }

            void prepareRequestUrl() {
                if (call_item_method == call_iter_method_offset) {
                    request_url = url_template;
                    request_url.replace(OFFSET_TEMPLATE, QString::number(start_offset));
                }
            }

            void prepareRequestUrlByToken(const QString & field, const QString & token) {
                if (call_item_method == call_iter_method_token)
                    request_url = _attachField(url_template, field, token);
            }

            QString _attachField(QString & url, const QString & field, const QString & token) {
                char delim = url.indexOf('?') == -1 ? '?' : '&';
                return url + delim + QStringLiteral("%1=%2").arg(field, token);
            }

            QString url_template;
            QString request_url;

            ApiCallType call_type;
            ApiCallAmount call_amount;
            ApiCallMethod call_method;
            ApiCallIterType call_iter;
            ApiCallIterMethod call_item_method;

            AdditionalProc post_proc;

            QJsonArray * arr;

            QStringList fields;
            int items_total_limit, requests_limit, per_request_limit;
            int start_offset;
            int items_fact_count, requests_fact_count;

            QObject * error_receiver;

            bool last_result_is_empty;
            bool forse_completing;

            bool isCompleted() {
                return last_result_is_empty || forse_completing || items_fact_count >= items_total_limit || requests_fact_count >= requests_limit;
            }

            void append(const QJsonObject & item, bool iterate = true) {
                if (!(last_result_is_empty = item.isEmpty()))
                    arr -> append(item);

                if (iterate)
                    iterateCounters();
            }

            void append(const QJsonArray & items) {
                if (!(last_result_is_empty = items.isEmpty()))
                    for(QJsonArray::ConstIterator it = items.constBegin(); it != items.constEnd(); it++)
                        arr -> append(*it);

                if (!last_result_is_empty)
                    iterateCounters();
            }

            QJsonValue last() { return arr -> last(); }
        };
    }
}

#endif // QUERIABLE_ARG
