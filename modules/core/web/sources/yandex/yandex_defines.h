#ifndef YANDEX_DEFINES_H
#define YANDEX_DEFINES_H

#include "modules/core/interfaces/search_limits.h"

#include "yandex_misc.h"

#define YANDEX_SEARCH_LIMIT 200
#define YANDEX_IDS_PER_REQUEST 50

namespace Core {
    namespace Web {
        namespace Yandex {
            class Base : public Misc {
            protected:
                //                PolyQueryRules rules(
                //                    int offset = 0, int items_limit = SOUNDCLOUD_ITEMS_LIMIT, int pages_limit = SOUNDCLOUD_PAGES_LIMIT,
                //                    int per_request = SOUNDCLOUD_PER_REQUEST_LIMIT,
                //                    ApiCallIterType call_type = call_iter_type_page)
                //                {
                //                    return PolyQueryRules(
                //                        call_type,
                //                        call_iter_method_offset,
                //                        qMin(items_limit, SOUNDCLOUD_ITEMS_LIMIT),
                //                        qMin(pages_limit, SOUNDCLOUD_PAGES_LIMIT),
                //                        tkn_limit,
                //                        qMin(qMin(per_request, items_limit), SOUNDCLOUD_PER_REQUEST_LIMIT),
                //                        tkn_offset,
                //                        offset
                //                    );
                //                }

                PolyQueryRules pageRules(
                    const QString & offset_token, int offset = 0,
                    int pages_count = 1, int items_limit = DEFAULT_ITEMS_LIMIT)
                {
                    return PolyQueryRules(
                       call_iter_type_page, offset, qMin(items_limit, DEFAULT_ITEMS_LIMIT),
                        qMin(pages_count, DEFAULT_REQUESTS_LIMIT), offset_token
                    );
                }

                //filter = albums: genre, pager: {total, page, perPage}, albums,
                //filter = : genre, tracks, artists, albums, pics
//                inline QString genresUrl(QString genre = QString(), const QString & filter = QString()) { return url_site_v1 + QStringLiteral("genre.jsx?genre=%1&filter=%2").arg(genre, filter); }

                QJsonObject prepareTracksBlock(QJsonObject & obj, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QJsonArray tracks = JSON_ARR(obj, tkn_tracks);
                    QJsonArray track_ids = JSON_ARR(obj, LSTR("trackIds"));
                    QJsonObject tracks_block = prepareBlock(dmt_audio, prepareTracks(tracks), block_params);

                    if (tracks.size() < track_ids.size()) {
                        QString ids;

                        for(QJsonArray::ConstIterator id = track_ids.constBegin() + tracks.size(); id != track_ids.constEnd(); id++)
                            ids = ids % (ids.isEmpty() ? QString() : LSTR(",")) % (*id).toString();

                        tracks_block.insert(
                            Web::tkn_more_cmd,
                            Cmd::build(sourceType(), cmd_mtd_tracks_info, {{CMD_ID, ids}}).toString()
                        );
                    }

                    return tracks_block;
                }
                QJsonObject preparePlaylistsBlock(QJsonObject & obj, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QJsonArray playlists = JSON_ARR(obj, tkn_playlists);
                    QJsonArray playlist_ids = JSON_ARR(obj, LSTR("playlistIds"));
                    QJsonObject playlists_block = prepareBlock(dmt_audio_set, preparePlaylists(playlists), block_params);

                    if (playlists.size() < playlist_ids.size()) {
                        QString ids;

                        for(QJsonArray::ConstIterator id = playlist_ids.constBegin() + playlists.size(); id != playlist_ids.constEnd(); id++)
                            ids = ids % (ids.isEmpty() ? QString() : LSTR(",")) % (*id).toString();

                        playlists_block.insert(
                            Web::tkn_more_cmd,
                            Cmd::build(sourceType(), cmd_mtd_playlists_info, {{CMD_ID, ids}}).toString()
                        );
                    }

                    return playlists_block;
                }

