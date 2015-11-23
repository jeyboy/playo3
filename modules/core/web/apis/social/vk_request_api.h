#ifndef VK_REQUEST_API
#define VK_REQUEST_API

//https://vk.com/dev/execute

#include "modules/core/web/interfaces/iapi.h"
#include "vk_api_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class RequestApi : public IApi {
            protected:
                inline QString boolToStr(bool val) { return val ? bool_str_true : bool_str_false; }

                QString authUrl() {
                    QUrl url(auth_url);

                    QUrlQuery query = QUrlQuery();
                    setParam(query, version_key, version_val);
                    setParam(query, display_token, page_token);
                    setParam(query, client_id_token, client_token);
                    setParam(query, response_type_token, response_token);
                    setParam(query, scope_token, scope_val);
                    setParam(query, redirect_uri_token, redirect_uri_val);

                    url.setQuery(query);
                    return url.toString();
                }

                QJsonArray search_postprocess(QString & predicate, QString & genre, const SearchLimit & limitations) {
                    if (predicate.isEmpty() && limitations.by_popularity())
                        return audioPopular(true, genre);
                    else
                        return audioSearch(predicate, limitations.by_artists(), limitations.by_owns(), limitations.by_popularity(), limitations.count);
                }
            public:
            //    QUrl wallUrl(QString & uid) {
            //        QUrlQuery query = genDefaultParams();

            //        setParam(query, code_key, QString(
            //            "var limit = 100; var offset = _1_; var finished = false; "
            //            "var response = []; var look_window = limit * " + api_call_limit_val + " %2b offset; var post_items = [];"

            //            "while (offset < look_window && !finished) {"
            //            "   var items = API.wall.get({ count: limit, offset: offset, owner_id: " + uid + "}).items;"
            //            "   finished = items.length < limit;"
            //            "   post_items.push(items);"
            //            "   offset = offset %2b limit;"
            //            "}"

            //            "while(post_items.length > 0) {"
            //            "   var curr = post_items.pop();"
            //            "   var audios = curr.attachments@.audio %2b"
            //            "       curr.copy_history[0].attachments@.audio %2b"
            //            "       curr.copy_history[1].attachments@.audio %2b"
            //            "       curr.copy_history[2].attachments@.audio %2b"
            //            "       curr.copy_history[3].attachments@.audio;"
            //            "   if (audios.length > 0) {"
            //            "       response.unshift({ title: curr.text, date: curr.date, audios: audios });"
            //            "   }"
            //            "}"

            //            "return { offset: offset, posts: response, " % finished_key % ": finished };"
            //        ));

            //        return baseUrl(execute_key, query);
            //    }
            //    QJsonArray wallAudio(QString & uid) {
            //        return lQuery(wallUrl(uid), DEFAULT_LIMIT_AMOUNT, "posts");
            //    }

                QUrl audioAlbumsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key,
                        QString(
                           "var curr; var count = 5;"
                           "var folders_result = API.audio.getAlbums({"
                           "                count: count, "
                           "                offset: " % predef1_key % ", "
                           "                owner_id: " % uid % ""
                           "    });"
                           "var folders_result = folders_result.items;"
                           "var proceed_folders = {};"
                           "while(folders_result.length > 0) {"
                           "    curr = folders_result.pop();"
                           "    proceed_folders.push({"
                           "        folder_id: curr.id,"
                           "        title: curr.title,"
                           "        items: API.audio.get({"
                           "            owner_id: " % uid % ","
                           "            album_id: curr.id"
                           "        }).items "
                           "    });"
                           "};"
                           "return { "
                           "    " % albums_key % ": proceed_folders, "
                           "    " % finished_key % ": (proceed_folders.length < count), "
                           "    " % offset_key % ": " % predef1_key % " %2b count"
                           "};"
                       )
                    );

                    return baseUrl(execute_key, query);
                }
                QJsonArray audioAlbums(QString & uid, int offset = 0) {
                    return lQuery(
                        audioAlbumsUrl(uid),
                        QueryRules(albums_key, 5, DEFAULT_LIMIT_AMOUNT, offset),
                        extract
                    );
                }
                void audioAlbums(QString & uid, QJsonArray & arr, int offset = 0) {
                    lQuery(
                        audioAlbumsUrl(uid),
                        QueryRules(albums_key, 5, DEFAULT_LIMIT_AMOUNT, offset),
                        arr, extract
                    );
                }


                QUrl userFullInfoUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key,
                       QString(
                           "var curr; var proceed_groups = [];"
                           "var groups = API.groups.get({"
                           "            owner_id: " % uid % ", "
                           "            count: 1000, "
                           "            extended: 1"
                           "    }).items;"
                           "while(groups.length > 0) {"
                           "    curr = groups.pop();"
                           "    proceed_groups.push({"
                           "        id: curr.id, "
                           "        title: curr.name"
                           "    });"
                           "};"

                           "var friends = API.friends.get({"
                           "            user_id: " % uid % ", "
                           "            order: \"name\", "
                           "            fields: \"nickname\""
                           "    });"
                           "var proceed_friends = [];"
                           "if (friends.count > 0) { "
                           "    while(friends.items.length > 0) { "
                           "        curr = friends.items.pop();"
                           "        proceed_friends.push({ "
                           "            id: curr.id, "
                           "            title: curr.first_name %2b \" \" %2b curr.last_name"
                           "        }); "
                           "    }; "
                           "};"

                           "var folders_result = API.audio.getAlbums({"
                           "            count: " % api_call_limit_val % ", "
                           "            owner_id: " % uid % ""
                           "    });"
                           "var folders_count = folders_result.count;"
                           "var proceed_folders = {};"
                           "if (folders_count > 0) { "
                           "    while(folders_result.items.length > 0) { "
                           "        curr = folders_result.items.pop();"
                           "        proceed_folders.push({"
                           "            folder_id: curr.id, "
                           "            title: curr.title, "
                           "            items: API.audio.get({ "
                           "                album_id: curr.id "
                           "            }).items "
                           "        });"
                           "    };"
                           "};"
                           "return {"
                           "    " % audio_list_key % ": API.audio.get({ "
                           "        count: 6000, owner_id: " % uid % ""
                           "    }),"
                           "    " % albums_key % ": [proceed_folders], "
                           "    " % groups_key % ": proceed_groups, "
                           "    " % friends_key % ": proceed_friends, "
                           "    " % albums_offset_key % ": " % api_call_limit_val % ", "
                           "    " % albums_finished_key % ": (folders_count < " % api_call_limit_val % "), "
                           "};"
                       )
                   );

                   return baseUrl(execute_key, query);
                }
                QUrl userShortInfoUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key,
                       QString(
                           "var folders_result = API.audio.getAlbums({ "
                           "            count: " % api_call_limit_val % ", "
                           "            owner_id: " % uid % ""
                           "        });"
                           "var folders_count = folders_result.count;"
                           "var sort_by_folders = {};"
                           "if (folders_count > 0) { "
                           "    while(folders_result.items.length > 0) { "
                           "        var curr = folders_result.items.pop(); "
                           "        sort_by_folders.push({"
                           "            folder_id: curr.id, "
                           "            title: curr.title, "
                           "            items: API.audio.get({ "
                           "                owner_id: " % uid % ", "
                           "                album_id: curr.id"
                           "            }).items"
                           "        });"
                           "    };"
                           "};"
                           "return {"
                           "    " % audio_list_key % ": API.audio.get({ "
                           "        count: 6000, "
                           "        owner_id: " % uid % ""
                           "    }), "
                           "    " % albums_key % ": [sort_by_folders], "
                           "    " % albums_offset_key % ": " % api_call_limit_val % ", "
                           "    " % albums_finished_key % ": (folders_count < " % api_call_limit_val % ")"
                           "};"
                       )
                    );

                    return baseUrl(execute_key, query);
                }


                QJsonObject userInfo(QString & uid, bool fullInfo = true) {
                    QUrl url = fullInfo ? userFullInfoUrl(uid) : userShortInfoUrl(uid);
                    QJsonObject ret = sQuery(url, extract);

                    QJsonArray ar = ret.value(albums_key).toArray();

                    if (!ret.value(albums_finished_key).toBool())
                        audioAlbums(uid, ar, ret.value(albums_offset_key).toInt());

                    ret.insert(albums_key, ar);
                    return ret;
                }

                QJsonObject userStatus(QString & uid) { // deactivated: 'deleted', id, first_name, last_name, counters
                    QUrlQuery query;
                    setParam(query, fields_key, counters_key);
                    setParam(query, user_ids_key, uid);

                    return sQuery(baseUrl(user_info_path, query)).value(response_key).toArray().first().toObject();
                }

                QUrl audioRecomendationsUrl(QString & uid, bool byUser, bool randomize) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key,
                       QString(
                           "var recomendations = API.audio.getRecommendations({"
                           + QString(byUser ? "user_id: " : "target_audio: ") % "\"" % uid % "\", "
                           "   count: 1000, "
                           "   shuffle: " % boolToStr(randomize) % ""
                           "});"
                           "return {" % audio_list_key % ": recomendations };"
                       )
                    );

                    return baseUrl(execute_key, query);
                }

                QJsonObject audioRecomendations(QString & uid, bool byUser, bool randomize) {
                    return sQuery(audioRecomendationsUrl(uid, byUser, randomize), extract);
                }


                enum SearchSort { creation_date = 0, duration = 1, popularity = 2 };

                QUrl audioSearchUrl(QString & searchStr, bool autoFix, bool artistOnly, bool searchByOwn, SearchSort sort, int limit = 1000) {
                    // count max eq 300 , limit is 1000
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key, QString(
                        "var limit = " % QString::number(limit) % ";"
                        "var search = []; var rule = true;"
                        "do {"
                        "    var count = limit - search.length;"
                        "    if (count > 300) count = 300;"
                        "    var items = API.audio.search({"
                        "        q: \"" % QUrl::toPercentEncoding(searchStr) % "\", count: count, offset: search.length, lyrics: 0,"
                        "        auto_complete: " % boolToStr(autoFix) % ","
                        "        performer_only: " % boolToStr(artistOnly) % ","
                        "        sort: " % QString::number(sort) % ","
                        "        search_own: " % boolToStr(searchByOwn) % ""
                        "    }).items;"
                        "    search = search %2b items;"
                        "    rule = search.length < limit && items.length != 0;"
                        "} while(rule);"
                        "return {" % audio_list_key % ": search};"
                    ));

                    return baseUrl(execute_key, query);
                }
                QJsonArray audioSearch(QString & predicate, bool onlyArtist, bool inOwn, bool mostPopular, int limit) {
                    return sQuery(
                        audioSearchUrl(predicate, false, onlyArtist, inOwn, mostPopular ? popularity : creation_date, qMin(1000, limit)), extract
                    ).value(audio_list_key).toArray();
                }

                QUrl audioSearchLimitedUrl(QString & searchStr, int limit) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key, QString(
                        "    var items = API.audio.search({"
                        "        q: \"" % QUrl::toPercentEncoding(searchStr) % QStringLiteral("\", count: ") % QString::number(limit) % ", lyrics: 0"
                        "    }).items;"
                        "return {" % audio_list_key % ": search};"
                    ));

                    return baseUrl(execute_key, query);
                }

                QJsonObject audioSearchLimited(QString & predicate, int limitation) {
                    return sQuery(audioSearchLimitedUrl(predicate, limitation), extract);
                }

                QUrl audioPopularUrl(bool onlyEng, int genreId) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key,
                       QString(
                           "var recomendations = API.audio.getPopular({"
                                "only_eng: " + boolToStr(onlyEng) + ", "
                                "count: 1000 " + (genreId != -1 ? (", genre_id: " + QString::number(genreId)) : "") + ""
                             "});"
                           "return {" % audio_list_key % ": recomendations};"
                       )
                    );

                    return baseUrl(execute_key, query);
                }
                QJsonArray audioPopular(bool onlyEng, const QString genre) {
                    return sQuery(audioPopularUrl(onlyEng, genres.toInt(genre)), extract).value(QStringLiteral("audio_list")).toArray();
                }

                QUrl audioRefreshUrl(QStringList & uids) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, code_key,
                       QStringLiteral("return API.audio.getById({ audios: \"") % uids.join(',') % QStringLiteral("\"});")
                    );

                    return baseUrl(execute_key, query);
                }
                QJsonArray getAudiosInfo(QStringList & audio_uids) {
                    return sQuery(audioRefreshUrl(audio_uids)).value(response_key).toArray();
                }
                QJsonObject getAudioInfo(const QString & audio_uid) {
                    QStringList uids; uids << audio_uid;
                    QJsonArray infos = getAudiosInfo(uids);
                    if (infos.isEmpty())
                        return QJsonObject();
                    else return infos[0].toObject();
                }

                QString refresh(const QString & audio_uid) {
                    return getAudioInfo(audio_uid).value(url_key).toString();
                }

                void nameToId(QString name, QString & id, QString & id_type) {
                    QUrlQuery query;
                    setParam(query, screen_name_key, name);
                    QJsonObject ret = sQuery(baseUrl(resole_user_path, query)).value(response_key).toObject();
                    id = QString::number(ret.value(object_id_key).toInt());
                    id_type = ret.value(type_key).toString();
                }

                QUrl audioLyricsUrl(QString & lyrics_id) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, lyrics_id_key, lyrics_id);
                    return baseUrl(lyrics_path, query);
                }
            };
        }
    }
}

#endif // VK_REQUEST_API
