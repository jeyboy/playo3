#ifndef VK_AUDIO
#define VK_AUDIO

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Audio : public Base {
            public:
                enum AudioSearchSort { ass_creation_date = 0, ass_duration = 1, ass_popularity = 2 };

                QString audioUrl(const QString & track_id) {
                    QJsonObject obj = audioInfo(track_id).toObject();
                    return obj.value(tkn_url).toString();
                }

                QString audioLyric(const QString & lyrics_id) { //TODO: finish me // not tested
                    SourceFlags perm = permissions(sf_lyric_by_audio);

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            return sRequest(
                                baseUrlStr(
                                    qst_api, path_lyrics,
                                    { { tkn_lyrics_id, lyrics_id } }
                                ),
                                call_type_json, 0, proc_json_extract
                            ).value(QStringLiteral("hudo")).toString();
                        }

                        default: Logger::obj().write(name(), "track Lyric is not accessable", Logger::log_error);
                    }

                    return QString();
                }

                QJsonValue audioInfo(const QString & track_id) {
                    QJsonObject info = audioInfo(QStringList() << track_id).toObject();
                    QJsonArray content = info.value(tkn_content).toArray();
                    return content.isEmpty() ? QJsonObject() : content[0].toObject();
                }

                QJsonValue audioInfo(const QStringList & track_ids) {
                    SourceFlags perm = permissions(sf_audio_by_id);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site: {
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

                        case sf_api: {
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

                        default: Logger::obj().write(name(), "track info is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue userAudioRecommendations(const QUrlQuery & args) {
                    return userAudioRecommendations(
                        args.queryItemValue(CMD_ID),
                        (bool)args.queryItemValue(CMD_PREDICATE).toInt(),
                        args.queryItemValue(CMD_OFFSET).toInt()
                    );
                }
                QJsonValue userAudioRecommendations(const QString & user_id, bool randomize, int offset = 0) { // ~50 per request
                    SourceFlags perm = permissions(sf_audio_recs_by_user);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site: {
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
                            return prepareBlock(dmt_audio, cmd_mtd_user_audio_recommendations, response, {}, {{CMD_ID, user_id}, {CMD_PREDICATE, (int)randomize}});
                        break;}

                        case sf_api: {
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

                        default: Logger::obj().write(name(), "userAudioRecommendations is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue audioRecommendations(const QUrlQuery & args) {
                    return audioRecommendations(
                        args.queryItemValue(CMD_ID),
                        (bool)args.queryItemValue(CMD_PREDICATE).toInt(),
                        args.queryItemValue(CMD_OFFSET).toInt()
                    );
                }
                QJsonValue audioRecommendations(const QString & track_id, bool randomize, int offset = 0) {
                    SourceFlags perm = permissions(sf_audio_recs_by_user);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site: {
                            Response * req_response = Manager::prepare() -> postFollowed(
                                IQueriable::baseUrlStr(
                                    qst_site, QStringLiteral("al_audio.php"),
                                    {
                                        { QStringLiteral("act"), QStringLiteral("a_load_section") },
                                        { QStringLiteral("al"), QStringLiteral("1") },
                                        { QStringLiteral("album_id"), QString(QStringLiteral("audio") % track_id) },
                                        { QStringLiteral("offset"), offset },
                                        { QStringLiteral("owner_id"), userID(sf_audio_recs_by_user) },
                                        { QStringLiteral("type"), QStringLiteral("recoms") }
                                    }
                                ),
                                dntHeader()
                            );

                            QJsonArray items = RESPONSE_TO_JSON_OBJECT(req_response).value(QStringLiteral("list")).toArray();
                            prepareTracks(items, block_content);

                            QueriableResponse response(block_content, QString::number(offset + block_content.size()), 0, 1, block_content.isEmpty());
                            return prepareBlock(dmt_audio, cmd_mtd_audio_recommendations, response, {}, {{CMD_ID, track_id}, {CMD_PREDICATE, (int)randomize}});
                        break;}

                        case sf_api: {
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

                        default: Logger::obj().write(name(), "audioRecommendations is not accessable", Logger::log_error);
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

                QJsonValue audioSearch(const QUrlQuery & args) { return audioSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue audioSearch(const SearchLimit & limits, QJsonArray * arr = 0, bool autoFix = false) {
                    SourceFlags perm = permissions(sf_audio_by_title);
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

                        default: Logger::obj().write(name(), "audioSearch is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue audioByPlaylist(const QUrlQuery & args) {
                    return audioByPlaylist(
                        args.queryItemValue(CMD_ID)/*,
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()*/
                    );
                }
                QJsonValue audioByPlaylist(const QString & playlist_id) { // not finished
                    SourceFlags perm = permissions(sf_audio_by_playlist);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
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

                        default: Logger::obj().write(name(), "audioByPlaylist is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }
            };
        }
    }
}

#endif // VK_AUDIO
