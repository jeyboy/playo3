#ifndef VK_SET
#define VK_SET

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Set : public Base {
            public:
                enum SetType { set_popular_tracks = 1, set_popular_video };

                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            switch(set_type) {
                                case set_popular_tracks: {
                                    int genre_id = -1; //limits.genre; //genres.toInt(attrs);

                                    return saRequest(
                                        baseUrlStr(
                                            qst_api_def, tkn_execute,
                                            {
                                                {
                                                    tkn_code, QString(
                                                       "var popular = API.audio.getPopular({"
                                                            "only_eng: " + boolToStr(limits.in_foreign())/*boolToStr(onlyEng)*/ + ", "
                                                            "count: 1000 " + (genre_id != -1 ? (", genre_id: " + QString::number(genre_id)) : "") + ""
                                                         "});"
                                                       "return {" % block_items_audio % ": popular};"
                                                    )
                                                }
                                            }
                                        ),
                                        call_type_json, 0, proc_json_extract,
                                        IQUERY_DEF_FIELDS << block_items_audio
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
