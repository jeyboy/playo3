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
            proc_none = 0, proc_json_wrap, proc_json_extract,
            proc_tracks1, proc_tracks2, proc_tracks3, proc_artists1, proc_artists2, proc_artists3, proc_genres1, proc_genres2, proc_genres3
        };

        struct PolyQueryRules {
            PolyQueryRules(ApiCallIterType _call_iter, ApiCallIterMethod _call_item_method = call_iter_method_offset, int _items_total_limit = DEFAULT_ITEMS_LIMIT, int _requests_limit = DEFAULT_REQUESTS_LIMIT, int _start_offset = 0) {
                call_iter = _call_iter;
                call_item_method = _call_item_method;
                items_total_limit = _items_total_limit;
                requests_limit = _requests_limit;
                start_offset = _start_offset;
            }

            ApiCallIterMethod call_item_method;
            ApiCallIterType call_iter;
            int items_total_limit;
            int requests_limit;
            int start_offset;
        };

        struct QueriableArg {
            QueriableArg(QJsonArray * arr, const QString & url, const ApiCallType & call_type, const AdditionalProc & post_proc = proc_none,
                const QString & field = QString(), QObject * error_receiver = 0)
                : url_template(url), request_url(url), call_type(call_type), call_amount(call_solo), call_method(call_method_get), post_proc(post_proc),
                  arr(arr), field(field), items_fact_count(0), requests_fact_count(0), error_receiver(error_receiver), last_result_is_empty(false), forse_completing(false)
            {}

            void setPolyLimitations(ApiCallIterType _call_iter, int _items_total_limit = DEFAULT_ITEMS_LIMIT,
                                    int _requests_limit = DEFAULT_REQUESTS_LIMIT, int _start_offset = 0, ApiCallIterMethod _call_item_method = call_iter_method_offset)
            {
                call_amount = call_poly;
                requests_limit = _requests_limit;
                call_iter = _call_iter;
                items_total_limit = _items_total_limit;
                start_offset = _start_offset;
                call_item_method = _call_item_method;
                prepareRequestUrl();
            }

            void setPolyLimitations(const PolyQueryRules & rules) {
                setPolyLimitations(
                    rules.call_iter,
                    rules.items_total_limit,
                    rules.requests_limit,
                    rules.start_offset,
                    rules.call_item_method
                );
            }

            void iterateCounters() {
                requests_fact_count++;
                start_offset += (call_iter == call_iter_type_page ? 1 : arr -> size() - items_fact_count);
                items_fact_count = arr -> size();
                prepareRequestUrl();
            }

            void prepareRequestUrl() {
                if (call_item_method == call_iter_method_offset)
                    request_url = url_template.replace(OFFSET_TEMPLATE, QString::number(start_offset));
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

            QString field;
            int items_total_limit, requests_limit;
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

                iterateCounters();
            }

            QJsonValue last() { return arr -> last(); }
        };


    }
}

#endif // QUERIABLE_ARG
