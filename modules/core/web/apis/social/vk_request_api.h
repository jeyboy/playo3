#ifndef VK_REQUEST_API
#define VK_REQUEST_API

//https://vk.com/dev/execute

#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/igenreable.h"
#include "vk_api_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class RequestApi : public IQueriable, public IGenreable {
            protected:
                enum AudioSearchSort { ass_creation_date = 0, ass_duration = 1, ass_popularity = 2 };
                enum VideoSearchSort { vss_creation_date = 0, vss_duration = 1, vss_relativity = 2 };

                inline QString boolToStr(bool val) { return val ? val_str_true : val_str_false; }

                QString authUrl() {
                    QUrl url(url_auth);

                    QUrlQuery query = QUrlQuery();
                    setParam(query, tkn_version, val_version);
                    setParam(query, tkn_display, val_page_token);
                    setParam(query, tkn_client_id, val_client_token);
                    setParam(query, tkn_response_type, val_response_token);
                    setParam(query, tkn_scope, val_scope);
                    setParam(query, tkn_redirect_uri, url_redirect_uri);

                    url.setQuery(query);
                    return url.toString();
                }

                PolyQueryRules rules(
                    int offset = 0, int items_limit = DEFAULT_ITEMS_LIMIT, int pages_limit = DEFAULT_REQUESTS_LIMIT,
                    int /*per_request*/ = 100,
                    ApiCallIterType call_type = call_iter_type_item)
                {
                    return PolyQueryRules(
                        call_type,
                        call_iter_method_offset,
                        qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_limit, DEFAULT_REQUESTS_LIMIT),
                        QString(), // limit already inserted to scripts
                        0, // per_request
                        QString(), // template already inserted to scripts
                        offset
                    );
                }

            public:
                enum InfoType {
                    info_music = 1,
                    info_rels = 2,
                    info_all = info_music | info_rels,
                };

                QString audioAlbumsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_audio_albums.arg(uid));
                    return baseUrlStr(tkn_execute, query).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE);
                }
                QJsonArray audioAlbums(QString & uid, int offset = 0) {
                    return pRequest(
                        audioAlbumsUrl(uid),
                        call_type_json,
                        rules(offset),
                        0,
                        proc_json_extract,
                        QStringList() << tkn_albums
                    );

//                    return lQuery(
//                        audioAlbumsUrl(uid),
//                        QueryRules(tkn_albums, 5, DEFAULT_ITEMS_LIMIT, offset),
//                        extract
//                    );
                }
                void audioAlbums(QString & uid, QJsonArray & arr, int offset = 0) {
                    pRequest(
                        audioAlbumsUrl(uid),
                        call_type_json,
                        rules(offset),
                        &arr,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_albums,
                        call_method_get,
                        Headers(),
                        0,
                        false
                    );

//                    lQuery(
//                        audioAlbumsUrl(uid),
//                        QueryRules(tkn_albums, 5, DEFAULT_ITEMS_LIMIT, offset),
//                        arr, extract
//                    );
                }

                QString userFullInfoUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_user_info.arg(uid));
                    return baseUrlStr(tkn_execute, query);
                }
                QString userAudiosUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_user_audios.arg(uid));
                    return baseUrlStr(tkn_execute, query);
                }
                QString userRelsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_user_rels.arg(uid));
                    return baseUrlStr(tkn_execute, query);
                }

                QJsonObject userRels(QString & uid) {
                    return sRequest(
                        userRelsUrl(uid),
                        call_type_json,
                        0,
                        proc_json_extract
                    );


//                    return sQuery(userRelsUrl(uid), extract);
                }

                QJsonObject userInfo(QString & uid, InfoType fullInfo = info_all) {
                    QString url = fullInfo == info_all ? userFullInfoUrl(uid) : userAudiosUrl(uid);

                    QJsonObject ret = sRequest(
                        url,
                        call_type_json,
                        0,
                        proc_json_extract
                    );


//                    QJsonObject ret = sQuery(url, extract);

                    QJsonArray ar = ret.value(tkn_albums).toArray();

                    if (!ret.value(tkn_albums_finished).toBool())
                        audioAlbums(uid, ar, ret.value(tkn_albums_offset).toInt());

                    ret.insert(tkn_albums, ar);
                    return ret;
                }

                QJsonObject userStatus(QString & uid) { // deactivated: 'deleted', id, first_name, last_name, counters
                    QUrlQuery query;
                    setParam(query, tkn_fields, tkn_counters);
                    setParam(query, tkn_user_ids, uid);

                    QJsonObject ret = sRequest(
                        baseUrlStr(path_user_info, query),
                        call_type_json
                    );


                    return ret/*sQuery(baseUrl(path_user_info, query))*/.value(tkn_response).toArray().first().toObject();
                }

                QString audioRecomendationsUrl(QString & uid, bool byUser, bool randomize) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code,
                       QString(
                           "var recomendations = API.audio.getRecommendations({"
                           + QString(byUser ? "user_id: " : "target_audio: ") % "\"" % uid % "\", "
                           "   count: 1000, "
                           "   shuffle: " % boolToStr(randomize) % ""
                           "});"
                           "return {" % tkn_audio_list % ": recomendations };"
                       )
                    );

                    return baseUrlStr(tkn_execute, query);
                }

                QJsonObject audioRecomendations(QString & uid, bool byUser, bool randomize) {
                    return sRequest(
                        audioRecomendationsUrl(uid, byUser, randomize),
                        call_type_json,
                        0,
                        proc_json_extract
                    );

//                    return sQuery(audioRecomendationsUrl(uid, byUser, randomize), extract);
                }

                QString audioSearchUrl(const SearchLimit & limits, bool autoFix = false) {
                    // count max eq 300 , limit is 1000
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code, QString(
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
                    ));

                    return baseUrlStr(tkn_execute, query);
                }


                QJsonArray audioSearch(const SearchLimit & limits, QJsonArray * arr = 0) {
                    return saRequest(
                        audioSearchUrl(limits),
                        call_type_json,
                        arr,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_audio_list
                    ); //.value(tkn_audio_list).toArray();

//                    return sQuery(
//                        audioSearchUrl(predicate, false, onlyArtist, inOwn, mostPopular ? popularity : creation_date, qMin(1000, limit)), extract
//                    ).value(tkn_audio_list).toArray();
                }

                QString audioSearchLimitedUrl(QString & searchStr, int limit) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code, QString(
                        "    var items = API.audio.search({"
                        "        q: \"" % encodeStr(searchStr) % QStringLiteral("\", count: ") % QString::number(limit) % ", lyrics: 0"
                        "    }).items;"
                        "return {" % tkn_audio_list % ": search};"
                    ));

                    return baseUrlStr(tkn_execute, query);
                }

                QJsonObject audioSearchLimited(QString & predicate, int limitation) {
                    return sRequest(
                        audioSearchLimitedUrl(predicate, limitation),
                        call_type_json,
                        0,
                        proc_json_extract
                    );

//                    return sQuery(audioSearchLimitedUrl(predicate, limitation), extract);
                }

                QString audioPopularUrl(bool onlyEng, int genreId) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code,
                       QString(
                           "var popular = API.audio.getPopular({"
                                "only_eng: " + boolToStr(onlyEng) + ", "
                                "count: 1000 " + (genreId != -1 ? (", genre_id: " + QString::number(genreId)) : "") + ""
                             "});"
                           "return {" % tkn_audio_list % ": popular};"
                       )
                    );

                    return baseUrlStr(tkn_execute, query);
                }
                QJsonArray audioPopular(bool onlyEng, const QString & genre) {
                    return saRequest(
                        audioPopularUrl(onlyEng, genres.toInt(genre)),
                        call_type_json,
                        0,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_audio_list
                    ); //.value(QStringLiteral("audio_list")).toArray();

//                    return sQuery(audioPopularUrl(onlyEng, genres.toInt(genre)), extract).value(QStringLiteral("audio_list")).toArray();
                }

                QString audioRefreshUrl(const QStringList & uids) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code,
                       QStringLiteral("return API.audio.getById({ audios: \"") % uids.join(',') % QStringLiteral("\"});")
                    );

                    return baseUrlStr(tkn_execute, query);
                }
                QJsonArray audioInfo(const QStringList & audio_uids) {
                    return saRequest(
                        audioRefreshUrl(audio_uids),
                        call_type_json,
                        0,
                        proc_json_extract
                    );//.value(tkn_response).toArray();

//                    return sQuery(audioRefreshUrl(audio_uids)).value(tkn_response).toArray();
                }
                QJsonObject audioInfo(const QString & audio_uid) {
                    QStringList uids; uids << audio_uid;
                    QJsonArray infos = audioInfo(uids);
                    if (infos.isEmpty())
                        return QJsonObject();
                    else return infos[0].toObject();
                }

                QString audioLyricsUrl(QString & lyrics_id) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_lyrics_id, lyrics_id);
                    return baseUrlStr(path_lyrics, query);
                }


                QJsonArray groupsByIdOrPermas(const QStringList & ids) { return groupsByIdOrPerma(ids.join(QStringLiteral(","))); }

                QJsonArray groupsByIdOrPerma(const QString & id) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_group_ids, id);

                    return saRequest(
                        baseUrlStr(path_groups_by_id, query),
                        call_type_json,
                        0,
                        proc_json_extract
                    ); //.value(tkn_response).toArray();

