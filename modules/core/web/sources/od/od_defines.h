#ifndef OD_DEFINES
#define OD_DEFINES

#include "modules/core/interfaces/isource_perm_flags.h"
#include "modules/core/interfaces/search_limits.h"
#include "modules/core/web/grabber_keys.h"

#include "od_keys.h"
#include "od_misc.h"

#define OD_LIMIT_PER_REQUEST 100
#define OD_SEARCH_LIMIT 200

namespace Core {
    namespace Web {
        namespace Od {
            class Base : public Misc {
            protected:
                virtual Permissions permissions(const PermitRequest & req_perm = pr_search_media) = 0;

                //                inline void setPagination(QUrlQuery & query, int per_request = OD_LIMIT_PER_REQUEST) {
                //                    setParam(query, tkn_offset, OFFSET_TEMPLATE);
                //                    setParam(query, tkn_limit, qMin(per_request, OD_LIMIT_PER_REQUEST));
                //                }

                PolyQueryRules rules(
                    int offset = 0, int items_limit = DEFAULT_ITEMS_LIMIT,
                    int pages_count = 10, int per_request = OD_LIMIT_PER_REQUEST)
                {
                    return PolyQueryRules(
                        call_iter_type_item,
                        call_iter_method_offset,
                        qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_count, DEFAULT_REQUESTS_LIMIT),
                        tkn_limit,
                        qMin(qMin(per_request, items_limit), OD_LIMIT_PER_REQUEST),
                        tkn_offset,
                        offset
                    );
                }

                PolyQueryRules pageRules(
                    const QString & offset_token, int offset = 1,
                    int items_limit = DEFAULT_ITEMS_LIMIT, int pages_count = 5)
                {
                    return PolyQueryRules(
                       call_iter_type_page, offset, qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_count, DEFAULT_REQUESTS_LIMIT), offset_token
                    );
                }

                inline QString audioUrlStr(const QString & predicate, const std::initializer_list<std::pair<QString, QVariant> > & params = {}) {
                    return baseUrlStr(qst_site_audio, predicate, params);

//                    return url_base_audio % predicate % tkn_coma_dot % genDefaultParams(qst_site).toString() %
//                            (query.isEmpty() ? QString() : '?' % query.toString());
                }

                // info request per ids for items (track / album / artist)
                // param (ids: ids of (track / album / artist) splited by coma)
                inline QString audioInfoUrl(const QStringList & ids) { return audioUrlStr(path_audio_info, {{tkn_ids, ids.join(',')}}); }

                inline QString audioSearchUrl(const QString & predicate) { return audioUrlStr(path_audio_search, {{ tkn_q, predicate }}); } // params : (q: predicate) and pagination attrs
            };
        }
    }
}

#endif // OD_DEFINES
