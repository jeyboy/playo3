#ifndef VK_TRACK
#define VK_TRACK

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Track : public Base {
            public:
                enum AudioSearchSort { ass_creation_date = 0, ass_duration = 1, ass_popularity = 2 };

                // curl 'https://vk.com/al_audio.php' -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Content-Type: application/x-www-form-urlencoded' -H 'Cookie: remixlang=0; remixstid=1076597543_28815117269d9b09eb; remixflash=20.0.0; remixscreen_depth=24; remixdt=0; remixrefkey=ce710b7f9b892b478e; remixseenads=0; remixab=1; audio_vol=100; remixsid=8ac0cdcba71ec450bf16ee7749837a9f394452eeeb29d273a48a6; remixsslsid=1; remixcurr_audio=20284990_365364675' -H 'DNT: 1' -H 'Host: vk.com' -H 'Referer: https://vk.com/audios20284990' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' --data 'act=reload_audio&ads_section=audio&ads_showed=&al=1&al_ad=1&ids=20284990_365364675%2C20284990_364502322%2C20284990_359025135%2C20284990_358285225%2C20284990_356796749%2C20284990_365364675'
                QString trackUrl(const QString & track_id) { return trackInfo(track_id).toObject().value(tkn_url).toString(); }

                QString trackLyric(const QString & lyrics_id) { //TODO: finish me // not tested
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return sRequest(
                                baseUrlStr(
                                    qst_api_def, path_lyrics,
                                    { { tkn_lyrics_id, lyrics_id } }
                                ),
                                call_type_json, 0, proc_json_extract
                            ).value(QStringLiteral("hudo")).toString();
                        }

                        default: Logger::obj().write("VK", "track Lyric is not accessable", true);
                    }

                    return QString();
                }

                QJsonValue trackInfo(const QString & track_id) {
                    QJsonArray info = tracksInfo(QStringList() << track_id).toArray();
                    return info.isEmpty() ? QJsonObject() : info[0].toObject();
                }

                QJsonValue tracksInfo(const QStringList & track_ids) {
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {
                                        {
                                            tkn_code,
                                            QString(QStringLiteral("return API.audio.getById({ audios: \"") % track_ids.join(',') % QStringLiteral("\"});"))
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract
                            );
                        }

                        default: Logger::obj().write("VK", "track Lyric is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue userRecommendations(const QUrlQuery & args) {
                    return userRecommendations(
                        args.queryItemValue(CMD_ID),
                        (bool)args.queryItemValue(CMD_PREDICATE).toInt()//,
//                        args.queryItemValue(CMD_OFFSET).toInt(),
//                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue userRecommendations(const QString & user_id, bool randomize) {
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {
                                        { tkn_code, QString(
                                               "var recomendations = API.audio.getRecommendations({"
                                               "user_id: \"" % user_id % "\", "
                                               "   count: 1000, "
                                               "   shuffle: " % boolToStr(randomize) % ""
                                               "});"
                                               "return {" % block_items_audio % ": recomendations };"
                                          )
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, IQUERY_DEF_FIELDS << block_items_audio
                            );
                        break;}

                        default: Logger::obj().write("VK", "userRecommendations is not accessable", true);
                    }

//                    return prepareBlock(dmt_group, cmd_mtd_groups_by_id, response, {{CMD_ID, user_id}, {CMD_PREDICATE, (int)randomize}});
                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue trackRecommendations(const QUrlQuery & args) {
                    return trackRecommendations(
                        args.queryItemValue(CMD_ID),
                        (bool)args.queryItemValue(CMD_PREDICATE).toInt()//,
//                        args.queryItemValue(CMD_OFFSET).toInt(),
//                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue trackRecommendations(const QString & track_id, bool randomize) {
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {
                                        { tkn_code, QString(
                                               "var recomendations = API.audio.getRecommendations({"
                                               "target_audio: \"" % track_id % "\", "
                                               "   count: 1000, "
                                               "   shuffle: " % boolToStr(randomize) % ""
                                               "});"
                                               "return {" % block_items_audio % ": recomendations };"
                                          )
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, IQUERY_DEF_FIELDS << block_items_audio
                            );
                        break;}

                        default: Logger::obj().write("VK", "trackRecommendations is not accessable", true);
                    }

//                    return prepareBlock(dmt_group, cmd_mtd_groups_by_id, response, {{CMD_ID, track_id}, {CMD_PREDICATE, (int)randomize}});
                    return prepareBlock(dmt_audio, block_content);
                }

//                QString audioSearchLimitedUrl(QString & searchStr, int limit) {
//                    QUrlQuery query = genDefaultParams();

//                    setParam(query, tkn_code, QString(
//                        "    var items = API.audio.search({"
//                        "        q: \"" % encodeStr(searchStr) % QStringLiteral("\", count: ") % QString::number(limit) % ", lyrics: 0"
//                        "    }).items;"
//                        "return {" % tkn_audio_list % ": search};"
//                    ));

//                    return baseUrlStr(qst_api_def, tkn_execute, query);
//                }

//                QJsonObject audioSearchLimited(QString & predicate, int limitation) {
//                    return sRequest(
//                        audioSearchLimitedUrl(predicate, limitation),
//                        call_type_json,
//                        0,
//                        proc_json_extract
//                    );

////                    return sQuery(audioSearchLimitedUrl(predicate, limitation), extract);
//                }

                QJsonValue tracksSearch(const QUrlQuery & args) { return tracksSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue tracksSearch(const SearchLimit & limits, QJsonArray * arr = 0, bool autoFix = false) {
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api_def, tkn_execute,
                                    {
                                        {
                                            tkn_code, QString(
                                                "var limit = " % QString::number(qMin(1000, limits.items_limit)) % ";"
                                                "var search = []; var rule = true;"
                                                "do {"
                                                "    var count = limit - search.length;"
                                                "    if (count > 300) count = 300;"
                                                "    var items = API.audio.search({"
                                                "        q: \"" % encodeStr(limits.predicate) % "\", count: count, offset: search.length, lyrics: 0,"
                                                "        auto_complete: " % boolToStr(autoFix) % ","
                                                "        performer_only: " % boolToStr(limits.by_artists()) % ","
                                                "        sort: " % QString::number(limits.by_popularity() ? ass_popularity : ass_creation_date) % ","
                                                "        search_own: " % boolToStr(limits.in_owns()) % ""
                                                "    }).items;"
                                                "    search = search %2b items;"
                                                "    rule = search.length < limit && items.length != 0;"
                                                "} while(rule);"
                                                "return {" % block_items_audio % ": search};"
                                            )
                                        }
                                    }
                                ),
                                call_type_json, arr, proc_json_extract,
                                IQUERY_DEF_FIELDS << block_items_audio
                            );
                        break;}

                        default: Logger::obj().write("VK", "tracksSearch is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue tracksByPlaylist(const QUrlQuery & args) {
                    return tracksByPlaylist(
                        args.queryItemValue(CMD_ID)/*,
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()*/
                    );
                }
                QJsonValue tracksByPlaylist(const QString & playlist_id) { // not finished
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api_def, QStringLiteral("audio.get"),
                                    {
                                        { QStringLiteral("album_id"), playlist_id },
                                        { QStringLiteral("count"), 6000} // max
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_items
                            );
                        break;}

                        default: Logger::obj().write("VK", "tracksByPlaylist is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }
            };
        }
    }
}

#endif // VK_TRACK
