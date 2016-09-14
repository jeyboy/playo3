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

                QJsonArray & prepareTracks(QJsonArray & tracks) { // TODO: write me
                    return tracks;
                }

                QJsonArray & preparePromotions(QJsonArray & promos) { // TODO: write me
                    QJsonArray res;

                    for(QJsonArray::Iterator promo = promos.begin(); promo != promos.end(); promo++) {
                        QJsonObject promo_obj = (*promo).toObject();

                        if (JSON_HAS_KEY(promo_obj, tkn_albums)) {
                            QJsonArray albums = JSON_ARR(promo_obj, tkn_albums);
                            res << prepareBlock(dmt_audio_set, prepareAlbums(albums), {{tkn_dir_name, JSON_STR(promo_obj, tkn_title)}});
                        } else if (JSON_HAS_KEY(promo_obj, tkn_playlist)) {
                            QJsonObject playlist = JSON_OBJ(promo_obj, tkn_playlist);
                            res << prepareBlock(dmt_audio_set, preparePlaylist(playlist), {{tkn_dir_name, JSON_STR(promo_obj, tkn_title)}});
                        } else {
                            qCritical() << "undefined promo:" << promo_obj;
                        }
                    }

                    promos = res;
                    return promos;
                }

                QJsonObject & preparePlaylist(QJsonObject & playlist) {
                    QJsonObject pl_obj = JSON_OBJ(playlist, tkn_playlist);

                    pl_obj.insert(
                        tkn_coverUri,
                        LSTR("http://") + JSON_STR(pl_obj, LSTR("ogImage")).replace(LSTR("%%"), LSTR("400x400"))
                    );

                    pl_obj.insert(
                        tkn_loadable_cmd,
                         Cmd::build(
                            sourceType(), cmd_mtd_tracks_by_playlist,
                            {
                                {CMD_ID, JSON_CSTR(pl_obj, LSTR("kind"))},
                                {CMD_OWNER, JSON_CSTR(JSON_OBJ(pl_obj, LSTR("owner")), LSTR("login"))}
                            }
                         ).toString()
                    );

                    return (playlist = pl_obj);
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

                    albums = res;
                    return albums;
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

                    artists = res;
                    return artists;
                }
            };
        }
    }
}

#endif // YANDEX_DEFINES_H
