#ifndef VK_TRACK
#define VK_TRACK

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Track : public Base {
            public:
                enum AudioSearchSort { ass_creation_date = 0, ass_duration = 1, ass_popularity = 2 };

                QString trackUrl(const QString & track_id) {
                    QJsonObject obj = trackInfo(track_id).toObject();
                    return obj.value(tkn_url).toString();
                }

                QString trackLyric(const QString & lyrics_id) { //TODO: finish me // not tested
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            return sRequest(
                                baseUrlStr(
                                    qst_api, path_lyrics,
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
                    QJsonObject info = tracksInfo(QStringList() << track_id).toObject();
                    QJsonArray content = info.value(tkn_content).toArray();
                    return content.isEmpty() ? QJsonObject() : content[0].toObject();
                }

                QJsonValue tracksInfo(const QStringList & track_ids) {
                    Permissions perm = permissions(pr_object_content); // pr_media_content
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site: {
                            Response * response = Manager::prepare() -> postFollowed(
                                baseUrlStr(
                                    qst_site, QStringLiteral("al_audio.php"),
                                    {
                                        { QStringLiteral("act"), QStringLiteral("reload_audio") },
                                        { QStringLiteral("al"), QStringLiteral("1") },
                                        { QStringLiteral("al_ad"), QStringLiteral("1") },
                                        { QStringLiteral("ids"), track_ids.join(',') }
                                    }
                                ), dntHeader()
                            );

                            QJsonArray items = RESPONSE_TO_JSON_ARRAY(response);
                            prepareTracks(items, block_content);
                        break;}

                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api, tkn_execute,
                                    {
                                        {
                                            tkn_code,
                                            QString(QStringLiteral("return API.audio.getById({ audios: \"") % track_ids.join(',') % QStringLiteral("\"});"))
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract
                            );
                        break;}

                        default: Logger::obj().write("VK", "track info is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue userRecommendations(const QUrlQuery & args) {
                    return userRecommendations(
                        args.queryItemValue(CMD_ID),
                        (bool)args.queryItemValue(CMD_PREDICATE).toInt(),
                        args.queryItemValue(CMD_OFFSET).toInt()
                    );
                }
                QJsonValue userRecommendations(const QString & user_id, bool randomize, int offset = 0) { // ~50 per request
                    Permissions perm = permissions(pr_recommendations);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site: {
                            Response * req_response = Manager::prepare() -> postFollowed(
                                IQueriable::baseUrlStr(
                                    qst_site, QStringLiteral("al_audio.php"),
                                    {
                                        { QStringLiteral("act"), QStringLiteral("a_load_section") },
                                        { QStringLiteral("al"), QStringLiteral("1") },
                                        { QStringLiteral("album_id"), QStringLiteral("-2") },
                                        { QStringLiteral("offset"), offset },
                                        { QStringLiteral("owner_id"), user_id },
                                        { QStringLiteral("type"), QStringLiteral("recoms") }
                                    }
                                ),
                                dntHeader()
                            );

                            QJsonArray items = RESPONSE_TO_JSON_OBJECT(req_response).value(QStringLiteral("list")).toArray();
                            prepareTracks(items, block_content);

                            QueriableResponse response(block_content, QString::number(offset + block_content.size()), 0, 1, block_content.isEmpty());
                            return prepareBlock(dmt_audio, cmd_mtd_user_recommendations, response, {}, {{CMD_ID, user_id}, {CMD_PREDICATE, (int)randomize}});
                        break;}

                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api, tkn_execute,
                                    {
                                        { tkn_code, QString(
                                               "var recomendations = API.audio.getRecommendations({"
                                               "user_id: \"" % user_id % "\", "
                                               "   count: 1000, "
                                               "   shuffle: " % boolToStr(randomize) % ""
                                               "});"
                                               "return {\"" % block_items_audio % "\": recomendations };"
                                          )
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, IQUERY_DEF_FIELDS << block_items_audio << tkn_items
                            );
                        break;}

                        default: Logger::obj().write("VK", "userRecommendations is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue trackRecommendations(const QUrlQuery & args) {
                    return trackRecommendations(
                        args.queryItemValue(CMD_ID),
                        (bool)args.queryItemValue(CMD_PREDICATE).toInt(),
                        args.queryItemValue(CMD_OFFSET).toInt()
                    );
                }
                QJsonValue trackRecommendations(const QString & track_id, bool randomize, int offset = 0) {
                    Permissions perm = permissions(pr_recommendations);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site: {
                            Response * req_response = Manager::prepare() -> postFollowed(
                                IQueriable::baseUrlStr(
                                    qst_site, QStringLiteral("al_audio.php"),
                                    {
                                        { QStringLiteral("act"), QStringLiteral("a_load_section") },
                                        { QStringLiteral("al"), QStringLiteral("1") },
                                        { QStringLiteral("album_id"), QString(QStringLiteral("audio") % track_id) },
                                        { QStringLiteral("offset"), offset },
                                        { QStringLiteral("owner_id"), userID(pr_recommendations) },
                                        { QStringLiteral("type"), QStringLiteral("recoms") }
                                    }
                                ),
                                dntHeader()
                            );

                            QJsonArray items = RESPONSE_TO_JSON_OBJECT(req_response).value(QStringLiteral("list")).toArray();
                            prepareTracks(items, block_content);

                            QueriableResponse response(block_content, QString::number(offset + block_content.size()), 0, 1, block_content.isEmpty());
                            return prepareBlock(dmt_audio, cmd_mtd_track_recommendations, response, {}, {{CMD_ID, track_id}, {CMD_PREDICATE, (int)randomize}});
                        break;}

                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api, tkn_execute,
                                    {
                                        { tkn_code, QString(
                                               "var recomendations = API.audio.getRecommendations({"
                                               "target_audio: \"" % track_id % "\", "
                                               "   count: 1000, "
                                               "   shuffle: " % boolToStr(randomize) % ""
                                               "});"
                                               "return {\"" % block_items_audio % "\": recomendations };"
                                          )
                                        }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract, IQUERY_DEF_FIELDS << block_items_audio << tkn_items
                            );
                        break;}

                        default: Logger::obj().write("VK", "trackRecommendations is not accessable", true);
                    }

//                    return prepareBlock(dmt_group, cmd_mtd_groups_by_id, response, {}, {{CMD_ID, track_id}, {CMD_PREDICATE, (int)randomize}});
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

//                    return baseUrlStr(qst_api, tkn_execute, query);
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
                                    qst_api, tkn_execute,
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
                                                "return {\"" % block_items_audio % "\": search};"
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
                                    qst_api, QStringLiteral("audio.get"),
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