//                    return sQuery(baseUrl(path_groups_by_id, query)).value(tkn_response).toArray();
                }

                QJsonArray groupsByName(const QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_type, val_group_types);
                    setParam(query, tkn_q, name);
                    setParam(query, QStringLiteral("count"), 100/*0*/);

                    return saRequest(
                        baseUrlStr(path_groups_search, query),
                        call_type_json,
                        0,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_items
                    );

//                    return sQuery(baseUrl(path_groups_search, query)).value(tkn_response).toArray();
                }

                QJsonArray usersByIdsOrPermas(const QStringList & ids) { return usersByIdOrPerma(ids.join(QStringLiteral(","))); }

                QJsonArray usersByIdOrPerma(const QString & id) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_user_ids, id);
                    setParam(query, tkn_fields, val_user_fields);

                    return saRequest(
                        baseUrlStr(path_user_info, query),
                        call_type_json,
                        0,
                        proc_json_extract
                    );

//                    return sQuery(baseUrl(path_user_info, query)).value(tkn_response).toArray();
                }

                void permaToId(const QString & name, QString & id, QString & id_type) {
                    QUrlQuery query;// = genDefaultParams();
                    setParam(query, tkn_screen_name, name);

                    QJsonObject ret = sRequest(
                        baseUrlStr(path_resole_user, query),
                        call_type_json,
                        0,
                        proc_json_extract
                    );

//                    QJsonObject ret = sQuery(baseUrl(path_resole_user, query)).value(tkn_response).toObject();

                    id = QString::number(ret.value(tkn_object_id).toInt());
                    id_type = ret.value(tkn_type).toString();
                }

                QJsonArray usersByName(const QString & name) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_q, name);
                    setParam(query, tkn_fields, val_user_fields);

                    return saRequest(
                        baseUrlStr(path_users_search, query),
                        call_type_json,
                        0,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_items
                    );

