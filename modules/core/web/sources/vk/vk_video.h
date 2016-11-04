#ifndef VK_VIDEO
#define VK_VIDEO

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Video : public Base {
            public:
                enum VideoSearchSort { vss_creation_date = 0, vss_duration = 1, vss_relativity = 2 };

                //curl 'https://vk.com/al_video.php' -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Content-Type: application/x-www-form-urlencoded' -H 'Cookie: remixlang=0; remixstid=1076597543_28815117269d9b09eb; remixflash=20.0.0; remixscreen_depth=24; remixdt=0; remixrefkey=5c3f82fa2d4f8a0088; remixseenads=2; remixab=1; audio_vol=100; remixsid=92e5eac654cea2c7a7313fbab1e21114008c330e92fe3d5c21598; remixsslsid=1; remixvkcom=1' -H 'DNT: 1' -H 'Host: vk.com' -H 'Referer: https://vk.com/video' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' --data 'act=s&al=1&preload=1&section=all'

                QString videoUrl(const QString & video_id) { // video id should by in format %owner_id_%video_id
                    Response * response = Manager::prepare() -> postFollowed(
                        LSTR("https://vk.com/al_video.php?act=show&al=1&autoplay=1&force_no_repeat=1&preload=1&video=%1").arg(video_id),
                        dntHeader()
                    );

                    QString resp = response -> toText();

                    QRegularExpressionMatch match;
                    if (resp.indexOf(QRegularExpression("vars = (.+)(?<=\\});"), 0, &match) != -1) {
                        QJsonObject obj = QJsonDocument::fromJson(match.captured(1).toUtf8()).object();

                        QStringList available_keys = obj.keys();
                        QMap<int, QHash<QString, QString> > pathes;
                        QString cache_key = LSTR("cache");
                        QString url_key = LSTR("url");

                        QRegularExpression cache_reg(LSTR("(%1|%2)(\\d+)").arg(cache_key, url_key));

                        for(QStringList::Iterator key = available_keys.begin(); key != available_keys.end(); key++) {
                            QRegularExpressionMatch res = cache_reg.match(*key);
                            if (res.hasMatch())
                                pathes[res.captured(2).toInt()].insert(res.captured(1), obj.value(*key).toString());
                        }

                        // INFO: take middle quality // need to add option in settings about preferable quality and take closest to chosen by user
                        int size = pathes.size();
                        if (size == 0)
                            return QString();

                        int quality_index = size / 2 + (size > 1 ? size % 2 : 0);
                        QHash<QString, QString> variants = pathes[pathes.keys()[quality_index]];

                        if (variants.contains(cache_key))
                            return variants[cache_key];
                        else
                            return variants[url_key];
                    }

                    return QString();
                }

                QJsonValue videoInfo(const QUrlQuery & args) { return videoInfo(args.queryItemValue(CMD_ID)); }
                //TODO: write me
                QJsonValue videoInfo(const QString & id) { return QJsonObject(); }

                QJsonValue videoSearch(const QUrlQuery & args) { return videoSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue videoSearch(const SearchLimit & limits, QJsonArray * arr = 0) { // count max eq 200 , limit is 1000
                    SourceFlags perm = permissions(sf_video_by_title);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
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

                        default: Logger::obj().write(name(), "video Search is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_video, block_content);
                }

                QJsonValue videoByUser(const QUrlQuery & args) {
                    return videoByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt()
                    );
                }
                QJsonValue videoByUser(const QString & user_id, int offset = 0) {
                    SourceFlags perm = permissions(sf_video_by_user);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site: {
                            Response * req_response = Manager::prepare() -> postFollowed(
                                IQueriable::baseUrlStr(
                                    qst_site, LSTR("al_video.php"),
                                    {
                                        { LSTR("act"),          LSTR("load_videos_silent") },
                                        { LSTR("al"),           LSTR("1") },
                                        { LSTR("is_layer"),     0 },
                                        { LSTR("need_albums"),  1 },
                                        { LSTR("offset"),       offset },
                                        { LSTR("oid"),          user_id },
                                        { LSTR("rowlen"),       3 },
                                        { LSTR("section"),      LSTR("all") }
                                    }
                                ),
                                dntHeader()
                            );

                            RESPONSE_TO_JSON_PARTS(req_response);
                            QJsonObject all_block = JSON_OBJ(json_parts[0].toObject(), LSTR("all"));
                            QJsonArray items = JSON_ARR(all_block, LSTR("list"));
                            prepareVideo(items, block_content);

                            int new_offset = offset + block_content.size();

                            QueriableResponse response(
                                block_content, QString::number(new_offset), 0, 1,
                                new_offset >= JSON_INT(all_block, LSTR("total"))
                            );

                            QJsonArray sets = json_parts[1].toArray();
                            QJsonArray mod_sets;
                            QString owner_id = userID(); // sets did not contains owner id

                            for(QJsonArray::Iterator set = sets.begin(); set != sets.end(); set++) {
                                QJsonObject obj;
                                QJsonArray fields = (*set).toArray();

                                QString id = JSON_CONV_STR(fields[6]);

                                // 1 - items amount in set
                                // 2 - art url
                                obj.insert(tkn_id, id);
                                obj.insert(tkn_title, fields[0].toString());
                                obj.insert(
                                    tkn_loadable_cmd,
                                    Cmd::build(
                                        sourceType(), cmd_mtd_load_set_data,
                                        {{CMD_ID, QString(owner_id % '_' % id)}, {CMD_MEDIA_TYPE, dmt_video}}
                                    ).toString()
                                );
                                mod_sets << obj;
                            }

                            return QJsonArray()
                                << prepareBlock(dmt_video_set, mod_sets, {{tkn_dir_name, LSTR("Videos")}})
                                << prepareBlock(dmt_video, cmd_mtd_video_by_user, response, {{tkn_dir_name, LSTR("Videos")}}, {{CMD_ID, user_id}});
                        break;}

                        case sf_api: {
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

                                QString id = JSON_CSTR(obj, tkn_id);
                                QString owner_id = JSON_CSTR(obj, tkn_owner_id);

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
                                << prepareBlock(dmt_video_set, mod_sets, {{tkn_dir_name, LSTR("Videos")}})
                                << prepareBlock(dmt_video, content.value(block_items_video), {{tkn_dir_name, LSTR("Videos")}});
                        break;}

                        default: Logger::obj().write(name(), "video Search is not accessable", Logger::log_error);
                    }

                    return QJsonArray();
                }

                QJsonValue videoByPlaylist(const QUrlQuery & args) {
                    return videoByPlaylist(args.queryItemValue(CMD_ID));
                }
                QJsonValue videoByPlaylist(const QString & playlist_id) { // not finished
                    SourceFlags perm = permissions(sf_video_by_playlist);
                    QJsonArray block_content;

                    QStringList parts = playlist_id.split('_');

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
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

                        default: Logger::obj().write(name(), "videoByPlaylist is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_video, block_content);
                }

                QJsonValue videoByCategory(const QUrlQuery & args) {
                    return videoByCategory(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoByCategory(const QString & category_id) {
                    QJsonArray cats = EXTRACT_ITEMS(videoCategories().toObject());

                    for(QJsonArray::Iterator cat = cats.begin(); cat != cats.end(); cat++) {
                        QJsonObject cat_obj = (*cat).toObject();

                        if (JSON_CSTR(cat_obj, tkn_id) == category_id) {
                            return prepareVideoCategoryBlock(JSON_ARR(cat_obj, tkn_items), category_id, JSON_CSTR(cat_obj, LSTR("next")));
//                            return QJsonObject {
//                                {tkn_content,       JSON_ARR(cat_obj, tkn_items)},
//                                {tkn_media_type,    category_id == LSTR("series") ? dmt_video_set : dmt_video},
//                                {tkn_source_id,     sourceType()},
//                                {tkn_more_cmd,      JSON_STR(cat_obj, tkn_more_cmd)}
//                            };
                        }
                    }

                    return QJsonObject();
                }
                QJsonValue videoByCategory(const QString & category_id, const QString & offset_token) {
                    SourceFlags perm = permissions(sf_video_by_category);
                    QJsonObject response;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            response = sRequest(
                                baseUrlStr(
                                    qst_api, tkn_execute,
                                    {
                                        {
                                            tkn_code, QString(
                                                "var response = API.video.getCatalogSection({"
                                                "    section_id: \"" % category_id % "\","
                                                "    from: \"" % offset_token % "\", count: 16"
                                                "});"
                                                "return {\"" % block_sets % "\": response.items, \"offset_token\": response.next};"
                                            )
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, IQUERY_DEF_FIELDS
                            );
                        break;}

                        default: Logger::obj().write(name(), "video by category is not accessable", Logger::log_error);
                    }

                    return prepareVideoCategoryBlock(JSON_ARR(response, block_sets), category_id, JSON_STR(response, LSTR("offset_token")));
//                    QString offset = JSON_STR(response, LSTR("offset_token"));
//                    QJsonArray block_content = JSON_ARR(response, block_sets);
//                    if (offset.isEmpty())
//                        return prepareBlock(dmt_video, block_content);
//                    else {
//                        QueriableResponse response(block_content, offset, 0, 1, block_content.isEmpty());
//                        return prepareBlock(dmt_video, cmd_mtd_video_by_category, response, {}, {{CMD_ID, category_id}});
//                    }
                }

                QJsonValue videoCategories(const QUrlQuery & args) {
                    return videoCategories(args.queryItemValue(CMD_OFFSET));
                }
                QJsonValue videoCategories(const QString & offset_token = QString()) {
                    SourceFlags perm = permissions(sf_video_categories);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {                        
                            sRequest(
                                baseUrlStr(
                                    qst_api,
                                    LSTR("video.getCatalog"),
                                    {
                                        {LSTR("filters"), LSTR("feed,top,ugc,series,other")},
                                        {LSTR("count"), LSTR("0")}, // affect only on other cats amount - max eq to 16
                                        {LSTR("from"), offset_token},
                                        {LSTR("items_count"), LSTR("16")}
                                    }
                                ), call_type_json, &block_content, proc_json_extract, IQUERY_DEF_FIELDS << LSTR("items")
                            );

                            QJsonArray temp;
                            for(QJsonArray::Iterator cat = block_content.begin(); cat != block_content.end(); cat++) {
                                QJsonObject cat_obj = (*cat).toObject();

                                cat_obj.insert(tkn_title, JSON_STR(cat_obj, LSTR("name")));
                                cat_obj.insert(tkn_more_cmd,
                                    Cmd::build(
                                        sourceType(), cmd_mtd_video_by_category,
                                        {
                                            {CMD_ID,        JSON_CSTR(cat_obj, tkn_id)},
                                            {CMD_OFFSET,    JSON_CSTR(cat_obj, LSTR("next"))}
                                        }
                                    ).toString()
                                );

                                temp << cat_obj;
                            }

                            block_content = temp;
                        break;}

                        default: Logger::obj().write(name(), "video categories is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_video_set, block_content);
                }
            };
        }
    }
}

#endif // VK_VIDEO
