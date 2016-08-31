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
                            qst_site_video, QStringLiteral("video/%1").arg(uid),
                            {
                                { QStringLiteral("cmd"), QStringLiteral("PopLayerVideo") },
                                { QStringLiteral("st.cmd"), QStringLiteral("userMain") },
                                { QStringLiteral("st.vpl.id"), uid },
                                { QStringLiteral("st.vpl.mi"), QStringLiteral("3") },
                                { QStringLiteral("st.vpl.mini"), QStringLiteral("false") }
                            }
                        ),
                        call_type_html, 0, proc_video2, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())
                    );

                    if (!videos.isEmpty()) {
                        int size = videos.size();
                        int video_index = size / 2 + (size > 1 ? size % 2 : 0);
                        return videos[video_index].toObject().value(QStringLiteral("url")).toString();
                    }

                    return QString();
                }

                QJsonValue videoSearch(const SearchLimit & limits) {
                    QueriableResponse response;

                    //TODO: finish me

                    return prepareBlock(dmt_video, cmd_mtd_video_search, response, limits);
                }
            };
        }
    }
}

#endif // OD_VIDEO
