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
            public:
                QJsonValue search(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_audio: return audioSearch(SearchLimit::fromICmdParams(args));
                        case crelt_album: return albumsSearch(SearchLimit::fromICmdParams(args));
                        case crelt_artist: return artistsSearch(SearchLimit::fromICmdParams(args));
                        default: QJsonObject();
                    }
                }

                QJsonValue info(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_audio: return audioInfo(args.queryItemValue(CMD_ID));
                        default: QJsonObject();
                    }
                }

                QJsonValue audio(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_artist: return audioByArtist(args.queryItemValue(CMD_ID));
                        case crelt_collection: return audioByCollection(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_tuner: return audioByTuner(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_album: return audioByAlbum(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_playlist: return audioByPlaylist(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );
                        case crelt_user: return audioByUser(
                            args.queryItemValue(CMD_ID),
                            args.queryItemValue(CMD_OFFSET).toInt(),
                            args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                        );

                        default: QJsonObject();
                    }
                }

                QJsonValue recommendations(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RESULT_TYPE).toInt()) {
                        case crt_audio: {
                            switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {

                            }
                        break;}
                    }
                }

                //                    QJsonValue userAudioRecommendations(const QUrlQuery & /*args*/) {
                //                        return userAudioRecommendations(
                //    //                        args.queryItemValue(CMD_ID)
                //    //                        args.queryItemValue(CMD_OFFSET).toInt(),
                //    //                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                //                        );
                //                    }
            };
        }
    }
}

#endif // OD_LAYER_H
