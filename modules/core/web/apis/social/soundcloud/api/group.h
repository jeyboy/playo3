#ifndef SOUNDCLOUD_API_GROUP
#define SOUNDCLOUD_API_GROUP

#include "defines.h"
#include "../interfaces/igroup.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Group : public QueryBase, public IGroup {
                protected:
                    QJsonValue groupsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(
                                qst_api_def,
                                path_user_groups.arg(user_id),
                                genDefaultParams(qst_api_def)
                            ),
                            call_type_json,
                            rules(offset, count),
                            0,
                            proc_json_patch
                        );
                    }

                    QJsonValue groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        QUrlQuery query = genDefaultParams(qst_api_def);
                        setSearchPredicate(query, name);

                        return pRequest(
                            baseUrlStr(
                                qst_api_def,
                                path_groups,
                                query
                            ),
                            call_type_json,
                            rules(offset, count),
                            0,
                            proc_json_patch
                        );
                    }

                    QJsonObject groupInfo(const QString & group_id) {
                        return sRequest(
                            baseUrlStr(
                                qst_api_def,
                                path_groups % '/' % group_id,
                                genDefaultParams(qst_api_def)
                            ),
                            call_type_json, 0, proc_json_patch
                        );
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_GROUP
