#ifndef SOUNDCLOUD_LAYER_H
#define SOUNDCLOUD_LAYER_H

#include "soundcloud_auth.h"
#include "soundcloud_group.h"
#include "soundcloud_playlist.h"
#include "soundcloud_set.h"
#include "soundcloud_audio.h"
#include "soundcloud_user.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Layer : public Sociable, public Auth, public Group,
                    public Playlist, public Set, public Audio, public User {
            protected:
                Layer() {
                    flags = {
                        {sf_endpoint, (SourceFlags)(
                            sf_offline_credentials_req | sf_is_primary | sf_audio | sf_user/*sf_sociable*/ |
                            sf_feed | sf_playlist | sf_compilation | sf_site | sf_api | sf_api_connectable)
                        },

//                        {sf_feed,                   sf_site},

                        {sf_feed_by_user,               sf_both_auth},
//                        {sf_feed_by_group,          sf_site_auth_only},

                        {sf_is_shareable,               sf_site_not_api_auth},

                        {sf_search,                     sf_site_not_api_auth},

                        {sf_compilation,                sf_site_not_api_auth},

//                        {sf_popular_audio,          sf_site_auth_only},
                        {sf_like_audio,                 sf_both_auth},
                        {sf_audio_by_id,                sf_site_not_api_auth},
                        {sf_audio_by_title,             sf_site_not_api_auth},
                        {sf_audio_by_genre,             sf_site_not_api_auth},
                        {sf_audio_by_tag,               sf_site_not_api_auth},
                        {sf_audio_by_group,             sf_site_not_api_auth},
                        {sf_audio_by_like,              sf_site_not_api_auth},
                        {sf_audio_by_user,              sf_site_not_api_auth},
                        {sf_audio_recs_by_user,         sf_site},
                        {sf_audio_recs_by_audio,        sf_site_not_api_auth},

//                        {sf_audio_playlist_by_id,   sf_site_auth_only},
                        {sf_audio_playlist_by_title,    sf_site_not_api_auth},
                        {sf_audio_playlist_by_tag,      sf_site},
                        {sf_audio_playlist_by_audio,    sf_site_not_api_auth},
                        {sf_audio_playlist_by_user,     sf_site_not_api_auth},

                        {sf_user_sociable,              sf_site_not_api_auth},
//                        {sf_group_sociable,             sf_site_not_api_auth},

                        {sf_user_by_id,                 sf_site_not_api_auth},
                        {sf_user_by_title,              sf_site_not_api_auth},
                        {sf_user_by_likes,              sf_site_not_api_auth},
                        {sf_user_by_audio_repost,       sf_site_not_api_auth},

//                        {sf_group_by_user,              sf_site_not_api_auth},
//                        {sf_group_by_title,             sf_site_not_api_auth},
//                        {sf_group_by_id,                sf_site_not_api_auth},
//                        {sf_group_by_audio,             sf_site}
                    };
                }

                //QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
                QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QJsonValue audio(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_audio: return audioByTag(
                            args.queryItemValue(CMD_PREDICATE),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );

                        case crelt_group: return audioByGroup(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );

                        case crelt_user: return audioByUser(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );

                        case crelt_user_likes: return audioByUserLikes(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );

                        default: QJsonObject();
                    }
                }

                QJsonValue playlists(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crt_audio: {
                            switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                                case crelt_tag: return playlistsByTag(
                                    args.queryItemValue(CMD_PREDICATE),
                                    args.queryItemValue(CMD_OFFSET).toInt(),
                                    args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                                );
                                case crelt_audio: return playlistsByAudio(
                                    args.queryItemValue(CMD_ID),
                                    args.queryItemValue(CMD_OFFSET).toInt(),
                                    args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                                );
                                case crelt_user: return playlistsByUser(
                                    args.queryItemValue(CMD_ID),
                                    args.queryItemValue(CMD_OFFSET).toInt(),
                                    args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                                );
                                default: QJsonObject();
                            }
                        break;}
                        default: QJsonObject();
                    }
                }

                QJsonValue search(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crelt_audio: return audioSearch(SearchLimit::fromICmdParams(args));
                        case crelt_playlist: return paylistsSearch(SearchLimit::fromICmdParams(args));
                        default: QJsonObject();
                    }
                }

                QJsonValue users(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_id: return usersById(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_name: return usersByName(
                            args.queryItemValue(CMD_PREDICATE),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_audio_likes: return usersByAudioLikes(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_audio_reposting: return usersByAudioReposting(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );

                        default: QJsonObject();
                    }
                }

                QJsonValue groups(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_id: return groupsById(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_audio: return groupsByAudio(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_user: return groupsByUser(
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

                QJsonValue info(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_audio: return audioInfo(args.queryItemValue(CMD_ID));
                        case crelt_group: return groupInfo(args.queryItemValue(CMD_ID));
                        default: QJsonObject();
                    }
                }

                QJsonValue recommendations(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crt_audio: {
                            switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                                case crelt_user: return audioRecommendations(
                                    args.queryItemValue(CMD_ID),
                                    args.queryItemValue(CMD_OFFSET).toInt(),
                                    args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                                );
                                default: QJsonObject();
                            }
                        break;}
                        case crt_stream: {
                            switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                                case crelt_user: return streamsRecommendations(
                                    args.queryItemValue(CMD_OFFSET).toInt(),
                                    args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                                );
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

#endif // SOUNDCLOUD_LAYER_H
