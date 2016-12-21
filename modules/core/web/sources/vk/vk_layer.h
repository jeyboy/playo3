#ifndef VK_LAYER_H
#define VK_LAYER_H

#include "vk_auth.h"
#include "vk_feed.h"
#include "vk_group.h"
#include "vk_playlist.h"
#include "vk_set.h"
#include "vk_audio.h"
#include "vk_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Layer : public Sociable, public Auth, public Feed, public Group,
                    public Playlist, public Set, public Audio, public User {
            protected:
                Layer() {
                    flags = {
                        {sf_endpoint, (SourceFlags)(
                            sf_is_primary | sf_audio | sf_video | sf_sociable |
                            sf_photo | sf_feed | sf_lyric | sf_playlist | sf_compilation |
                            sf_site | sf_site_connectable | sf_api | sf_api_connectable)
                        },

                        {sf_feed,                       sf_both_auth},

//                        {sf_feed_by_user,               sf_both_auth},
//                        {sf_feed_by_group,              sf_both_auth},

                        {sf_is_shareable,               sf_both_auth /*| sf_multiple_keys*/},

                        {sf_search,                     sf_both_auth},

                        {sf_compilation,                sf_both_auth},

//                        {sf_popular_artist,         sf_both_auth},

//                        {sf_popular_audio,          sf_both_auth},
                        {sf_audio_by_id,                sf_both_auth},
                        {sf_audio_by_title,             sf_both_auth},
                        {sf_audio_by_genre,             sf_both_auth},
//                        {sf_audio_by_stream,        sf_both_auth},
                        {sf_audio_by_artist,            sf_both_auth},
//                        {sf_audio_by_compilation,   sf_both_auth},
                        {sf_audio_by_playlist,          sf_both_auth},
//                        {sf_audio_by_album,         sf_both_auth},
                        {sf_audio_by_user,              sf_both_auth},
                        {sf_audio_recs_by_user,         sf_both_auth},
                        {sf_audio_recs_by_audio,        sf_both_auth},

//                        {sf_audio_playlist_by_id,   sf_both_auth},
                        {sf_audio_playlist_by_user,     sf_both_auth},

                        {sf_video_categories,           sf_both_auth},
                        {sf_video_by_id,                sf_both_auth},
                        {sf_video_by_title,             sf_both_auth},
                        {sf_video_by_user,              sf_both_auth},
                        {sf_video_by_category,          sf_both_auth},
                        {sf_video_by_playlist,          sf_both_auth},

                        {sf_user_sociable,              sf_both_auth},
                        {sf_user_status,                sf_api},
                        {sf_user_by_id,                 sf_both_auth},
                        {sf_user_by_title,              sf_both_auth},
                        {sf_user_by_perma,              sf_both_auth},

                        {sf_group_sociable,             sf_both_auth},
                        {sf_group_by_user,              sf_both_auth},
                        {sf_group_by_title,             sf_both_auth},
                        {sf_group_by_id,                sf_both_auth},
                        {sf_group_by_perma,             sf_both_auth},

                        {sf_lyric_by_audio,             sf_both_auth}
                    };
                }

                inline QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
//                inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }

                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QJsonValue categories(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_video: return videoCategories(args.queryItemValue(CMD_OFFSET));
                        default: QJsonObject();
                    }
                }

                QJsonValue info(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_audio: return audioInfo(args.queryItemValue(CMD_ID));
                        case crelt_video: return videoInfo(args.queryItemValue(CMD_ID));
                        default: QJsonObject();
                    }
                }

                QJsonValue search(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crt_audio: return audioSearch(SearchLimit::fromICmdParams(args));
                        case crt_video: return videoSearch(SearchLimit::fromICmdParams(args));
                        default: QJsonObject();
                    }
                }

                QJsonValue recommendations(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crt_audio: {
                            switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                                case crelt_user: return userAudioRecommendations(
                                    args.queryItemValue(CMD_ID),
                                    (bool)args.queryItemValue(CMD_PREDICATE).toInt(),
                                    args.queryItemValue(CMD_OFFSET).toInt()
                                );
                                case crelt_audio: return audioRecommendations(
                                    args.queryItemValue(CMD_ID),
                                    (bool)args.queryItemValue(CMD_PREDICATE).toInt(),
                                    args.queryItemValue(CMD_OFFSET).toInt()
                                );
                                default: QJsonObject();
                            }
                        break;}
                        default: QJsonObject();
                    }
                }

                QJsonValue playlists(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crt_audio: {
                            switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                                case crelt_user: return audioPlaylistsByUser(
                                    args.queryItemValue(CMD_ID),
                                    0,
                                    args.queryItemValue(CMD_OFFSET).toInt(),
                                    args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                                );
                                default: QJsonObject();
                            }
                        break;}
                        default: QJsonObject();
                    }
                }

                QJsonValue audio(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_playlist: return audioByPlaylist(
                            args.queryItemValue(CMD_ID)/*,
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()*/
                        );
                        case crelt_artist: return audioByArtist(
                            args.queryItemValue(CMD_PREDICATE)/*,
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()*/
                        );
                        default: QJsonObject();
                    }
                }

                QJsonValue video(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_user: return videoByUser(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt()
                        );
                        case crelt_playlist: return videoByPlaylist(args.queryItemValue(CMD_ID));
                        case crelt_category: return videoByCategory(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET)
                        );

                        default: QJsonObject();
                    }
                }

                QJsonValue users(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_perma:
                        case crelt_id: return usersByIdOrPerma(
                            args.queryItemValue(CMD_ID)/*,
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()*/
                        );
                        case crelt_name: return usersByName(
                            args.queryItemValue(CMD_PREDICATE),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        default: QJsonObject();
                    }
                }

                QJsonValue groups(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_perma:
                        case crelt_id: return groupsByIdOrPerma(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_name: return groupsByName(
                            args.queryItemValue(CMD_PREDICATE),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        default: QJsonObject();
                    }
                }
            };
        }
    }
}

#endif // VK_LAYER_H
