#ifndef SOUNDCLOUD_DEFINES
#define SOUNDCLOUD_DEFINES

#include "modules/core/web/interfaces/iqueriable.h"
#include "api_keys.h"

#define SOUNDCLOUD_PAGES_LIMIT 25
#define SOUNDCLOUD_ITEMS_LIMIT 2000
#define SOUNDCLOUD_PER_REQUEST_LIMIT 200
#define SOUNDCLOUD_PER_REQUEST_LIMIT_SET 2
#define SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET 10
#define SOUNDCLOUD_IDS_PER_REQUEST 25

//#define OFFLINE_RESTPOINT_V1 QStringLiteral("https://api.soundcloud.com/")
//#define OFFLINE_RESTPOINT_V2 QStringLiteral("https://api-v2.soundcloud.com/")
#define COLLECTION_FIELDS IQUERY_DEF_FIELDS << QStringLiteral("collection")

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Base {
            protected:
                inline void setSearchPredicate(QUrlQuery & query, const QString & predicate) { setParam(query, tkn_q, predicate); }
                inline void setIdsFilter(QUrlQuery & query, const QStringList & uids) { setParam(query, tkn_ids, uids); }

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
                inline void setAudioTypesParam(QUrlQuery & query) { setParam(query, tkn_types, val_audio_types); }

                // add to search
                inline void setAudioTypesParamOriginal(QUrlQuery & query) { setParam(query, tkn_types, val_audio_org_types); }
                inline void setAudioTypesParamRemix(QUrlQuery & query) { setParam(query, tkn_types, val_audio_rmx_types); }


                inline void setGenreLimitation(QUrlQuery & query, const QString & genre) { setParam(query, tkn_genres, genre); }
                inline void setOrder(QUrlQuery & query, bool hottest) { setParam(query, tkn_order, hottest ? val_hotness_order : val_created_at_order); }
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

#endif // SOUNDCLOUD_DEFINES
