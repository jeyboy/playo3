#ifndef SOUNDCLOUD_GROUP
#define SOUNDCLOUD_GROUP

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SiteGroup : SiteBase {
            protected:
                QJsonValue groupsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("tracks/%1/groups").arg(track_id));
                    QUrlQuery query = genDefaultParams(qst_html);
                    url.setQuery(query);

                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count),
                        0,
                        proc_json_patch,
                        QStringList() << DEF_JSON_FIELD << QStringLiteral("collection")
                    );
                }

                QJsonValue groupsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // def by site request is 5000
                    QUrl url(OFFLINE_RESTPOINT_V1 % QStringLiteral("users/%1/groups").arg(user_id));
                    QUrlQuery query = genDefaultParams(qst_html);
                    setParam(query, QStringLiteral("representation"), QStringLiteral("mini"));
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    url.setQuery(query);

                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count),
                        0,
                        proc_json_patch,
                        QStringList() << DEF_JSON_FIELD << QStringLiteral("collection")
                    );
                }

                QJsonArray groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    QUrl url(OFFLINE_RESTPOINT_V2 % QStringLiteral("search/groups"));
                    QUrlQuery query = genDefaultParams(qst_html);
                    setParam(query, QStringLiteral("q"), name);
//                    setParam(query, QStringLiteral("linked_partitioning"), 1);
                    setParam(query, QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base));
                    setParam(query, QStringLiteral("sc_a_id"), generateMark());
                    setParam(query, QStringLiteral("facet"), QStringLiteral("model"));
                    url.setQuery(query);

                    return pRequest(
                        url.toString(),
                        call_type_json,
                        rules(offset, count),
                        0,
                        proc_json_patch
                    );
                }

                QJsonObject groupInfo(const QString & group_id) {
                    QUrl url(OFFLINE_RESTPOINT_V1 % QStringLiteral("groups/") % group_id);
                    url.setQuery(genDefaultParams(qst_html));
                    return request(url);
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_GROUP
