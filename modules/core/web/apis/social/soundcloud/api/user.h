#ifndef SOUNDCLOUD_API_USER
#define SOUNDCLOUD_API_USER

#include "defines.h"
#include "../interfaces/iuser.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class User : public QueryBase, public IUser {
                public:
                    QJsonValue userFollowings(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(
                                qst_json_def,
                                path_user_followings.arg(user_id),
                                genDefaultParams(qst_json_def)
                            ),
                            call_type_json,
                            rules(offset, count),
                            0,
                            proc_json_patch
                        );
                    }

                    QJsonValue userFollowers(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(
                                qst_json_def,
                                path_user_followers.arg(user_id),
                                genDefaultParams(qst_json_def)
                            ),
                            call_type_json,
                            rules(offset, count),
                            0,
                            proc_json_patch
                        );
                    }

                    QJsonValue usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        QUrlQuery query = genDefaultParams(qst_json_def);
                        setSearchPredicate(query, name);

                        return pRequest(
                            baseUrlStr(qst_json_def, path_users, query),
                            call_type_json,
                            rules(offset, count),
                            0,
                            proc_json_patch
                        );

    //                    return lQuery(userByNameUrl(name), queryRules(count), wrap);
                    }
                    QJsonArray userById(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(qst_json_def, path_users % '/' % user_id, genDefaultParams(qst_json_def)),
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
}

#endif // SOUNDCLOUD_API_USER
