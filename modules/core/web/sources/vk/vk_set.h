#ifndef VK_SET
#define VK_SET

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Set : public Base {
            public:
                enum SetType { set_popular_tracks = 1, set_popular_video };

                QJsonValue setByType(const SetType & set_type, const QString & attrs) {
                    Permissions perm = permissions(pr_media_content);
                    QStringList params = attrs.split('|', QString::SkipEmptyParts);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            switch(set_type) {
                                case set_popular_tracks: {
                                    int genre_id = -1; //params.last(); //genres.toInt(attrs);

                                    return saRequest(
                                        baseUrlStr(
                                            qst_api_def, tkn_execute,
                                            {
                                                {
                                                    tkn_code, QString(
                                                       "var popular = API.audio.getPopular({"
                                                            "only_eng: " + params.first()/*boolToStr(onlyEng)*/ + ", "
                                                            "count: 1000 " + (genre_id != -1 ? (", genre_id: " + QString::number(genre_id)) : "") + ""
                                                         "});"
                                                       "return {" % tkn_audio_list % ": popular};"
                                                    )
                                                }
                                            }
                                        ),
                                        call_type_json, 0, proc_json_extract,
                                        IQUERY_DEF_FIELDS << tkn_audio_list
                                    );
                                break;}

                                case set_popular_video: {

                                break;}
                            }
                        }

                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // VK_SET
