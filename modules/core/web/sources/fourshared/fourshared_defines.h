#ifndef FOURSHARED_DEFINES
#define FOURSHARED_DEFINES

#include "fourshared_api_keys.h"
#include "modules/core/web/grabber_keys.h"

#include "modules/core/interfaces/isource_perm_flags.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/search_limits.h"
#include "modules/core/misc/file_utils/extensions.h"

//#include "modules/core/media/genres/music_genres.h"

#define FOURSHARED_PAGES_LIMIT 10
#define FOURSHARED_ITEMS_LIMIT 1000
#define FOURSHARED_PER_REQUEST_LIMIT 100

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Base : public IQueriable {
            protected:
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4, book = 5,
                    program = 6, web = 7, mobile = 8, android = 10
                };

//                inline void setGenreFilter(QUrlQuery & query, const QString & genre) { setParam(query, tkn_genre, genre); }
//                inline void setArtistFilter(QUrlQuery & query, const QString & artist) { setParam(query, tkn_artist, artist); }

                virtual Permissions permissions(const PermitRequest & req_perm = pr_search_media) = 0;

                PolyQueryRules rulesApi(
                    int offset = 0, int items_limit = FOURSHARED_ITEMS_LIMIT, int pages_limit = FOURSHARED_PAGES_LIMIT,
                    int per_request = FOURSHARED_PER_REQUEST_LIMIT,
                    ApiCallIterType call_type = call_iter_type_page)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, FOURSHARED_ITEMS_LIMIT),
                        qMin(pages_limit, FOURSHARED_PAGES_LIMIT),
                        tkn_limit,
                        qMin(qMin(per_request, items_limit), FOURSHARED_PER_REQUEST_LIMIT),
                        tkn_offset,
                        offset
                    );
                }

                PolyQueryRules rulesSite(
                    int offset = 0, int items_limit = FOURSHARED_ITEMS_LIMIT,
                    int pages_limit = FOURSHARED_PAGES_LIMIT, ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        offset,
                        qMin(items_limit, FOURSHARED_ITEMS_LIMIT),
                        qMin(pages_limit, FOURSHARED_PAGES_LIMIT)
                    );
                }
            };
        }
    }
}

#endif // FOURSHARED_DEFINES
