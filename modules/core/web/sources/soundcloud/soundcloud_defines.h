#ifndef SOUNDCLOUD_BASE_DEFINES
#define SOUNDCLOUD_BASE_DEFINES

#include "modules/core/interfaces/isource_perm_flags.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"

#include "modules/core/web/grabber_keys.h"
#include "soundcloud_keys.h"

#define SOUNDCLOUD_PAGES_LIMIT 25
#define SOUNDCLOUD_ITEMS_LIMIT 2000
#define SOUNDCLOUD_PER_REQUEST_LIMIT 200
#define SOUNDCLOUD_IDS_PER_REQUEST 25

#define SOUNDCLOUD_PER_REQUEST_LIMIT_SET 2
#define SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET 10

#define COLLECTION_FIELDS IQUERY_DEF_FIELDS << QStringLiteral("collection")

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Base : public IQueriable {
            protected:
                virtual Permissions permissions(const PermitRequest & req_perm = pr_search_media) = 0;

                inline void setAudioTypesParam(QUrlQuery & query) { setParam(query, tkn_types, val_audio_types); }
                inline void setAudioTypesParamOriginal(QUrlQuery & query) { setParam(query, tkn_types, val_audio_org_types); }
                inline void setAudioTypesParamRemix(QUrlQuery & query) { setParam(query, tkn_types, val_audio_rmx_types); }


                inline void setGenreLimitation(QUrlQuery & query, const QString & genre) { setParam(query, tkn_genres, genre); }

                QUrlQuery trackSearchQuery(const QString & predicate, const QString & genre, bool hottest = false) {
                    QUrlQuery query = genDefaultParams(qst_api_def);

                    setAudioTypesParam(query);

                    setParam(query, tkn_order, hottest ? val_hotness_order : val_created_at_order);
                    setParam(query, tkn_genres, genre);
                    setParam(query, tkn_q, predicate);

                    return query;
                }

                QString generateMark() {
                    const QString possibleCharacters = QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789");
                    int valid_range = possibleCharacters.length();

                    QString randomString;
                    for(int i = 0; i < 36; ++i) {
                        int index = qrand() % valid_range;
                        randomString.append(possibleCharacters.at(index));

                        if (i == 8 || i == 13 || i == 18 || i == 23) {
                            randomString.append('-');
                            i++;
                        }
                    }
                    return randomString;
                }

                Headers headers() {
                    return {
                        {QStringLiteral("DNT"), QStringLiteral("1")},
                        {QStringLiteral("Origin"), QStringLiteral("https://soundcloud.com")},
                        {QStringLiteral("Referer"), QStringLiteral("https://soundcloud.com")}
                    };
                }

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
        }
    }
}

#endif // SOUNDCLOUD_BASE_DEFINES