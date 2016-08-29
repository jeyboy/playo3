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
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt()
                    );
                }
                QJsonValue videoByCategory(const VideoPlaylistType & pl_type, int offset = 1, int pages_limit = 2) {
                    QString vpl = vplToParam(pl_type);
                    // st.vv_aux_page=1
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
                        call_type_html, pageRules(QStringLiteral("st.vv_page"), offset, pages_limit), 0, proc_video1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    QJsonObject obj = response.content.first().toObject();
                    return prepareBlock(
                        obj.contains(tkn_duration) ? dmt_video : dmt_video_set,
                        cmd_mtd_groups_by_user, response, {{CMD_ID, QString::number(pl_type)}}
                    );
                }

                QJsonValue videoByPlaylist(const QUrlQuery & args) {
                    return videoByPlaylist(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt()
                    );
                }
                //curl 'https://ok.ru/video/c872569?cmd=VideoVitrinaMain&st.cmd=userMain&st.vv_page=1&st.vv_ft=album&st.vv_aux_page=1&st.vv_albumId=c872569&st._aid=VideoVitrina_section_kino&st.vpl.mini=false&gwt.requested=469d4b95&p_sId=-4564355032148548588' -X POST -H 'Host: ok.ru' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Content-Type: application/x-www-form-urlencoded' -H 'TKN: S6LCw5rORjcRPXJdring9O6pYSeRfdPc' -H 'Referer: https://ok.ru/' -H 'Content-Length: 0' -H 'Cookie: CDN=; bci=-586190216227621157; _flashVersion=20; AUTHCODE=X__rkHgxkeJ9R2RmnUvCj9_WaMavx7ApsAHPti2CfQzp91KHwJR7kUiGOAXDzMrlP5R0bvHvgg_ibeTB3aa-im5ugR3yoWJksGf-LqKUT_Y_2; cudr=0; nbp=1; BANNER_LANG=ru; viewport=1040; TZD=6.-55; TZ=6; TD=-55; _fo4cl=1; CDN=; JSESSIONID=b44526891ddfab02445a8cb6a775431b9f125f229e72a567.d2bfb0e4; LASTSRV=ok.ru' -H 'Connection: keep-alive'
                QJsonValue videoByPlaylist(const QString & album_id, int offset = 1, int pages_limit = 2) {
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
                        call_type_html, pageRules(QStringLiteral("st.vv_page"), offset, pages_limit), 0, proc_video1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    QJsonObject obj = response.content.first().toObject();
                    return prepareBlock(
                        obj.contains(tkn_duration) ? dmt_video : dmt_video_set,
                        cmd_mtd_groups_by_user, response, {{CMD_ID, album_id}}
                    );
                }
            };
        }
    }
}

#endif // OD_VIDEO_PLAYLIST
