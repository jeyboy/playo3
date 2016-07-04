#ifndef SOUNDCLOUD_SITE_GROUP
#define SOUNDCLOUD_SITE_GROUP

#include "defines.h"
#include "../interfaces/igroup.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class Group : public QueryBase, public IGroup {
                public:
                    QJsonObject groupInfo(const QString & group_id) {
                        sRequest(
                            baseUrlStr(qst_site_def, path_groups % '/' % group_id, {}),
                            call_type_json, 0, proc, IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue groupsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                        return pRequest(baseUrlStr(qst_site_alt1, QStringLiteral("tracks/%1/groups").arg(track_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            COLLECTION_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue groupsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // def by site request is 5000
    //                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                        return pRequest(
                            baseUrlStr(
                                qst_site_def, path_user_groups.arg(user_id),
                                {{QStringLiteral("representation"), QStringLiteral("mini")}}
                            ),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            COLLECTION_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
    //                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                        return pRequest(
                            baseUrlStr(
                                qst_site_alt1, QStringLiteral("search/groups"),
                                {
                                    {tkn_q, name},
                                    {QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base)},
                                    {QStringLiteral("sc_a_id"), generateMark()},
                                    {QStringLiteral("facet"), QStringLiteral("model")}
                                }
                            ),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_GROUP
