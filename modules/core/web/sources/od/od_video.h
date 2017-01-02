#ifndef OD_VIDEO
#define OD_VIDEO

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Video : public virtual Base {
            protected:
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

                QJsonValue videoInfo(const QUrlQuery & args) { return videoInfo(args.queryItemValue(CMD_ID)); }
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

                QJsonValue videoSearch(const QUrlQuery & args) {
                    return videoSearch(SearchLimit::fromICmdParams(args));
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
                        call_type_html, pageRules(LSTR("st.page"), qMax(limits.start_offset.toInt(), 1), limits.requests_limit), 0, proc_video3, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())
                    );

                    return prepareBlock(dmt_video, cmd_mtd_video_search, response, limits, block_params);
                }
            };
        }
    }
}

#endif // OD_VIDEO
