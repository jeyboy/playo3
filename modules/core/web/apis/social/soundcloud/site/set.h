#ifndef SOUNDCLOUD_SITE_SET
#define SOUNDCLOUD_SITE_SET

#include "defines.h"
#include "../interfaces/iset.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class Set : public QueryBase, public ISet {
                public:
                    QJsonValue byType(const SetType & setType, const QString & genre = QStringLiteral("all-music")) { // next_href
                        QUrlQuery query = genDefaultParams(qst_html_alt1);
                        query.addQueryItem(QStringLiteral("genre"), QStringLiteral("soundcloud:genres:") % genre);
                        query.addQueryItem(QStringLiteral("kind"),
                            setType == new_hot ? QStringLiteral("trending") : QStringLiteral("top")
                        );
                        // linked_partitioning=1

                        QJsonObject obj = sRequest(
                            baseUrlStr(
                                qst_html_alt1,
                                QStringLiteral("charts"),
                                query
                            ),
                            call_type_json, 0, proc,
                            IQUERY_DEF_FIELDS, call_method_get,
                            headers()
                        );
                        QJsonArray res; // "last_updated":"2016-07-01T05:27:38Z"

                        QJsonArray collection = obj.value(QStringLiteral("collection")).toArray();
                        for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++)
                            res.append((*it).toObject().value(QStringLiteral("track")));

                        return res;
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_SET
