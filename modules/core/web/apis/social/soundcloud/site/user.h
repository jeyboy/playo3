#ifndef SOUNDCLOUD_SITE_USER
#define SOUNDCLOUD_SITE_USER

#include "defines.h"
#include "../interfaces/iuser.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class User : public QueryBase, public IUser {
                public:
                    QJsonValue usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
    //                    setParam(query, QStringLiteral("linked_partitioning"), 1);

                        return pRequest(
                            baseUrlStr(
                                qst_site_alt1, QStringLiteral("search/users"),
                                {
                                    { tkn_q, name },
                                    { QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base) },
                                    { QStringLiteral("sc_a_id"), generateMark() },
                                    { QStringLiteral("facet"), QStringLiteral("place") }
                                }
                            ),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue usersByTrackLikes(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(
                                qst_site_alt1, QStringLiteral("tracks/%1/likers").arg(track_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue usersByTrackReposting(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        // linked_partitioning=1
                        return pRequest(
                            baseUrlStr(
                                qst_site_def, QStringLiteral("e1/tracks/%1/reposters").arg(track_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue userFollowings(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(
                                qst_site_def, QStringLiteral("users/%1/followings").arg(user_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }

                    QJsonValue userFollowers(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(
                                qst_site_def, QStringLiteral("users/%1/followers").arg(user_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch,
                            IQUERY_DEF_FIELDS, call_method_get, headers()
                        );
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_USER