                QJsonArray & prepareUsers(QJsonArray & users) {
                    QJsonArray res;

                    for(QJsonArray::Iterator user = users.begin(); user != users.end(); user++) {
                        QJsonObject user_obj = (*user).toObject();

                        user_obj.insert(
                            tkn_coverUri,
                            LSTR("https://yapic.yandex.ru/get/%1/islands-retina-middle").arg(JSON_CSTR(user_obj, tkn_uid))
                        );

                        res << user_obj;
                    }

                    return (users = res);
                }
                QJsonArray & prepareTracks(QJsonArray & tracks) { // INFO: its just a stub at this moment
                    return tracks;
                }
                QJsonArray & preparePromotions(QJsonArray & promos) {
                    QJsonArray blocks;

                    for(QJsonArray::Iterator promo = promos.begin(); promo != promos.end(); promo++) {
                        QJsonObject promo_obj = (*promo).toObject();

                        if (JSON_HAS_KEY(promo_obj, tkn_album)) {
                            QJsonObject album = JSON_OBJ(promo_obj, tkn_album);
                            blocks << prepareBlock(dmt_audio_set, prepareAlbums(QJsonArray() << album), {{tkn_dir_name, JSON_STR(promo_obj, tkn_title)}});
                        } else if (JSON_HAS_KEY(promo_obj, tkn_albums)) {
                            QJsonArray albums = JSON_ARR(promo_obj, tkn_albums);
                            blocks << prepareBlock(dmt_audio_set, prepareAlbums(albums), {{tkn_dir_name, JSON_STR(promo_obj, tkn_title)}});
                        } else if (JSON_HAS_KEY(promo_obj, tkn_playlist)) {
                            QJsonObject playlist = JSON_OBJ(promo_obj, tkn_playlist);
                            blocks << prepareBlock(dmt_audio_set, preparePlaylists(QJsonArray() << playlist));
                        } else if (JSON_HAS_KEY(promo_obj, tkn_playlists)) {
                            QJsonArray playlists = JSON_ARR(promo_obj, tkn_playlists);
                            blocks << prepareBlock(dmt_audio_set, preparePlaylists(playlists));
                        } else if (JSON_HAS_KEY(promo_obj, tkn_track)) {
                            QJsonObject track = JSON_OBJ(promo_obj, tkn_track);
                            blocks << prepareBlock(dmt_audio, prepareTracks(QJsonArray() << track), {{tkn_dir_name, JSON_STR(promo_obj, tkn_title)}});
                        } else if (JSON_HAS_KEY(promo_obj, tkn_tracks)) {
                            QJsonArray tracks = JSON_ARR(promo_obj, tkn_tracks);
                            blocks << prepareBlock(dmt_audio, prepareTracks(tracks), {{tkn_dir_name, JSON_STR(promo_obj, tkn_title)}});
                        } else {
                            qCritical() << "undefined promo:" << promo_obj;
                        }
                    }

                    return (promos = blocks);
                }
                QJsonArray & preparePlaylists(QJsonArray & playlists) {
                    QJsonArray res;

                    for(QJsonArray::Iterator playlist = playlists.begin(); playlist != playlists.end(); playlist++) {
                        QJsonObject playlist_obj = (*playlist).toObject();
                        if (JSON_HAS_KEY(playlist_obj, tkn_playlist))
                            playlist_obj = JSON_OBJ((*playlist).toObject(), tkn_playlist);

                        if (JSON_HAS_KEY(playlist_obj, LSTR("ogImage")))
                            playlist_obj.insert(
                                tkn_coverUri,
                                LSTR("http://") + JSON_STR(playlist_obj, LSTR("ogImage")).replace(LSTR("%%"), LSTR("400x400"))
                            );

                        playlist_obj.insert(
                            tkn_loadable_cmd,
                             Cmd::build(
                                sourceType(), cmd_mtd_tracks_by_playlist,
                                {
                                    {CMD_ID, JSON_CSTR(playlist_obj, LSTR("kind"))},
                                    {CMD_OWNER, JSON_CSTR(JSON_OBJ(playlist_obj, tkn_owner), tkn_login)}
                                }
                             ).toString()
                        );

                        res << playlist_obj;
                    }

                    return (playlists = res);
                }
                QJsonArray & prepareAlbums(QJsonArray & albums) {
                    QJsonArray res;

                    for(QJsonArray::Iterator album = albums.begin(); album != albums.end(); album++) {
                        QJsonObject album_obj = (*album).toObject();

                        album_obj.insert(
                            tkn_loadable_cmd,
                             Cmd::build(
                                sourceType(), cmd_mtd_tracks_by_album,
                                {{CMD_ID, JSON_CSTR(album_obj, tkn_id)}}
                             ).toString()
                        );

                        QString cat_str;
                        QJsonArray artists = JSON_ARR(album_obj, tkn_artists);
                        for(QJsonArray::Iterator item = artists.begin(); item != artists.end(); item++) {
                            QJsonObject item_obj = (*item).toObject();
                            cat_str = cat_str % (cat_str.isEmpty() ? QString() : LSTR(",")) % JSON_STR(item_obj, tkn_name);
                        }

                        album_obj.insert(
                            tkn_full_title,
                            QString(cat_str % LSTR(" - ") % JSON_STR(album_obj, tkn_title))
                        );

                        album_obj.insert(
                            tkn_coverUri,
                            LSTR("http://") + JSON_STR(album_obj, tkn_coverUri).replace(LSTR("%%"), LSTR("s400x400"))
                        );

                        res << album_obj;
                    }

                    return (albums = res);
                }
                QJsonArray & prepareArtists(QJsonArray & artists) {
                    QJsonArray res;

                    for(QJsonArray::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                        QJsonObject artist_obj = (*artist).toObject();

                        artist_obj.insert(
                            tkn_loadable_cmd,
                             Cmd::build(
                                sourceType(), cmd_mtd_artist_info,
                                {{CMD_ID, JSON_CSTR(artist_obj, tkn_id)}}
                             ).toString()
                        );

                        artist_obj.insert(
                            tkn_coverUri,
                            LSTR("http://") + JSON_STR(JSON_OBJ(artist_obj, tkn_cover), LSTR("uri")).replace(LSTR("%%"), LSTR("s400x400"))
                        );

                        artist_obj.insert(tkn_full_title, JSON_STR(artist_obj, tkn_name));

                        res << artist_obj;
                    }

                    return (artists = res);
                }
                QJsonArray & prepareVideos(QJsonArray & videos) {//TODO: write me
                    int i = 0;
//                    QJsonArray res;

//                    for(QJsonArray::Iterator video = videos.begin(); video != videos.end(); video++) {
//                        QJsonObject video_obj = (*video).toObject();

//                        video_obj.insert(
//                            tkn_loadable_cmd,
//                             Cmd::build(
//                                sourceType(), cmd_mtd_artist_info,
//                                {{CMD_ID, JSON_CSTR(video_obj, tkn_id)}}
//                             ).toString()
//                        );

//                        video_obj.insert(
//                            tkn_coverUri,
//                            LSTR("http://") + JSON_STR(JSON_OBJ(video_obj, tkn_cover), LSTR("uri")).replace(LSTR("%%"), LSTR("s400x400"))
//                        );

//                        video_obj.insert(tkn_full_title, JSON_STR(artist_obj, tkn_name));

//                        res << video_obj;
//                    }

//                    return (videos = res);

                    return videos;
                }

            };
        }
    }
}

#endif // YANDEX_DEFINES_H
