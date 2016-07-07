#ifndef SOUNDCLOUD_IPLAYLIST
#define SOUNDCLOUD_IPLAYLIST

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Playlist : public Base {
            protected:
                QJsonValue playlistsByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            return pRequest(
                                baseUrlStr(
                                    qst_site_alt1, QStringLiteral("playlists/discovery"),
                                    {{ QStringLiteral("tag"), tag }}
                                ),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_OFFLINE_PER_REQUEST_LIMIT_SET),
                                0, proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "PLAYLIST BY TAG is not accessable", true);
                    }
                    return QJsonArray();
                }
                QJsonValue playlistsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            return pRequest(
                                baseUrlStr(qst_api_def, QStringLiteral("tracks/%1/playlists").arg(track_id), {}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET),
                                0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            return pRequest(
                                baseUrlStr(qst_site_alt1, QStringLiteral("tracks/%1/playlists").arg(track_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "PLAYLIST BY TRACKS is not accessable", true);
                    }
                    return QJsonArray();
                }

                // predicate is used for search in title - genre - tags - permalinks
                QJsonValue playlistsByPredicate(const QString & predicate, int count = 10, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            return pRequest(
                                baseUrlStr(qst_api_def, path_playlists, {{tkn_q, predicate}}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, SOUNDCLOUD_PER_REQUEST_LIMIT_SET),
                                0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            return pRequest(
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
                        }

                        default: Logger::obj().write("Soundcloud", "PLAYLISTS BY PREDICATE is not accessable", true);
                    }
                    return QJsonArray();
                }
                QJsonValue playlistsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api: {
                            return pRequest(
                                baseUrlStr(qst_api_def, path_user_playlists.arg(user_id), {}),
                                call_type_json, rules(offset, count, SOUNDCLOUD_PAGES_LIMIT, 25), 0, proc_json_patch
                            );
                        }

                        case perm_site: {
                            return pRequest(
                                baseUrlStr(qst_site_alt1, QStringLiteral("users/%1/playlists").arg(user_id), {}),
                                call_type_json, rules(offset, count), 0, proc_json_patch,
                                COLLECTION_FIELDS, call_method_get, headers()
                            );
                        }

                        default: Logger::obj().write("Soundcloud", "PLAYLIST BY USER is not accessable", true);
                    }
                    return QJsonArray();
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_IPLAYLIST
