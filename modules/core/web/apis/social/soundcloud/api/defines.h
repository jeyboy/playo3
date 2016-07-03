#ifndef SOUNDCLOUD_API_DEFINES
#define SOUNDCLOUD_API_DEFINES

#include "modules/core/web/apis/social/soundcloud/interfaces/defines.h"

#define SOUNDCLOUD_PER_REQUEST_LIMIT_SET 2
#define COLLECTION_FIELDS IQUERY_DEF_FIELDS << QStringLiteral("collection")

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class QueryBase : public Base, public virtual IQueriable {
                protected:
                    inline void setAudioTypesParam(QUrlQuery & query) { setParam(query, tkn_types, val_audio_types); }

                    // add to search
                    inline void setAudioTypesParamOriginal(QUrlQuery & query) { setParam(query, tkn_types, val_audio_org_types); }
                    inline void setAudioTypesParamRemix(QUrlQuery & query) { setParam(query, tkn_types, val_audio_rmx_types); }


                    inline void setGenreLimitation(QUrlQuery & query, const QString & genre) { setParam(query, tkn_genres, genre); }
                    inline void setOrder(QUrlQuery & query, bool hottest) { setParam(query, tkn_order, hottest ? val_hotness_order : val_created_at_order); }

                    QUrlQuery trackSearchQuery(const QString & predicate, const QString & genre, bool hottest = false) {
                        QUrlQuery query = genDefaultParams(qst_api_def);
                        setAudioTypesParam(query);
                        setOrder(query, hottest);

                        if (!genre.isEmpty())
                            setGenreLimitation(query, genre);

                        if (!predicate.isEmpty())
                            setSearchPredicate(query, predicate);

                        return query;
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_DEFINES
