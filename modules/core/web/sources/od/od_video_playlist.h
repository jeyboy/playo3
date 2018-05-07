#ifndef OD_VIDEO_PLAYLIST
#define OD_VIDEO_PLAYLIST

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class VideoPlaylist : public virtual Base {
            protected:
                enum VideoPlaylistType : int {
                    vpl_unknow = 0,
                    vpl_my_video = 1000,
                    vpl_my_liked,
                    vpl_my_history,
                    vpl_my_subscriptions,
                    vpl_top,
                    vpl_new,
                    vpl_popular,
                    vpl_serial,
                    vpl_tvshow,
                    vpl_kino,
                    vpl_bloggers,
                    vpl_children,
                    vpl_musicchannel
                };

                QString vplToParam(const VideoPlaylistType & vpl) {
                    switch(vpl) {
                        case vpl_my_video:              return LSTR("myVideo");
                        case vpl_my_liked:              return LSTR("myLiked");
                        case vpl_my_history:            return LSTR("history");
                        case vpl_my_subscriptions:      return LSTR("subscriptions");
                        case vpl_top:                   return LSTR("top");
                        case vpl_new:                   return LSTR("new");
                        case vpl_popular:               return LSTR("suggestedAlbums");
                        case vpl_serial:                return LSTR("serial");
                        case vpl_tvshow:                return LSTR("tvshow");
                        case vpl_kino:                  return LSTR("kino");
                        case vpl_bloggers:              return LSTR("bloggers");
                        case vpl_children:              return LSTR("children");
                        case vpl_musicchannel:          return LSTR("musicchannel");

                        default:                        return QString();
                    }
                }
                QString vplToString(const VideoPlaylistType & vpl) {
                    switch(vpl) {
                        case vpl_my_video:              return LSTR("My Video");
                        case vpl_my_liked:              return LSTR("My Liked Video");
                        case vpl_my_history:            return LSTR("My Video History");
                        case vpl_my_subscriptions:      return LSTR("My Video Subscriptions");
                        case vpl_top:                   return LSTR("Top video of the week");
                        case vpl_new:                   return LSTR("New Video");
                        case vpl_popular:               return LSTR("Popular Video");
                        case vpl_serial:                return LSTR("Serials");
                        case vpl_tvshow:                return LSTR("TV shows");
                        case vpl_kino:                  return LSTR("Moovies");
                        case vpl_bloggers:              return LSTR("Bloggers");
                        case vpl_children:              return LSTR("Children");
                        case vpl_musicchannel:          return LSTR("Music Video");

                        default:                        return LSTR("Unknow");
                    }
                }

                QJsonValue videoByCategory(const QUrlQuery & args) {
                    return videoByCategory(
                        (VideoPlaylistType)args.queryItemValue(CMD_ID).toInt(),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt(),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoByCategory(const VideoPlaylistType & pl_type, int pages_limit = 1, const QString & offset_token = QString(), const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QString vpl = vplToParam(pl_type);
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_video, QStringLiteral("video/%1").arg(vpl),
                            {
                                { LSTR("cmd"),          LSTR("VideoVitrinaMain") },
                                { LSTR("st.cmd"),       LSTR("userMain") },
                                { LSTR("st.vpl.mini"),  const_false },
                                { LSTR("st.vv_ft"),     vpl }
                            }
                        ),
                        call_type_html, tokenRules(LSTR("st.lastelem"), offset_token, pages_limit), 0, proc_video1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    QJsonObject obj = response.content.first().toObject();
                    return prepareBlock(
                        obj.contains(tkn_duration) ? dmt_video : dmt_video_set,
                        cmd_mtd_video_by_category, response, block_params, {{CMD_ID, QString::number(pl_type)}}
                    );
                }

                QJsonValue videoByPlaylist(const QUrlQuery & args) {
                    return videoByPlaylist(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt(),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoByPlaylist(const QString & album_id, int pages_limit = 1, const QString & offset_token = QString(), const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_video, QStringLiteral("video/%1").arg(album_id),
                            {
                                { LSTR("cmd"),              LSTR("VideoVitrinaMain") },
                                { LSTR("st.cmd"),           LSTR("userMain") },
                                { LSTR("st.vpl.mini"),      const_false },
                                { LSTR("st.vv_albumId"),    album_id },
                                { LSTR("st.vv_ft"),         LSTR("album") }
                            }
                        ),
                        call_type_html, tokenRules(LSTR("st.lastelem"), offset_token, pages_limit), 0, proc_video1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    QJsonObject obj = response.content.first().toObject();
                    return prepareBlock(
                        obj.contains(tkn_duration) ? dmt_video : dmt_video_set,
                        cmd_mtd_video_by_playlist, response, block_params, {{CMD_ID, album_id}}
                    );
                }
            };
        }
    }
}

#endif // OD_VIDEO_PLAYLIST
