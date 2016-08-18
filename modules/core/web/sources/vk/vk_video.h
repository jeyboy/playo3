#ifndef VK_VIDEO
#define VK_VIDEO

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Video : public Base {
            public:
                enum VideoSearchSort { vss_creation_date = 0, vss_duration = 1, vss_relativity = 2 };

                QString videoUrl(const QString & video_id) { // video id should by in format %owner_id_%video_id
                    Response * response = Manager::prepare() -> postFollowed(
                        QStringLiteral("https://vk.com/al_video.php?act=show&al=1&autoplay=1&force_no_repeat=1&preload=1&video=%1").arg(video_id),
                        dntHeader()
                    );

                    QString resp = response -> toText();

                    QRegularExpressionMatch match;
                    if (resp.indexOf(QRegularExpression("vars = ([^;]+);"), 0, &match) != -1) {
                        QJsonObject obj = QJsonDocument::fromJson(match.captured(1).toUtf8()).object();
                        int i = 0;
                        // TODO: finish me
                    }

                    return QString();
                }

                QJsonValue videoSearch(const QUrlQuery & args) { return videoSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue videoSearch(const SearchLimit & limits, QJsonArray * arr = 0) { // count max eq 200 , limit is 1000
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api, tkn_execute,
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
                                                "return {\"" % block_items_video % "\": search};"
                                            )
                                        }
                                    }
                                ),
                                call_type_json, arr, proc_json_extract, IQUERY_DEF_FIELDS << block_items_video
                            );
                        break;}

                        default: Logger::obj().write("VK", "video Search is not accessable", true);
                    }

                    return prepareBlock(dmt_video, block_content);
                }


                QJsonValue videoByUser(const QUrlQuery & args) {
                    return videoByUser(
                        args.queryItemValue(CMD_ID)
                    );
                }
                QJsonValue videoByUser(const QString & user_id) {
                    QJsonObject content = sRequest(
                        baseUrlStr(
                            qst_api, tkn_execute,
                            {{ tkn_code, query_user_videos(user_id) }}
                        ),
                        call_type_json, 0, proc_json_extract
                    );

                    QJsonArray sets = content.value(block_sets_video).toArray();
                    QJsonArray mod_sets;

                    for(QJsonArray::Iterator set = sets.begin(); set != sets.end(); set++) {
                        QJsonObject obj = (*set).toObject();

                        QString id = idToStr(obj.value(tkn_id));
                        QString owner_id = idToStr(obj.value(tkn_owner_id));

                        obj.insert(
                            tkn_loadable_cmd,
                            Cmd::build(
                                sourceType(), cmd_mtd_load_set_data,
                                {{CMD_ID, QString(owner_id % '_' % id)}, {CMD_MEDIA_TYPE, dmt_video}}
                            ).toString()
                        );

                        obj.insert(tkn_id, id);
                        mod_sets << obj;
                    }

                    return QJsonArray()
                        << prepareBlock(dmt_video, content.value(block_items_video))
                        << prepareBlock(dmt_video_set, mod_sets);
                }


                QJsonValue videoByPlaylist(const QString & playlist_id) { // not finished
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    QStringList parts = playlist_id.split('_');

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api, tkn_execute,
                                    {
                                        {
                                            tkn_code, QString(
                                                "var search = []; var rule = true;"
                                                "while(rule){"
                                                "    var items = API.video.get({"
                                                "        owner_id: " % parts.first() % ","
                                                "        album_id: " % parts.last() % ","
                                                "        count: 200,"
                                                "        offset: search.length"
                                                "    }).items;"
                                                "    search = search %2b items;"
                                                "    rule = search.length < 4000 && items.length != 0;"
                                                "}"
                                                "return {\"" % block_items_video % "\": search};"
                                            )
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, IQUERY_DEF_FIELDS << block_items_video
                            );
                        break;}

                        default: Logger::obj().write("VK", "tracksByPlaylist is not accessable", true);
                    }

                    return prepareBlock(dmt_video, block_content);
                }
            };
        }
    }
}

#endif // VK_VIDEO
