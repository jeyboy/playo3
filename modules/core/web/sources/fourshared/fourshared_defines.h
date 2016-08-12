#ifndef FOURSHARED_DEFINES
#define FOURSHARED_DEFINES

#include "fourshared_keys.h"

#include "modules/core/interfaces/isource.h"
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
            class Base : public virtual ISource, public virtual IQueriable {
            protected:
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4, book = 5,
                    program = 6, web = 7, mobile = 8, android = 10
                };

//                inline void setGenreFilter(QUrlQuery & query, const QString & genre) { setParam(query, tkn_genre, genre); }
//                inline void setArtistFilter(QUrlQuery & query, const QString & artist) { setParam(query, tkn_artist, artist); }

                QJsonObject prepareBlock(const DataMediaType & dmt_val, const SearchLimit & limits, const ICmdMethods & mtd, const QJsonArray & block_content) {
                    QJsonObject block;

                    int source_id = sourceType();

                    block.insert(tkn_content, block_content);
                    block.insert(tkn_media_type, dmt_val);
                    block.insert(tkn_source_id, source_id);

                    if (block_content.size() < limits.items_limit)
                        block.insert(
                            tkn_more_cmd,
                            Cmd::build(source_id, mtd, limits.toICmdParams(block_content.size())).toString()
                        );

                    return block;
                }

                inline Headers siteHeaders() {
                    return Headers({{QStringLiteral("x-security"), Manager::cookie(QStringLiteral("Login"), url_html_site_base)}});
                }

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
