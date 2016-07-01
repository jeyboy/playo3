#ifndef FOURSHARED_REQUEST_SITE
#define FOURSHARED_REQUEST_SITE

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class RequestSite : public virtual IQueriable {
            protected:
                enum CategoryTypes {
                    music = 1, video = 2, photo = 3, archive = 4, book = 5,
                    program = 6, web = 7, mobile = 8, android = 10
                };

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

                QJsonValue popularSite(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(
                            DMT_AUDIO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/lastmonth/CAQD/%1/music").arg(OFFSET_TEMPLATE),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_tracks1
                            )
                        );

                    if (limits.include_video())
                        res.insert(
                            DMT_VIDEO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/lastmonth/CAQD/%1/video").arg(OFFSET_TEMPLATE),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_video1
                            )
                        );

                    return res;
                }

                QJsonValue searchProcSite(const SearchLimit & limits) {
                    QJsonObject res;

                    if (limits.include_audio())
                        res.insert(
                            DMT_AUDIO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/CCQD/%1/music/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_tracks1
                            )
                        );

                    if (limits.include_video())
                        res.insert(
                            DMT_VIDEO,
                            pRequest(
                                QStringLiteral("http://search.4shared.com/q/CCQD/%1/video/%2").arg(OFFSET_TEMPLATE, limits.predicate),
                                call_type_html,
                                rulesSite(limits.start_offset, limits.items_limit, limits.requests_limit),
                                0,
                                proc_video1
                            )
                        );

                    return res;
                }
            };
        }
    }
}

#endif // FOURSHARED_REQUEST_SITE
