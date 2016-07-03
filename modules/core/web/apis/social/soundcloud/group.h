#ifndef SOUNDCLOUD_GROUP
#define SOUNDCLOUD_GROUP

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SiteGroup : SiteBase {
            protected:
                QJsonValue groupsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                    QUrlQuery query = genDefaultParams(qst_html_alt1);

                    return pRequest(
                        baseUrlStr(
                            qst_html_alt1,
                            QStringLiteral("tracks/%1/groups").arg(track_id),
                            query
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch,
                        COLLECTION_FIELDS, call_method_get, headers()
                    );
                }

                QJsonValue groupsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // def by site request is 5000
                    QUrlQuery query = genDefaultParams(qst_html_def);
                    setParam(query, QStringLiteral("representation"), QStringLiteral("mini"));
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                    return pRequest(
                        baseUrlStr(
                            qst_html_def,
                            QStringLiteral("users/%1/groups").arg(user_id),
                            query
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch,
                        COLLECTION_FIELDS, call_method_get, headers()
                    );
                }

                QJsonArray groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    QUrlQuery query = genDefaultParams(qst_html_alt1);
                    setSearchPredicate(query, name);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("model"));

                    return pRequest(
                        baseUrlStr(
                            qst_html_alt1,
                            QStringLiteral("search/groups"),
                            query
                        ),
                        call_type_json, rules(offset, count), 0, proc_json_patch,
                        IQUERY_DEF_FIELDS, call_method_get, headers()
                    );
                }

                QJsonObject groupInfo(const QString & group_id) {
                    sRequest(
                        baseUrlStr(
                            qst_html_def,
                            QStringLiteral("groups/") % group_id,
                            genDefaultParams(qst_html_def)
                        ),
                        call_type_json, 0, proc,
                        IQUERY_DEF_FIELDS, call_method_get, headers()
                    );
                }
            };

            class ApiGroup : public ApiBase {
            protected:
            };
        }
    }
}

#endif // SOUNDCLOUD_GROUP
