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
                        case vpl_my_video: return QStringLiteral("myVideo");
                        case vpl_my_liked: return QStringLiteral("myLiked");
                        case vpl_my_history: return QStringLiteral("history");
                        case vpl_my_subscriptions: return QStringLiteral("subscriptions");
                        case vpl_top: return QStringLiteral("top");
                        case vpl_new: return QStringLiteral("new");
                        case vpl_popular: return QStringLiteral("suggestedAlbums");
                        case vpl_serial: return QStringLiteral("serial");
                        case vpl_tvshow: return QStringLiteral("tvshow");
                        case vpl_kino: return QStringLiteral("kino");
                        case vpl_bloggers: return QStringLiteral("bloggers");
                        case vpl_children: return QStringLiteral("children");
                        case vpl_musicchannel: return QStringLiteral("musicchannel");

                        default: return QString();
                    }
                }
                QString vplToString(const VideoPlaylistType & vpl) {
                    switch(vpl) {
                        case vpl_my_video: return QStringLiteral("My Video");
                        case vpl_my_liked: return QStringLiteral("My Liked Video");
                        case vpl_my_history: return QStringLiteral("My Video History");
                        case vpl_my_subscriptions: return QStringLiteral("My Video Subscriptions");
                        case vpl_top: return QStringLiteral("Top video of the week");
                        case vpl_new: return QStringLiteral("New Video");
                        case vpl_popular: return QStringLiteral("Popular Video");
                        case vpl_serial: return QStringLiteral("Serials");
                        case vpl_tvshow: return QStringLiteral("TV shows");
                        case vpl_kino: return QStringLiteral("Moovies");
                        case vpl_bloggers: return QStringLiteral("Bloggers");
                        case vpl_children: return QStringLiteral("Children");
                        case vpl_musicchannel: return QStringLiteral("Music Video");

                        default: return QStringLiteral("Unknow");
                    }
                }

                QJsonValue videoByCategory(const QUrlQuery & args) {
                    return videoByCategory(
                        (VideoPlaylistType)args.queryItemValue(CMD_ID).toInt(),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt(),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoByCategory(const VideoPlaylistType & pl_type, int pages_limit = 1, const QString & offset_token = QString()) {
                    QString vpl = vplToParam(pl_type);
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_video, QStringLiteral("video/%1").arg(vpl),
                            {
                                { QStringLiteral("cmd"), QStringLiteral("VideoVitrinaMain") },
                                { QStringLiteral("st.cmd"), QStringLiteral("userMain") },
                                { QStringLiteral("st.vpl.mini"), QStringLiteral("false") },
                                { QStringLiteral("st.vv_ft"), vpl }
                            }
                        ),
                        call_type_html, tokenRules(QStringLiteral("st.lastelem"), offset_token, pages_limit), 0, proc_video1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    QJsonObject obj = response.content.first().toObject();
                    return prepareBlock(
                        obj.contains(tkn_duration) ? dmt_video : dmt_video_set,
                        cmd_mtd_video_by_category, response, {{CMD_ID, QString::number(pl_type)}}
                    );
                }

                QJsonValue videoByPlaylist(const QUrlQuery & args) {
                    return videoByPlaylist(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt(),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoByPlaylist(const QString & album_id, int pages_limit = 1, const QString & offset_token = QString()) {
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_video, QStringLiteral("video/%1").arg(album_id),
                            {
                                { QStringLiteral("cmd"), QStringLiteral("VideoVitrinaMain") },
                                { QStringLiteral("st.cmd"), QStringLiteral("userMain") },
                                { QStringLiteral("st.vpl.mini"), QStringLiteral("false") },
                                { QStringLiteral("st.vv_albumId"), album_id },
                                { QStringLiteral("st.vv_ft"), QStringLiteral("album") }
                            }
                        ),
                        call_type_html, tokenRules(QStringLiteral("st.lastelem"), offset_token, pages_limit), 0, proc_video1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    QJsonObject obj = response.content.first().toObject();
                    return prepareBlock(
                        obj.contains(tkn_duration) ? dmt_video : dmt_video_set,
                        cmd_mtd_video_by_playlist, response, {{CMD_ID, album_id}}
                    );
                }
            };
        }
    }
}

#endif // OD_VIDEO_PLAYLIST
