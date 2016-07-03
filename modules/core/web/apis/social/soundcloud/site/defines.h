#ifndef SOUNDCLOUD_SITE_DEFINES
#define SOUNDCLOUD_SITE_DEFINES

#include "modules/core/web/apis/social/soundcloud/interfaces/defines.h"

#define SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET 10
#define COLLECTION_FIELDS IQUERY_DEF_FIELDS << QStringLiteral("collection")

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Site {
                class QueryBase : public Base, public virtual IQueriable {
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

    //                QJsonObject request(const QString & url, const AdditionalProc & proc = proc_json_patch) {
    //                    return sRequest(
    //                        url, call_type_json, 0, proc,
    //                        IQUERY_DEF_FIELDS, call_method_get,
    //                        headers()
    //                    );
    //                }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_SITE_DEFINES
