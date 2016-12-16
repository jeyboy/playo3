#ifndef OD_VIDEO
#define OD_VIDEO

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Video : public virtual Base {
            protected:
                enum VideoCategoryType : int {
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

                QString videoUrl(const QString & id) {
                    QString uid = id;

                    if (!Info::isNumber(uid))
                        Info::extractNumber(uid, uid);

                    QJsonArray videos = saRequest(
                        baseUrlStr(
                            qst_site_video, LSTR("video/%1").arg(uid),
                            {
                                { LSTR("cmd"),          LSTR("PopLayerVideo") },
                                { LSTR("st.cmd"),       LSTR("userMain") },
                                { LSTR("st.vpl.id"),    uid },
                                { LSTR("st.vpl.mi"),    LSTR("3") },
                                { LSTR("st.vpl.mini"),  const_false }
                            }
                        ),
                        call_type_html, 0, proc_video2, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())
                    );

                    if (!videos.isEmpty()) {
                        int size = videos.size();
                        int video_index = size / 2 + (size > 1 ? size % 2 : 0);
                        return videos[video_index].toObject().value(LSTR("url")).toString();
                    }

                    return QString();
                }

                QJsonValue videoVariants(Html::Tag * video_box) {
                    QString options = video_box -> data(LSTR("options"));
                    QJsonObject opts_json = QJsonDocument::fromJson(options.toUtf8()).object();
                    QString metadata = JSON_STR2(opts_json, LSTR("flashvars"), LSTR("metadata"));

                    QJsonObject metadata_json = QJsonDocument::fromJson(metadata.toUtf8()).object();
                    return JSON_ARR(metadata_json, LSTR("videos"));
                }

                QString vplToParam(const VideoCategoryType & vpl) {
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
                QString vplToString(const VideoCategoryType & vpl) {
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

                QJsonValue videoByCategory(const int & category_type, int pages_limit = 1, const QString & offset_token = QString(), const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QString vpl = vplToParam((VideoCategoryType)category_type);
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_video, LSTR("video/%1").arg(vpl),
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

                QJsonValue videoByPlaylist(const QString & album_id, int pages_limit = 1, const QString & offset_token = QString(), const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_video, LSTR("video/%1").arg(album_id),
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

                QJsonValue videoInfo(const QString & id) {
                    QJsonArray videos = saRequest(
                        baseUrlStr(
                            qst_site_video, LSTR("video/%1").arg(id),
                            {
                                { LSTR("cmd"),          LSTR("PopLayerVideo") },
                                { LSTR("st.cmd"),       LSTR("userMain") },
                                { LSTR("st.vpl.id"),    id },
                                { LSTR("st.vpl.mi"),    LSTR("3") },
                                { LSTR("st.vpl.mini"),  const_false }
                            }
                        ),
                        call_type_html, 0, proc_video4, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())
                    );

                    return videos;
                }

                QJsonValue videoSearch(const SearchLimit & limits, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_group, LSTR("search"),
                            {
                                { LSTR("cmd"),          LSTR("PortalSearchResults") },
                                { LSTR("st.cmd"),       LSTR("searchResult") },
                                { LSTR("st.mode"),      LSTR("Movie") },
                                { LSTR("st.grmode"),    LSTR("Groups") },
                                { LSTR("st.query"),     limits.predicate },
                                { LSTR("st.posted"),    LSTR("set") },

                                // mode // VideoCompilation // 	Live
//                                { LSTR("st.vmode"),    LSTR("VideoCompilation") },

                                // duration limitations // LONG // SHORT
//                                { LSTR("st.vln"),    LSTR("LONG") },

                                // high quality
                                //{ LSTR("st.vqt"),       LSTR("on") }
                            }
                        ),
                        call_type_html, pageRules(LSTR("st.page"), qMax(limits.start_offset, 1), limits.requests_limit), 0, proc_video3, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())
                    );

                    return prepareBlock(dmt_video, cmd_mtd_video_search, response, limits, block_params);
                }
            };
        }
    }
}

#endif // OD_VIDEO
