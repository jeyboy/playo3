#ifndef SOUNDCLOUD_API_TRACK
#define SOUNDCLOUD_API_TRACK

#include "defines.h"
#include "../interfaces/itrack.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            namespace Api {
                class Track : public QueryBase, public ITrack {
                public:
                    QString trackUrl(const QString & track_id) {
                        QJsonObject obj = trackInfo(track_id);
                        return obj.value(QStringLiteral("stream_url")).toString();
                    }

                    bool likeTrack(const QString & user_id, const QString & track_id) {
                        QString res = Manager::prepare() -> putFollowed(
                            baseUrlStr(qst_api_def, path_user_favorites.arg(user_id) % '/' % track_id, {}),
                            Headers()
                        ) -> toText();
                        qDebug() << "FAV" << res;
                        return false; // TODO: update me
                    }

                    QJsonObject trackInfo(const QString & track_id) {
                        return sRequest(
                            baseUrlStr(qst_api_def, path_tracks % '/' % track_id, {}),
                            call_type_json
                        );
                    }

                    //"id": 142370360,
                    //"permalink": "sam-smith-stay-with-me",
                    QJsonValue tracksInfo(const QStringList & track_ids) { //TODO: test me
                        QJsonArray arr;
                        int limiter = track_ids.size() / SOUNDCLOUD_IDS_PER_REQUEST + (int)(track_ids.size() % SOUNDCLOUD_IDS_PER_REQUEST != 0);
                        for(int step = 0; step < limiter; step++) {
                            QJsonObject obj = sRequest(
                                baseUrlStr(
                                    qst_api_def, path_tracks,
                                    {{ tkn_ids, (QStringList)track_ids.mid(step * SOUNDCLOUD_IDS_PER_REQUEST, SOUNDCLOUD_IDS_PER_REQUEST) }}
                                ),
                                call_type_json, 0, proc_json_wrap
                            );

                            QJsonArray new_items = obj.value(DEF_JSON_FIELD).toArray();
                            QueriableArg::arrAppend(arr, new_items);
                        }

                        return arr;
                    }

                    QJsonValue trackRelations(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { //TODO: test me
                        return pRequest(
                            baseUrlStr(qst_api_def, path_related_tracks.arg(track_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch
                        );
                    }

                    QJsonValue tracksSearch(const SearchLimit & limitations) {
                        return pRequest(
                            baseUrlStr(
                                qst_api_def, path_tracks,
                                trackSearchQuery(limitations.predicate, limitations.genre, limitations.by_popularity())
                            ),
                            call_type_json,
                            rules(limitations.start_offset, limitations.items_limit), 0, proc_json_patch
                        );
                    }


                    // support a comma separated list of tags
                    QJsonValue tracksByTag(const QString & tags, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
                        return pRequest(
                            baseUrlStr(qst_api_def, path_tracks, {{ QStringLiteral("tags"), tags }}),
                            call_type_json, rules(offset, count), 0, proc_json_patch
                        );
                    }


                    QJsonValue tracksByGroup(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                        return pRequest(
                            baseUrlStr(qst_api_def, path_group_tracks.arg(group_id), {{tkn_types, val_audio_types}}),
                            call_type_json, rules(offset, count), 0, proc_json_patch
                        );
                    }

                    QJsonValue tracksByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                        QJsonArray res = pRequest(
                            baseUrlStr(qst_api_def, path_user_tracks.arg(user_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch
                        );

                        return res;
                    }

                    QJsonValue tracksByUserLikes(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) { // next_href
                        QJsonArray res = pRequest(
                            baseUrlStr(qst_api_def, path_user_favorites.arg(user_id), {}),
                            call_type_json, rules(offset, count), 0, proc_json_patch
                        );

                        return res;
                    }
                };
            }
        }
    }
}

#endif // SOUNDCLOUD_API_TRACK
