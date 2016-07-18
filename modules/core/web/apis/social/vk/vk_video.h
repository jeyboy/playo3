#ifndef VK_VIDEO
#define VK_VIDEO

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Video : public Base {
            public:
                enum VideoSearchSort { vss_creation_date = 0, vss_duration = 1, vss_relativity = 2 };

                QJsonArray videoSearch(const SearchLimit & limits, QJsonArray * arr = 0) { // count max eq 200 , limit is 1000

                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return saRequest(
                                baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {
                                        {
                                            tkn_code, QString(
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
                                            )
                                        }
                                    }
                                ),
                                call_type_json, arr, proc_json_extract, QStringList() << tkn_response << tkn_video_list
                            );
                        }

                        default: Logger::obj().write("VK", "video Search is not accessable", true);
                    }

                    return QJsonArray();
                }

                QJsonValue videoByUser(const QString & user_id) {
                    return sRequest(
                        User::baseUrlStr(
                            qst_api_def, tkn_execute,
                            {{ tkn_code, query_user_videos.arg(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );
                }

                QJsonValue videoByPlaylist(const QString & playlist_id) { // not finished
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
//                            return saRequest(
//                                baseUrlStr(
//                                    qst_api_def, QStringLiteral("video.get"),
//                                    {
//                                        { QStringLiteral("album_id"), playlist_id },
//                                        { QStringLiteral("count"), 200}
//                                    }
//                                ),
//                                call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_items
//                            );

                            return saRequest(
                                baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {
                                        {
                                            tkn_code, QString(
                                                "var search = []; var rule = true;"
                                                "do {"
                                                "    var items = API.video.get({"
                                                "        album_id: \"" % playlist_id % "\","
                                                "        count: 200"
                                                "    }).items;"
                                                "    search = search %2b items;"
                                                "    rule = search.length < 4000 && items.length != 0;"
                                                "} while(rule);"
                                                "return {" % tkn_video_list % ": search};"
                                            )
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_video_list
                            );
                        }

                        default: Logger::obj().write("VK", "tracksByPlaylist is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // VK_VIDEO
