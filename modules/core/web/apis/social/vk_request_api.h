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









            public:
                enum InfoType {
                    info_music = 1,
                    info_rels = 2,
                    info_all = info_music | info_rels,
                };

                QString userFullInfoUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_user_info.arg(uid));
                    return baseUrlStr(qst_api_def, tkn_execute, query);
                }
                QString userAudiosUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_user_audios.arg(uid));
                    return baseUrlStr(qst_api_def, tkn_execute, query);
                }
                QString userRelsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_user_rels.arg(uid));
                    return baseUrlStr(qst_api_def, tkn_execute, query);
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







                QString audioSearchLimitedUrl(QString & searchStr, int limit) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code, QString(
                        "    var items = API.audio.search({"
                        "        q: \"" % encodeStr(searchStr) % QStringLiteral("\", count: ") % QString::number(limit) % ", lyrics: 0"
                        "    }).items;"
                        "return {" % tkn_audio_list % ": search};"
                    ));

                    return baseUrlStr(qst_api_def, tkn_execute, query);
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

                QString audioRefreshUrl(const QStringList & uids) {
                    QUrlQuery query = genDefaultParams();

                    setParam(query, tkn_code,
                       QStringLiteral("return API.audio.getById({ audios: \"") % uids.join(',') % QStringLiteral("\"});")
                    );

                    return baseUrlStr(qst_api_def, tkn_execute, query);
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




                QJsonArray groupsByIdOrPermas(const QStringList & ids) { return groupsByIdOrPerma(ids.join(QStringLiteral(","))); }

                QJsonArray groupsByIdOrPerma(const QString & id) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_group_ids, id);

                    return saRequest(
                        baseUrlStr(qst_api_def, path_groups_by_id, query),
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
                        baseUrlStr(qst_api_def, path_groups_search, query),
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
                        baseUrlStr(qst_api_def, path_user_info, query),
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
                        baseUrlStr(qst_api_def, path_resole_user, query),
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
                        baseUrlStr(qst_api_def, path_users_search, query),
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
                        baseUrlStr(qst_api_def, QStringLiteral("newsfeed.get"), query),
                        call_type_json,
                        0,
                        proc_json_extract,
                        QStringList() << tkn_response << tkn_items
                    );
                }

                QString videoAlbumsUrl(QString & uid) {
                    QUrlQuery query = genDefaultParams();
                    setParam(query, tkn_code, query_audio_albums.arg(uid));
                    return baseUrlStr(qst_api_def, tkn_execute, query).replace(VK_DEFAULT_OFFSET_TEMPLATE, OFFSET_TEMPLATE);
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
