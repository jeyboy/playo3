#ifndef OD_SET
#define OD_SET

#include "od_collection.h"
#include "od_tuner.h"
#include "od_album.h"
#include "od_artist.h"
#include "od_video_playlist.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Set : public Collection, public Tuner, public Album, public Artist, public VideoPlaylist {
            protected:               
                enum SetType {
                    set_popular_tracks = 1, set_popular_artists, set_popular_tuners, set_of_collections,
                    set_popular_collections, set_popular_albums, set_listened, set_downloaded, set_of_radio,

                    set_video_my = 1000, set_video_my_liked, set_video_my_history, set_video_my_subscriptions,
                    set_video_week_top, set_video_new, set_video_popular, set_video_serial, set_video_tv_show,
                    set_video_kino, set_video_bloggers, set_video_children, set_video_music
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
                        case set_popular_tuners: return QStringLiteral("Popular channels");
                        case set_popular_collections: return QStringLiteral("Popular collections");
                        case set_popular_albums: return QStringLiteral("Popular albums");
                        case set_listened: return QStringLiteral("Listened");
                        case set_downloaded: return QStringLiteral("Downloaded");

                        default:
                            if (stype >= set_video_my)
                                return vplToString((VideoPlaylistType)stype);

                            return QStringLiteral("Unknown");
                    }
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    QueriableResponse response;
                    DataMediaType dmt_val = dmt_unknow;

                    switch(set_type) {
                        case set_popular_tracks: {
                            response = pRequest(
                                audioUrlStr(path_audio_popular_tracks), // path_audio_popular_tracks respondable to 'tuner' field for style clarification of popular tracks
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_tracks
                            );
                            dmt_val = dmt_audio;
                        break;}

                        case set_popular_artists: {
                            response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_artists
                            );

                            prepareArtists(response.content);
                            dmt_val = dmt_set;
                        break;}

                        case set_popular_tuners: {
                            response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << QStringLiteral("tuners")
                            );

                            prepareTuners(response.content);
                            dmt_val = dmt_set;
                        break;}

                        case set_popular_collections: {
                            response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_collections
                            );

                            prepareCollections(response.content);
                            dmt_val = dmt_set;
                        break;}

                        case set_popular_albums: {
                            response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ QStringLiteral("locale"), QStringLiteral("ru") }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );

                            prepareAlbums(response.content);
                            dmt_val = dmt_set;
                        break;}

                        case set_listened: {
                            response = pRequest(
                                audioUrlStr(path_audio_history),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_tracks
                            );
                            dmt_val = dmt_audio;
                        break;}

                        case set_downloaded: { //TODO: not tested
                            response = pRequest(
                                audioUrlStr(path_audio_downloaded),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << tkn_albums
                            );
                            dmt_val = dmt_audio;
                        break;}

                        case set_of_collections: return randomCollections();

                        case set_of_radio: return tunersList();

                        default:
                            if (set_type >= set_video_my)
                                return videoByCategory((VideoPlaylistType)set_type);
                    }

                    return prepareBlock(dmt_val, cmd_mtd_set_by_type, response, limits, {{CMD_SET_TYPE, set_type}});
                }   
            public:
                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                inline QMap<QString, QString> setsList() {
                    return {
                        {setTypeToStr(set_popular_tracks),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_popular_tracks))},
                        {setTypeToStr(set_popular_artists),     QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_popular_artists))},
                        {setTypeToStr(set_popular_tuners),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_popular_tuners))},
                        {setTypeToStr(set_popular_collections), QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_popular_collections))},
                        {setTypeToStr(set_popular_albums),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_popular_albums))},
                        {setTypeToStr(set_listened),            QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_listened))},
                        {setTypeToStr(set_downloaded),          QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_downloaded))},

                        {setTypeToStr(set_of_collections),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_of_collections))},
                        {setTypeToStr(set_of_radio),            QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_of_radio))},

                        {setTypeToStr(set_video_my),            QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_my))},
                        {setTypeToStr(set_video_my_liked),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_my_liked))},
                        {setTypeToStr(set_video_my_history),    QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_my_history))},
                        {setTypeToStr(set_video_my_subscriptions),    QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_my_subscriptions))},
                        {setTypeToStr(set_video_week_top),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_week_top))},
                        {setTypeToStr(set_video_new),           QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_new))},
                        {setTypeToStr(set_video_popular),       QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_popular))},
                        {setTypeToStr(set_video_serial),        QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_serial))},
                        {setTypeToStr(set_video_tv_show),       QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_tv_show))},
                        {setTypeToStr(set_video_kino),          QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_kino))},
                        {setTypeToStr(set_video_bloggers),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_bloggers))},
                        {setTypeToStr(set_video_children),      QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_children))},
                        {setTypeToStr(set_video_music),         QStringLiteral("%1=%2").arg(CMD_SET_TYPE, QString::number(set_video_music))},
                    };
                }

                inline QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
//                inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
            };                    
        }
    }
}

#endif // OD_SET
