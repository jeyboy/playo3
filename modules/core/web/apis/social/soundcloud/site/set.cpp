#include "set.h"
#include "../api/requests.h"

using namespace Core::Web::Soundcloud::Site;

QJsonValue Set::setByType(const SetType & setType, const QString & genre) { // next_href
    switch(setType) {
        case new_hot:
        case top_50: {
            QJsonObject obj = sRequest(
                baseUrlStr(
                    qst_site_alt1,
                    QStringLiteral("charts"),
                    {
                        // linked_partitioning=1
                        { QStringLiteral("genre"), QStringLiteral("soundcloud:genres:") % genre.isEmpty() ? QStringLiteral("all-music") : genre },
                        { QStringLiteral("kind"), setType == new_hot ? QStringLiteral("trending") : QStringLiteral("top") }
                    }
                ),
                call_type_json, 0, proc, IQUERY_DEF_FIELDS, call_method_get, headers()
            );
            QJsonArray res; // "last_updated":"2016-07-01T05:27:38Z"

            QJsonArray collection = obj.value(QStringLiteral("collection")).toArray();
            for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++)
                res.append((*it).toObject().value(QStringLiteral("track")));

            return res;
        }

        default: return Api::Requests::obj().setByType(setType, genre);
    }
}