//                    QJsonObject ret = sQuery(baseUrl(path_users_search, query)).value(tkn_response).toObject();
//                    return ret.value(tkn_items).toArray();
                }

                // https://new.vk.com/dev/newsfeed.getRecommended
                // not finished
                QJsonArray usersFeeds(const QStringList & sources = QStringList()) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, QStringLiteral("filters"), QStringLiteral("audio,video"));
                    setParam(query, QStringLiteral("count"), 100);

                    if (!sources.isEmpty())
                        setParam(query, QStringLiteral("source_ids"), sources);

                    return saRequest(
                        baseUrlStr(QStringLiteral("newsfeed.get"), query),
                        call_type_json,
                        0,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_items
                    );
                }

                QString videoAlbumsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_audio_albums.arg(uid));
                    return baseUrlStr(tkn_execute, query).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE);
                }
                QJsonArray videoAlbums(QString & uid, int offset = 0) {
                    return pRequest(
                        videoAlbumsUrl(uid),
                        call_type_json,
                        rules(offset),
                        0,
                        proc_json_extract,
                        QStringList() << tkn_albums
                    );

//                    return lQuery(
//                        audioAlbumsUrl(uid),
//                        QueryRules(tkn_albums, 5, DEFAULT_ITEMS_LIMIT, offset),
//                        extract
//                    );
                }

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

                    return baseUrlStr(tkn_execute, query);
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

                QJsonArray videoPopular(const SearchLimit & /*limits*/, QJsonArray * /*arr*/ = 0) { // write me
                    return QJsonArray();
//                    return saRequest(
//                        audioPopularUrl(onlyEng, genres.toInt(genre)),
//                        call_type_json,
//                        0,
//                        proc_json_extract,
//                        QStringList() << tkn_response << tkn_audio_list
//                    ); //.value(QStringLiteral("audio_list")).toArray();
                }
            };
        }
    }
}

#endif // VK_REQUEST_API
