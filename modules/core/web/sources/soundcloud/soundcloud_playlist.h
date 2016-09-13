#ifndef SOUNDCLOUD_PLAYLIST
#define SOUNDCLOUD_PLAYLIST

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Playlist : public Base {
            public:
                QJsonValue playlistsByTag(const QUrlQuery & args) {
                    return playlistsByTag(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue playlistsByTag(const QString & tag, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("playlists/discovery"),
                                    {{ QStringLiteral("tag"), tag }}
                                ),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET),
                                0, proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "PLAYLIST BY TAG is not accessable", true);
                    }
                    return prepareBlock(dmt_set, cmd_mtd_playlists_by_tag, response, {}, {{CMD_PREDICATE, tag}});
                }

                QJsonValue playlistsByTrack(const QUrlQuery & args) {
                    return playlistsByTrack(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue playlistsByTrack(const QString & track_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, QStringLiteral("tracks/%1/playlists").arg(track_id), {}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET),
                                0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site_alt1, QStringLiteral("tracks/%1/playlists").arg(track_id), {}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET),
                                0, proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "PLAYLIST BY TRACKS is not accessable", true);
                    }
                    return prepareBlock(dmt_set, cmd_mtd_playlists_by_track, response, {}, {{CMD_ID, track_id}});
                }

                QJsonValue playlistsByPredicate(const QUrlQuery & args) {
                    return playlistsByPredicate(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                // predicate is used for search in title - genre - tags - permalinks
                QJsonValue playlistsByPredicate(const QString & predicate, int offset = 0, int count = 10) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_playlists, {{tkn_q, predicate}}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET),
                                0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("search/playlists"),
                                    {
                                        { tkn_q, predicate },
                                        { QStringLiteral("user_id"), Manager::cookie(QStringLiteral("sc_anonymous_id"), url_site_base) },
                                        { QStringLiteral("sc_a_id"), generateMark() },
                                        { QStringLiteral("facet"), QStringLiteral("genre") }
                                    }
                                ),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET), 0,
                                proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "PLAYLISTS BY PREDICATE is not accessable", true);
                    }
                    return prepareBlock(dmt_set, cmd_mtd_playlists_by_predicate, response, {}, {{CMD_PREDICATE, predicate}});
                }

                QJsonValue playlistsByUser(const QUrlQuery & args) {
                    return playlistsByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue playlistsByUser(const QString & user_id, int offset = 0, int count = SOUNDCLOUD_ITEMS_LIMIT) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(qst_api, path_user_playlists.arg(user_id), {}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET), 0, proc_json_patch
                            );
                        break;}

                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site_alt1, QStringLiteral("users/%1/playlists").arg(user_id), {}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        break;}

                        default: Logger::obj().write(name(), "PLAYLIST BY USER is not accessable", true);
                    }
                    return prepareBlock(dmt_audio_set, cmd_mtd_playlists_by_user, response, {}, {{CMD_ID, user_id}});
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_PLAYLIST
