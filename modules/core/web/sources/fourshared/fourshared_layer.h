#ifndef FOURSHARED_LAYER_H
#define FOURSHARED_LAYER_H

#include "fourshared_auth.h"
#include "fourshared_audio.h"
#include "fourshared_video.h"
#include "fourshared_set.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Layer : public Auth, public Set {
            protected:
                Layer() {
                    flags = {
                        {sf_endpoint, (SourceFlags)
                            (sf_is_primary | sf_audio | sf_video | sf_compilation |
                            sf_site /*| sf_api*/ | sf_site_connectable /*| sf_api_connectable*/)
                        },

                        {sf_compilation,        sf_site_not_api_auth},

                        {sf_search,             sf_site_not_api_auth},
//                        {sf_user_sociable,      sf_both_auth},

                        {sf_is_shareable,       sf_site_not_api_auth},

                        {sf_popular_audio,      sf_site_not_api_auth},
                        {sf_audio_by_id,        sf_site_not_api_auth},
                        {sf_audio_by_title,     sf_site_not_api_auth},
                        {sf_audio_by_genre,     sf_site_not_api_auth},
                        {sf_audio_by_tag,       sf_site_not_api_auth},
                        {sf_audio_by_artist,    sf_site_not_api_auth},
                        {sf_audio_by_album,     sf_site_not_api_auth},

                        {sf_popular_video,      sf_site_not_api_auth},
                        {sf_video_by_title,     sf_site_not_api_auth},
                        {sf_video_by_genre,     sf_site_not_api_auth},
                        {sf_video_by_tag,       sf_site_not_api_auth}
                    };
                }

                QJsonValue info(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_audio: return audioInfo(args.queryItemValue(CMD_ID));
                        case crelt_video: return videoInfo(args.queryItemValue(CMD_ID));
                        default: QJsonObject();
                    }
                }

                QJsonValue search(const QUrlQuery & args) {
                    switch(args.queryItemValue(CMD_RELATION_TYPE).toInt()) {
                        case crelt_audio: return audioSearch(SearchLimit::fromICmdParams(args));
                        case crelt_video: return videoSearch(SearchLimit::fromICmdParams(args));
                        default: QJsonObject();
                    }
                }

                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }
                QJsonValue openSet(const QUrlQuery & attrs) {
                    return QJsonArray() << setByType(attrs);
                }

                QJsonValue loadContainerData(const QUrlQuery & attrs) {
                    return itemsByCollection(attrs.queryItemValue(CMD_ID));
                }
                QJsonValue loadContainerData(const QString & attrs) { return loadContainerData(QUrlQuery(attrs)); }


            };
        }
    }
}

#endif // FOURSHARED_LAYER_H
