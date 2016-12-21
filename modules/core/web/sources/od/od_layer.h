#ifndef OD_LAYER_H
#define OD_LAYER_H

#include "od_user.h"
#include "od_group.h"
#include "od_auth.h"
#include "od_playlist.h"
#include "od_set.h"
#include "od_audio.h"
#include "od_video.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Layer : public Sociable, public User, public Auth, public Group,
                    public Playlist, public Set, public Audio, public Video {
            protected:
                Layer() {
                    flags = QHash<SourceFlags, SourceFlags>{
                        {sf_endpoint, (SourceFlags)(
                            sf_online_credentials_req | sf_is_primary | sf_audio | sf_video | sf_user/*sf_sociable*/ |
                            sf_photo | sf_feed | sf_playlist | sf_compilation | sf_site | sf_site_connectable)
                        },

                        {sf_feed,                   sf_site},

                        {sf_feed_by_user,           sf_site_auth_only},
                        {sf_feed_by_group,          sf_site_auth_only},

                        {sf_is_shareable,           sf_site_auth_only},

                        {sf_search,                 sf_site_auth_only},

                        {sf_compilation,            sf_site_auth_only},

                        {sf_stream_list,            sf_site_auth_only},

                        {sf_album_by_title,         sf_site_auth_only},

                        {sf_artist_by_id,           sf_site_auth_only},
                        {sf_artist_by_title,        sf_site_auth_only},

                        {sf_popular_artist,         sf_site_auth_only},

                        {sf_popular_audio,          sf_site_auth_only},
                        {sf_audio_by_id,            sf_site_auth_only},
                        {sf_audio_by_title,         sf_site_auth_only},
                        {sf_audio_by_stream,        sf_site_auth_only},
                        {sf_audio_by_artist,        sf_site_auth_only},
                        {sf_audio_by_compilation,   sf_site_auth_only},
                        {sf_audio_by_playlist,      sf_site_auth_only},
                        {sf_audio_by_album,         sf_site_auth_only},
                        {sf_audio_by_user,          sf_site_auth_only},
                        {sf_audio_recs_by_user,     sf_site_auth_only},
//                        {sf_audio_recs_by_track,    sf_site_auth_only},

                        {sf_audio_playlist_by_id,   sf_site_auth_only},

                        {sf_video_by_id,            sf_site_auth_only},
                        {sf_video_by_title,         sf_site_auth_only},
                        {sf_video_by_user,          sf_site_auth_only},
                        {sf_video_by_category,      sf_site_auth_only},
                        {sf_video_by_playlist,      sf_site_auth_only},


                        {sf_user_sociable,          sf_site_auth_only},
                        {sf_user_by_id,             sf_site_auth_only},
                        {sf_user_by_title,          sf_site_auth_only},

//                        {sf_group_sociable,         sf_site_auth_only}, // not finished
                        {sf_group_by_user,          sf_site_auth_only},
                        {sf_group_by_title,         sf_site_auth_only},
                        {sf_group_by_id,            sf_site_auth_only},
                    };
                }

                QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QJsonValue search(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crel_audio: return audioSearch(SearchLimit::fromICmdParams(args));
                        case crel_video: return videoSearch(SearchLimit::fromICmdParams(args));
                        case crel_artist: return artistsSearch(SearchLimit::fromICmdParams(args));
                        case crel_audio_album: return albumsSearch(SearchLimit::fromICmdParams(args));
                        default: QJsonObject();
                    }
                }

                QJsonValue info(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crel_audio: return audioInfo(args.queryItemValue(CMD_ID));
                        case crel_video: return videoInfo(args.queryItemValue(CMD_ID));
                        case crel_audio_playlist: return playlistsInfo(args.queryItemValue(CMD_ID));
//                        case crelt_user: return userInfo();
                        default: QJsonObject();
                    }
                }

                QJsonValue audio(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crel_artist: return audioByArtist(args.queryItemValue(CMD_ID));
                        case crel_collection: return audioByCollection(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crel_tuner: return audioByTuner(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crel_album: return audioByAlbum(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crel_playlist: return audioByPlaylist(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crel_user: return audioByUser(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );

                        default: QJsonObject();
                    }
                }

                QJsonValue video(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crel_category: return videoByCategory(
                            args.queryItemValue(CMD_ID).toInt(),
                            args.queryItemValue(CMD_REQUESTS_LIMIT).toInt(),
                            args.queryItemValue(CMD_OFFSET)
                        );

                        case crel_playlist: return videoByPlaylist(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_REQUESTS_LIMIT).toInt(),
                            args.queryItemValue(CMD_OFFSET)
                        );

                        default: QJsonObject();
                    }
                }

                QJsonValue recommendations(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crel_audio | crel_user: return userAudioRecommendations(
//                                    args.queryItemValue(CMD_ID)
//                                    args.queryItemValue(CMD_OFFSET).toInt(),
//                                    args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        default: QJsonObject();
                    }
                }

                QJsonValue users(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crel_name: return usersByName(
                            args.queryItemValue(CMD_PREDICATE),
                            args.queryItemValue(CMD_OFFSET).toInt()
                        );
                        case crel_id: return usersByName(args.queryItemValue(CMD_ID));
                        default: QJsonObject();
                    }
                }

                QJsonValue groups(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crel_user: return groupsByUser(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_REQUESTS_LIMIT).toInt()
                        );

                        case crel_name: return groupsByName(
                            args.queryItemValue(CMD_PREDICATE),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_REQUESTS_LIMIT).toInt()
                        );

                        case crel_id:
                        case crel_perma: return groupsByIdOrPerma(args.queryItemValue(CMD_ID));
                        default: QJsonObject();
                    }
                }
            };
        }
    }
}

#endif // OD_LAYER_H
