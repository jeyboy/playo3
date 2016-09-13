#ifndef SOUNDCLOUD_TRACK
#define SOUNDCLOUD_TRACK

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Track : public Base {
            public:
                // add station and track to listened
                // curl 'https://api-v2.soundcloud.com/me/play-history?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1472842846' -X POST -H 'Accept: application/json, text/javascript, */*; q=0.1' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'Content-Length: 94' -H 'Content-Type: application/json' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'origin: https://soundcloud.com'

                // like track
                // curl 'https://api-v2.soundcloud.com/me/play-history?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1472842846' -X POST -H 'Accept: application/json, text/javascript, */*; q=0.1' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'Content-Length: 94' -H 'Content-Type: application/json' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'origin: https://soundcloud.com'

                // like station
                // curl 'https://api-v2.soundcloud.com/me/play-history?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1472842846' -X POST -H 'Accept: application/json, text/javascript, */*; q=0.1' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'Content-Length: 94' -H 'Content-Type: application/json' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'origin: https://soundcloud.com'

                QString trackUrl(const QString & track_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            QJsonObject obj = trackInfo(track_id);
                            QString url = obj.value(QStringLiteral("stream_url")).toString();
                            if (!url.isEmpty()) return url;
                        break;}

                        case perm_site: {
                            QJsonObject obj = sRequest(
                                baseUrlStr(qst_site, QStringLiteral("i1/tracks/%1/streams").arg(track_id), {}),
                                call_type_json, 0, proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );

                            QString res = obj.value(QStringLiteral("http_mp3_128_url")).toString();
                            return res;
                        break;}

                        default: Logger::obj().write(name(), "TRACK URL is not accessable", true);
                    }

                    return QString();
                }
                bool likeTrack(const QString & user_id, const QString & track_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            QString res = Manager::prepare() -> putFollowed(
                                baseUrlStr(qst_api, path_user_favorites.arg(user_id) % '/' % track_id, {}),
                                Headers()
                            ) -> toText();
                            qDebug() << "FAV" << res;
                            return false; // TODO: update me
                        break;}

                        case perm_site: {
                            // alt // https://api.soundcloud.com/e1/me/track_likes/265476609
                            QJsonObject obj = Manager::prepare() -> putFollowed(
                                baseUrlStr(qst_site, path_user_favorites.arg(user_id) % '/' % track_id, {}),
                                headers()
                            ) -> toJson();

                            return obj.value(QStringLiteral("status")).toString().startsWith(QStringLiteral("201"));
                        break;}

                        default: Logger::obj().write(name(), "LIKE TRACK is not accessable", true);
                    }

                    return false;
                }

                QJsonObject trackInfo(const QString & track_id) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            return sRequest(
                                baseUrlStr(qst_api, path_tracks % '/' % track_id, {}),
                                call_type_json
                            );
                        break;}

                        case perm_site: {
                            return sRequest(
                                baseUrlStr(qst_site_alt1, path_tracks, {{ tkn_ids, track_id }}),
                                call_type_json, 0, proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK INFO is not accessable", true);
                    }

                    return QJsonObject();
                }

                //"id": 142370360,
                //"permalink": "sam-smith-stay-with-me"
                QJsonValue tracksInfo(const QStringList & track_ids) {
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray response;
                    QJsonObject obj;

                    int limiter = track_ids.size() / SOUNDCLOUD_IDS_PER_REQUEST + (int)(track_ids.size() % SOUNDCLOUD_IDS_PER_REQUEST != 0);
                    for(int step = 0; step < limiter; step++) {
                        std::initializer_list<std::pair<QString, QVariant> > params = {{ tkn_ids, (QStringList)track_ids.mid(step * SOUNDCLOUD_IDS_PER_REQUEST, SOUNDCLOUD_IDS_PER_REQUEST) }};

                        switch(perm) {
                            case perm_api: {
                                obj = sRequest(
                                    baseUrlStr(qst_api, path_tracks, params),
                                    call_type_json, 0, proc_json_wrap
                                );
                            break;}

                            case perm_site: {
                                obj = sRequest(
                                    baseUrlStr(qst_site_alt1, path_tracks, params),
                                    call_type_json, 0, proc_json_wrap, IQUERY_DEF_FIELDS, call_method_get, headers()
                                );
                            break;}

                            default: { Logger::obj().write(name(), "TRACKS INFO is not accessable", true); break;}
                        }

                        QueriableArg::arrAppend(response, obj.value(DEF_JSON_FIELD).toArray());
                    }

                    return prepareBlock(dmt_audio, response);
                }

                QJsonValue trackRecommendations(const QUrlQuery & args) {
                    return trackRecommendations(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue trackRecommendations(const QString & track_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_related_tracks.arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site_alt1, path_related_tracks.arg(track_id), {}),
                                call_type_json, rules(offset, count), 0,
                                proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK RELATIONS is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_track_recommendations, response, {}, {{CMD_ID, track_id}});
                }

                QJsonValue tracksSearch(const QUrlQuery & args) { return tracksSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue tracksSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_api, path_tracks,
                                    trackSearchQuery(limits.predicate, limits.genre, limits.by_popularity())
                                ),
                                call_type_json,
                                rules(limits.start_offset, limits.items_limit, limits.requests_limit), 0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1,
                                    QStringLiteral("search/tracks"),
                                    {
                                        { tkn_q, limits.predicate },
                                        { QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base) },
                                        { QStringLiteral("sc_a_id"), generateMark() },
                                        { QStringLiteral("facet"), QStringLiteral("genre") }
                                    }
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit, limits.requests_limit), 0,
                                proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK SEARCH is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_search, response, limits);
                }

                QJsonValue tracksByTag(const QUrlQuery & args) {
                    return tracksByTag(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByTag(const QString & tag, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: { // support a comma separated list of tags
                            response = pRequest(
                                baseUrlStr(qst_api, path_tracks, {{ QStringLiteral("tags"), tag }}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site, QStringLiteral("search/sounds"),
                                    {
                                        { tkn_q, QStringLiteral("*") },
                                        { QStringLiteral("filter.genre"), tag } // its a genius !!! // try to use a comma separated list of tags
                                    }
                                ),
                                call_type_json, rules(offset, count), 0,
                                proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK BY TAG is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_tag, response, {}, {{CMD_PREDICATE, tag}});
                }

                QJsonValue tracksByGroup(const QUrlQuery & args) {
                    return tracksByGroup(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByGroup(const QString & group_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_group_tracks.arg(group_id), {{tkn_types, val_audio_types}}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, QStringLiteral("groups/%1/tracks").arg(group_id), {}),
                                call_type_json, rules(offset, count), 0,
                                proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default:
                            Logger::obj().write(name(), "TRACK BY GROUP is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_group, response, {}, {{CMD_ID, group_id}});
                }

                QJsonValue tracksByUser(const QUrlQuery & args) {
                    return tracksByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByUser(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_user_tracks.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("users/%1/tracks").arg(user_id),
                                    {{ QStringLiteral("representation"), QString() }}
                                ),
                                call_type_json, rules(offset, count), 0,
                                proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK BY USER is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_user, response, {}, {{CMD_ID, user_id}});
                }

                QJsonValue tracksByUserLikes(const QUrlQuery & args) {
                    return tracksByUserLikes(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue tracksByUserLikes(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_user_favorites.arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site_alt1, QStringLiteral("users/%1/likes").arg(user_id), {}),
                                call_type_json, rules(offset, count), 0,
                                proc_json_patch, IQUERY_DEF_FIELDS << QStringLiteral("collection") <<
                                QStringLiteral("track"), call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "TRACK BY USER LIKES is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_by_user_likes, response, {}, {{CMD_ID, user_id}});
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_TRACK
