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
                        case set_of_collections:        return LSTR("Some random collections");
                        case set_of_radio:              return LSTR("List of channels");

                        case set_popular_tracks:        return LSTR("Popular tracks");
                        case set_popular_artists:       return LSTR("Popular artists");
                        case set_popular_tuners:        return LSTR("Popular channels");
                        case set_popular_collections:   return LSTR("Popular collections");
                        case set_popular_albums:        return LSTR("Popular albums");
                        case set_listened:              return LSTR("Listened");
                        case set_downloaded:            return LSTR("Downloaded");

                        default:
                            if (stype >= set_video_my)
                                return vplToString((VideoPlaylistType)stype);

                            return LSTR("Unknown");
                    }
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
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
                                    {{ LSTR("locale"), siteLocale(const_default_locale) }}
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
                                    {{ LSTR("locale"), siteLocale(const_default_locale) }}
                                ),
                                call_type_json, rules(limits.start_offset, limits.items_limit), 0,
                                proc_json_extract, QStringList() << LSTR("tuners")
                            );

                            prepareTuners(response.content);
                            dmt_val = dmt_set;
                        break;}

                        case set_popular_collections: {
                            response = pRequest(
                                audioUrlStr(
                                    path_audio_popular,
                                    {{ LSTR("locale"), siteLocale(const_default_locale) }}
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
                                    {{ LSTR("locale"), siteLocale(const_default_locale) }}
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

                    return prepareBlock(dmt_val, cmd_mtd_set_by_type, response, limits, block_params, {{CMD_SET_TYPE, set_type}});
                }   
            public:
                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                inline QMap<QString, QString> setsList() {
                    QMap<QString, QString> res;
                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

                    if (isConnected()) {
                        APPEND_SET_OPTION(res, cmd_tmpl, set_popular_tracks);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_popular_artists);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_popular_tuners);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_popular_collections);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_popular_albums);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_listened);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_downloaded);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_of_collections);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_of_radio);

                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_my);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_my_liked);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_my_history);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_my_subscriptions);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_week_top);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_new);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_popular);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_serial);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_tv_show);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_kino);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_bloggers);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_children);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_music);
                    }

                    return res;
                }

                inline QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
//                inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
            };                    
        }
    }
}

#endif // OD_SET
