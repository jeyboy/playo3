#ifndef YANDEX_LAYER_H
#define YANDEX_LAYER_H

#include "yandex_auth.h"
#include "yandex_artist.h"
#include "yandex_feed.h"
#include "yandex_playlist.h"
#include "yandex_set.h"
#include "yandex_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Layer : public Sociable, public Auth, public Artist,
                    public Feed, public Playlist, public Set, public User {
            protected:
                Layer() {
                    flags = {
                        {sf_endpoint, (SourceFlags) (
                            sf_is_primary | sf_audio | sf_video | sf_sociable | sf_site |
                            sf_feed | sf_playlist | sf_album | sf_compilation | sf_site_connectable)
                        },

                        {sf_feed,                       sf_site},

                        {sf_feed_by_user,               sf_site_auth_only},

                        {sf_is_shareable,               sf_site},

                        {sf_search,                     sf_site},

                        {sf_compilation,                sf_site},

//                        {sf_stream_list,            sf_site_auth_only},
                        {sf_stream_config,              sf_site},
                        {sf_stream_by_genre,            sf_site},
                        {sf_stream_by_artist,           sf_site},

                        {sf_album_by_id,                sf_site},
                        {sf_album_by_title,             sf_site},
                        {sf_album_by_artist,            sf_site},

                        {sf_artist_by_id,               sf_site},
                        {sf_artist_by_title,            sf_site},
                        {sf_audio_recs_by_artist,       sf_site},
//                        {sf_artist_by_title,        sf_site_auth_only},

//                        {sf_popular_artist,         sf_site_auth_only},

//                        {sf_popular_audio,          sf_site_auth_only},
                        {sf_audio_by_id,                sf_site},
                        {sf_audio_by_title,             sf_site},
//                        {sf_audio_by_stream,        sf_site_auth_only},
                        {sf_audio_by_artist,            sf_site},
//                        {sf_audio_by_compilation,   sf_site_auth_only},
                        {sf_audio_by_playlist,          sf_site},
                        {sf_audio_by_album,             sf_site},
                        {sf_audio_by_user,              sf_site_auth_only},
//                        {sf_audio_recs_by_user,     sf_site_auth_only},
////                        {sf_audio_recs_by_track,    sf_site_auth_only},

                        {sf_audio_playlist_by_id,       sf_site},
                        {sf_audio_playlist_by_title,    sf_site},
                        {sf_audio_playlist_by_user,     sf_site_auth_only},

                        {sf_video_by_id,                sf_site},
                        {sf_video_by_title,             sf_site},
//                        {sf_video_by_user,          sf_site_auth_only},
//                        {sf_video_by_category,      sf_site_auth_only},
//                        {sf_video_by_playlist,      sf_site_auth_only},


                        {sf_user_by_user,               sf_site_auth_only},
                        {sf_user_by_id,                 sf_site_auth_only},
//                        {sf_user_by_title,          sf_site_auth_only},
                    };
                }

                QJsonValue info(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_album:
                            switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                                case crelt_artist: return albumsByArtist(args.queryItemValue(CMD_ID));
                                default: QJsonObject();
                            }

                            return albumsInfo(args.queryItemValue(CMD_ID).split(','));
//                        case crelt_audio: return audioInfo(args.queryItemValue(CMD_ID));
                        default: QJsonObject();
                    }
                }

                QJsonValue search(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
//                        case crelt_audio: return audioSearch(SearchLimit::fromICmdParams(args));
                        default: QJsonObject();
                    }
                }


                QJsonValue albums(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crt_audio: {
                            switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                                case crelt_artist: return albumsByArtist(args.queryItemValue(CMD_ID));
                                default: QJsonObject();
                            }
                        break;}
                        default: QJsonObject();
                    }
                }
            };
        }
    }
}

#endif // YANDEX_LAYER_H
