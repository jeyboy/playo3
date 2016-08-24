#ifndef OD_DEFINES
#define OD_DEFINES

#include "modules/core/interfaces/search_limits.h"

#include "od_keys.h"
#include "od_misc.h"

#define OD_LIMIT_PER_REQUEST 100
#define OD_SEARCH_LIMIT 200

namespace Core {
    namespace Web {
        namespace Od {
            class Base : public Misc {
            protected:
                PolyQueryRules rules(
                    int offset = 0, int items_limit = DEFAULT_ITEMS_LIMIT,
                    int pages_count = 10, int per_request = OD_LIMIT_PER_REQUEST)
                {
                    return PolyQueryRules(
                        call_iter_type_item,
                        call_iter_method_offset,
                        qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_count, DEFAULT_REQUESTS_LIMIT),
                        tkn_limit,
                        qMin(qMin(per_request, items_limit), OD_LIMIT_PER_REQUEST),
                        tkn_offset,
                        offset
                    );
                }

                PolyQueryRules pageRules(
                    const QString & offset_token, int offset = 1,
                    int pages_count = 5, int items_limit = DEFAULT_ITEMS_LIMIT)
                {
                    return PolyQueryRules(
                       call_iter_type_page, offset, qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_count, DEFAULT_REQUESTS_LIMIT), offset_token
                    );
                }

                inline QString audioUrlStr(const QString & predicate, const std::initializer_list<std::pair<QString, QVariant> > & params = {}) {
                    return baseUrlStr(qst_site_audio, predicate, params);
                }

                // info request per ids for items (track / album / artist)
                // param (ids: ids of (track / album / artist) splited by coma)
                inline QString audioInfoUrl(const QStringList & ids) { return audioUrlStr(path_audio_info, {{tkn_ids, ids.join(',')}}); }

                inline QString audioSearchUrl(const QString & predicate) { return audioUrlStr(path_audio_search, {{ tkn_q, predicate }}); } // params : (q: predicate) and pagination attrs


                void prepareCollections(QJsonArray & collections) {
                    QJsonArray res;
                    for(QJsonArray::Iterator collection = collections.begin(); collection != collections.end(); collection++) {
                        QJsonObject collection_obj = (*collection).toObject();
                        QString uid = idToStr(collection_obj.value(tkn_id));

                        collection_obj.insert(
                            tkn_loadable_cmd,
                             Cmd::build(
                                sourceType(), cmd_mtd_tracks_by_collection,
                                {{CMD_ID, uid}}
                             ).toString()
                        );
                        res << collection_obj;
                    }

                    collections = res;
                }

                void prepareAlbums(QJsonArray & albums) {
                    QJsonArray res;
                    for(QJsonArray::Iterator album = albums.begin(); album != albums.end(); album++) {
                        QJsonObject album_obj = (*album).toObject();
                        QString uid = idToStr(album_obj.value(tkn_id));

                        album_obj.insert(
                            tkn_loadable_cmd,
                             Cmd::build(
                                sourceType(), cmd_mtd_tracks_by_album,
                                {{CMD_ID, uid}}
                             ).toString()
                        );
                        res << album_obj;
                    }

                    albums = res;
                }

                void prepareArtists(QJsonArray & artists) {
                    QJsonArray res;
                    for(QJsonArray::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                        QJsonObject artist_obj = (*artist).toObject();
                        QString uid = idToStr(artist_obj.value(tkn_id));

                        artist_obj.insert(
                            tkn_loadable_cmd,
                             Cmd::build(
                                sourceType(), cmd_mtd_tracks_by_artist,
                                {{CMD_ID, uid}}
                             ).toString()
                        );
                        res << artist_obj;
                    }

                    artists = res;
                }

                void prepareTuners(QJsonArray & tuners) {
                    QJsonArray block_content;
                    for(QJsonArray::Iterator tuner = tuners.begin(); tuner != tuners.end(); tuner++) {
                        QJsonObject tuner_obj = (*tuner).toObject();

                        QString name = tuner_obj.value(tkn_name).toString();
                        QString artists_str;

                        QJsonArray artists = tuner_obj.value(tkn_artists).toArray();
                        for(QJsonArray::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                            QJsonObject artist_obj = (*artist).toObject();

                            artists_str = artists_str % ',' % ' ' %  artist_obj.value(tkn_name).toString();
                        }

                        name = QStringLiteral("%1 (%2)").arg(name, artists_str.mid(2));
                        int source_id = sourceType();
                        QString uid = idToStr(tuner_obj.value(QStringLiteral("data")));

                        block_content << QJsonObject {
                            {tkn_id, uid },
                            {tkn_name, name},
                            {tkn_loadable_cmd,
                                 Cmd::build(
                                    source_id, cmd_mtd_tracks_by_tuner,
                                    {{CMD_ID, uid}}
                                 ).toString()
                            }
                        };
                    }

                    tuners = block_content;
                }
            };
        }
    }
}

#endif // OD_DEFINES
