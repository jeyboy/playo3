#ifndef OD_SET
#define OD_SET

#include "od_collection.h"
#include "od_tuner.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Set : public Collection, public Tuner {
            protected:
                enum SetType {
                    set_popular_tracks = 1, set_popular_artists, set_popular_tuners, set_of_collections,
                    set_popular_collections, set_popular_albums, set_listened, set_downloaded, set_of_radio
                };

//                inline QString listenedAudioUrl() { return audioUrlStr(path_audio_history); } // params : pagination attrs
//                inline QString downloadedAudioUrl() { return audioUrlStr(path_audio_downloaded); } // params : pagination attrs
//                inline QString popularAudioUrl() { return audioUrlStr(path_audio_popular_tracks); }
                // artists // tracks // tuners // collections // albums
//                inline QString popAudioUrl() { return audioUrlStr(path_audio_popular); } // (locale: 'ru') and pagination attrs

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_of_collections: return QStringLiteral("Some random collections");
                        case set_of_radio: return QStringLiteral("List of channels");

                        case set_popular_tracks: return QStringLiteral("Popular tracks");
                        case set_popular_artists: return QStringLiteral("Popular artists");
                        case set_popular_tuners: return QStringLiteral("Popular radios");
                        case set_popular_collections: return QStringLiteral("Popular collections");
                        case set_popular_albums: return QStringLiteral("Popular albums");
                        case set_listened: return QStringLiteral("Listened");
                        case set_downloaded: return QStringLiteral("Downloaded");
                        default: return QStringLiteral("Unknown: ");
                    }
                }

                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    switch(set_type) {
                        case set_popular_tracks: {
                            QueriableResponse response = pRequest(
                                audioUrlStr(path_audio_popular_tracks), // path_audio_popular_tracks respondable to 'tuner' field for style clarification of popular tracks
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_tracks
                            );
                        break;}

                        case set_popular_artists: {
                            QueriableResponse response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_artists
                            );
                        break;}

                        case set_popular_tuners: {
                            QueriableResponse response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << QStringLiteral("tuners")
                            );
                        break;}

                        case set_popular_collections: {
                            QueriableResponse response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << QStringLiteral("collections")
                            );
                        break;}

                        case set_popular_albums: {
                            QueriableResponse response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );
                        break;}

                        case set_listened: { //TODO: not finished
                            QueriableResponse response = pRequest(
                                audioUrlStr(path_audio_history),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );
                        break;}

                        case set_downloaded: {//TODO: not finished
                            QueriableResponse response = pRequest(
                                audioUrlStr(path_audio_downloaded),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );
                        break;}

                        case set_of_collections: return randomCollections();

                        case set_of_radio: return tunersList();

                        default:;
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // OD_SET
