#ifndef OD_SET
#define OD_SET

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Set : public Base {
            protected:
                enum SetType {
                    set_popular_tracks = 1, set_popular_artists, set_popular_tuners,
                    set_popular_collections, set_popular_albums, set_listened, set_downloaded
                };

//                inline QString listenedAudioUrl() { return audioUrlStr(path_audio_history); } // params : pagination attrs
//                inline QString downloadedAudioUrl() { return audioUrlStr(path_audio_downloaded); } // params : pagination attrs
//                inline QString popularAudioUrl() { return audioUrlStr(path_audio_popular_tracks); }
                // artists // tracks // tuners // collections // albums
//                inline QString popAudioUrl() { return audioUrlStr(path_audio_popular); } // (locale: 'ru') and pagination attrs

                QJsonValue setByType(const SetType & set_type, const QString & /*attrs*/ = QString()) {
//                    QStringList params = attrs.split('|', QString::SkipEmptyParts);

                    switch(set_type) {
                        case set_popular_tracks: {
                            return pRequest(
                                audioUrlStr(path_audio_popular_tracks),
                                call_type_json, rules(), 0,
                                proc_json_extract, QStringList() << tkn_tracks
                            );
                        break;}

                        case set_popular_artists: {
                            return pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(), 0,
                                proc_json_extract, QStringList() << tkn_artists
                            );
                        break;}

                        case set_popular_tuners: {
                            return pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(), 0,
                                proc_json_extract, QStringList() << QStringLiteral("tuners")
                            );
                        break;}

                        case set_popular_collections: {
                            return pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(), 0,
                                proc_json_extract, QStringList() << QStringLiteral("collections")
                            );
                        break;}

                        case set_popular_albums: {
                            return pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );
                        break;}

                        case set_listened: { //TODO: not finished
                            return pRequest(
                                audioUrlStr(path_audio_history),
                                call_type_json, rules(), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );
                        break;}

                        case set_downloaded: {//TODO: not finished
                            return pRequest(
                                audioUrlStr(path_audio_downloaded),
                                call_type_json, rules(), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );
                        break;}

                        default:;
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // OD_SET
