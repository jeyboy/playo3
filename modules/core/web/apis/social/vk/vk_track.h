#ifndef VK_TRACK
#define VK_TRACK

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Track : public Base {
            public:
                enum AudioSearchSort { ass_creation_date = 0, ass_duration = 1, ass_popularity = 2 };

                QString trackUrl(const QString & track_id) { return trackInfo(track_id).value(tkn_url).toString(); }

                QString trackLyric(const QString & lyrics_id) { //TODO: finish me // not tested
                    return sRequest(
                        baseUrlStr(
                            qst_api_def, path_lyrics,
                            { { tkn_lyrics_id, lyrics_id } }
                        ),
                        call_type_json, 0, proc_json_extract
                    );
                }

                QJsonValue trackInfo(const QString & track_id) {
                    QJsonArray info = audioInfo(QStringList() << track_id).toArray();
                    return info.isEmpty() ? QJsonObject() : info[0].toObject();
                }

                QJsonValue tracksInfo(const QStringList & track_ids) {
                    return saRequest(
                        baseUrlStr(
                            qst_api_def, tkn_execute,
                            {
                                {
                                    tkn_code,
                                    QStringLiteral("return API.audio.getById({ audios: \"") % track_ids.join(',') % QStringLiteral("\"});")
                                }
                            }
                        ),
                        call_type_json, 0, proc_json_extract
                    );
                }

                QJsonValue trackRecommendations(const QString & user_id, bool byOwn, bool randomize) {
                    return sRequest(
                        baseUrlStr(
                            qst_api_def, tkn_execute,
                            {
                                { tkn_code, QString(
                                       "var recomendations = API.audio.getRecommendations({"
                                       + QString(byOwn ? "user_id: " : "target_audio: ") % "\"" % user_id % "\", "
                                       "   count: 1000, "
                                       "   shuffle: " % boolToStr(randomize) % ""
                                       "});"
                                       "return {" % tkn_audio_list % ": recomendations };"
                                  )
                                }
                            }
                        ),
                        call_type_json, 0, proc_json_extract
                    );
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

                QJsonValue tracksSearch(const SearchLimit & limits, QJsonArray * arr = 0, bool autoFix = false) {
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
                                        "return {" % tkn_audio_list % ": search};"
                                    )
                                }
                            }
                        ),
                        call_type_json, arr, proc_json_extract,
                        IQUERY_DEF_FIELDS << tkn_audio_list
                    );
                }

                QJsonValue trackPopular(const QString & genre, bool & onlyEng) {
                    int genre_id = genres.toInt(genre);

                    return saRequest(
                        baseUrlStr(
                            qst_api_def, tkn_execute,
                            {
                                {
                                    tkn_code, QString(
                                       "var popular = API.audio.getPopular({"
                                            "only_eng: " + boolToStr(onlyEng) + ", "
                                            "count: 1000 " + (genre_id != -1 ? (", genre_id: " + QString::number(genre_id)) : "") + ""
                                         "});"
                                       "return {" % tkn_audio_list % ": popular};"
                                    )
                                }
                            }
                        ),
                        call_type_json, 0, proc_json_extract,
                        IQUERY_DEF_FIELDS << tkn_audio_list
                    );
                }
            };
        }
    }
}

#endif // VK_TRACK
