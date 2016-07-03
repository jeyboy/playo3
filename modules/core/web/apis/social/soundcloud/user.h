#ifndef SOUNDCLOUD_USER
#define SOUNDCLOUD_USER

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SiteUser : SiteBase {
            protected:
                QJsonValue usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    QUrlQuery query = genDefaultParams(qst_html_alt1);
                    setSearchPredicate(query, name);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("place"));

                    return pRequest(
                        baseUrlStr(
                            qst_html_alt1,
                            QStringLiteral("search/users"),
                            query
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch,
                        IQUERY_DEF_FIELDS, call_method_get, headers()
                    );
                }
            };

            class ApiUser : public ApiBase {
            protected:
            };
        }
    }
}

#endif // SOUNDCLOUD_USER
