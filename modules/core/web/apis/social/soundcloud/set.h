#ifndef SOUNDCLOUD_SET
#define SOUNDCLOUD_SET

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class SiteSet : SiteBase {
            protected:
                enum ChartType { new_hot = 1, top_50 };

                QJsonValue chart(const ChartType & chartType, const QString & genre = QStringLiteral("all-music")) { // next_href
                    QUrlQuery query = genDefaultParams(qst_html_alt1);
                    query.addQueryItem(QStringLiteral("genre"), QStringLiteral("soundcloud:genres:") % genre);
                    query.addQueryItem(QStringLiteral("kind"),
                        chartType == new_hot ? QStringLiteral("trending") : QStringLiteral("top")
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

            class ApiSet : public ApiBase {
            protected:
            };
        }
    }
}

#endif // SOUNDCLOUD_SET
