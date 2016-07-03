#ifndef SOUNDCLOUD_USER
#define SOUNDCLOUD_USER

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SiteUser : SiteBase {
            protected:
                QJsonValue usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("search/users"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    setParam(query, QStringLiteral("q"), name);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("place"));
                    url.setQuery(query);

                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count),
                        0,
                        proc_json_patch
                    );
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_USER
