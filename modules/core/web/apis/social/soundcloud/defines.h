#ifndef SOUNDCLOUD_DEFINES
#define SOUNDCLOUD_DEFINES

#include "modules/core/web/interfaces/iqueriable.h"
#include "api_keys.h"

#define SOUNDCLOUD_PAGES_LIMIT 25
#define SOUNDCLOUD_ITEMS_LIMIT 2000
#define SOUNDCLOUD_PER_REQUEST_LIMIT 200
#define SOUNDCLOUD_PER_REQUEST_LIMIT_SET 2
#define SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET 10

#define OFFLINE_RESTPOINT_V1 QStringLiteral("https://api.soundcloud.com/")
#define OFFLINE_RESTPOINT_V2 QStringLiteral("https://api-v2.soundcloud.com/")

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Base {
            protected:
                PolyQueryRules rules(
                    int offset = 0, int items_limit = SOUNDCLOUD_ITEMS_LIMIT, int pages_limit = SOUNDCLOUD_PAGES_LIMIT,
                    int per_request = SOUNDCLOUD_PER_REQUEST_LIMIT,
                    ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, SOUNDCLOUD_ITEMS_LIMIT),
                        qMin(pages_limit, SOUNDCLOUD_PAGES_LIMIT),
                        tkn_limit,
                        qMin(qMin(per_request, items_limit), SOUNDCLOUD_PER_REQUEST_LIMIT),
                        tkn_offset,
                        offset
                    );
                }
            };

            class ApiBase : public Base, public virtual IQueriable {
            protected:

            };

            class SiteBase : public Base, public virtual IQueriable {
            protected:
                QString generateMark() {
                    QCryptographicHash hash(QCryptographicHash::Md5);
                    hash.addData(QString::number(QDateTime::currentMSecsSinceEpoch()).toUtf8());

                    QString res = QString(hash.result());
                    res = res.insert(8, '-'); res = res.insert(13, '-'); res = res.insert(18, '-'); res = res.insert(23, '-');
                    return res;

                    //"184de48a-5c8d-4ef7-908a-b4602d2a667d"
                }

                Headers headers() {
                    return {
                        {QStringLiteral("DNT"), QStringLiteral("1")},
                        {QStringLiteral("Origin"), QStringLiteral("https://soundcloud.com")},
                        {QStringLiteral("Referer"), QStringLiteral("https://soundcloud.com")}
                    };
                }

                QJsonObject request(const QUrl & url, const AdditionalProc & proc = proc_json_patch) {
                    return sRequest(
                        url.toString(), call_type_json, 0, proc,
                        QStringList() << DEF_JSON_FIELD, call_method_get,
                        headers()
                    );
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_DEFINES
