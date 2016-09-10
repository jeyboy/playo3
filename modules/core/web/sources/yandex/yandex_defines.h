#ifndef YANDEX_DEFINES_H
#define YANDEX_DEFINES_H

#include "modules/core/interfaces/search_limits.h"

#include "yandex_misc.h"

#define YANDEX_SEARCH_LIMIT 200

namespace Core {
    namespace Web {
        namespace Yandex {
            class Base : public Misc {
                //                PolyQueryRules rules(
                //                    int offset = 0, int items_limit = SOUNDCLOUD_ITEMS_LIMIT, int pages_limit = SOUNDCLOUD_PAGES_LIMIT,
                //                    int per_request = SOUNDCLOUD_PER_REQUEST_LIMIT,
                //                    ApiCallIterType call_type = call_iter_type_page)
                //                {
                //                    return PolyQueryRules(
                //                        call_type,
                //                        call_iter_method_offset,
                //                        qMin(items_limit, SOUNDCLOUD_ITEMS_LIMIT),
                //                        qMin(pages_limit, SOUNDCLOUD_PAGES_LIMIT),
                //                        tkn_limit,
                //                        qMin(qMin(per_request, items_limit), SOUNDCLOUD_PER_REQUEST_LIMIT),
                //                        tkn_offset,
                //                        offset
                //                    );
                //                }

                //filter = albums: genre, pager: {total, page, perPage}, albums,
                //filter = : genre, tracks, artists, albums, pics
                inline QString genresUrl(QString genre = QString(), const QString & filter = QString()) { return url_site_v1 + QStringLiteral("genre.jsx?genre=%1&filter=%2").arg(genre, filter); }
            };
        }
    }
}

#endif // YANDEX_DEFINES_H
