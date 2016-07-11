#ifndef VK_VIDEO
#define VK_VIDEO

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Video : public Base {
            public:
                enum VideoSearchSort { vss_creation_date = 0, vss_duration = 1, vss_relativity = 2 };

                QString videoSearchUrl(const SearchLimit & limits) {
                    // count max eq 200 , limit is 1000
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code, QString(
                        "var limit = " % QString::number(qMin(1000, limits.items_limit)) % ";"
                        "var search = []; var rule = true;"
                        "do {"
                        "    var count = limit - search.length;"
                        "    if (count > 200) count = 200;"
                        "    var items = API.video.search({"
                        "        q: \"" % encodeStr(limits.predicate) % "\", count: count, offset: search.length, lyrics: 0,"
                        "        adult: " % boolToStr(true) % ","
                        "        sort: " % QString::number(limits.in_relative() ? vss_relativity : vss_creation_date) % ","
                        "        search_own: " % boolToStr(limits.in_owns()) % ""
                        "    }).items;"
                        "    search = search %2b items;"
                        "    rule = search.length < limit && items.length != 0;"
                        "} while(rule);"
                        "return {" % tkn_video_list % ": search};"
                    ));

                    return baseUrlStr(qst_api_def, tkn_execute, query);
                }

                QJsonArray videoSearch(const SearchLimit & limits, QJsonArray * arr = 0) {
                    return saRequest(
                        videoSearchUrl(limits),
                        call_type_json,
                        arr,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_video_list
                    );
                }
            };
        }
    }
}

#endif // VK_VIDEO
