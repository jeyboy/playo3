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

                //curl 'https://ok.ru/video/top?cmd=VideoVitrinaMain&st.cmd=userMain&st.vv_page=1&st.vv_ft=top&st.vpl.mini=false&gwt.requested=469d4b95' -X POST -H 'Host: ok.ru' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'DNT: 1' -H 'Content-Type: application/x-www-form-urlencoded' -H 'TKN: 7XLOcU6mrT-14FjdPoHVJCXvCuM8nkoX' -H 'Cookie: CDN=; bci=-586190216227621157; _flashVersion=20; AUTHCODE=X__rkHgxkeJ9R2RmnUvCj1piJRbA2L9LdISO_sp4xPjcjrhdHK8dw6oPfofjJWhuIWzEoDbTWVbijyND_blQppx58sTJ-9B3CAqh5UtzVJc_2; cudr=0; viewport=1040; TZD=6.-3756; TZ=6; TD=-3756; _fo4cl=1; CDN=; BANNER_LANG=ru; nbp=1; JSESSIONID=b8a6a422ceed30959655a87399b54f00c955feed4d905963.a674207f; LASTSRV=ok.ru'
                QJsonValue videoPlaylistByCategory(const VideoPlaylistType & pl_type, int offset = 1, int pages_limit = 5) {
                    QString vpl = vplToParam(pl_type);
                    // st.vv_aux_page=1
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_group, QStringLiteral("video/%1").arg(vpl),
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

                    return prepareBlock(dmt_video, cmd_mtd_groups_by_user, response, {{CMD_ID, QString::number(pl_type)}});
                }
            };
        }
    }
}

#endif // OD_VIDEO_PLAYLIST
