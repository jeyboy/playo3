#ifndef OD_GROUP
#define OD_GROUP

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Group : public Base {
            public:
                QJsonValue groupsByUser(const QString & user_id) {
                    return saRequest(
                        baseUrlStr(
                            qst_site, QStringLiteral("profile/%1/groups/mine").arg(user_id),
                            {
                                { QStringLiteral("st.cmd"), QStringLiteral("userGroups") },
                                { QStringLiteral("st._aid"), QStringLiteral("GroupsSubMenu_User_MyGroupsNav_All") },
                                { QStringLiteral("st.vpl.mini"), QStringLiteral("false") },
                            }
                        ),
                        call_type_html, 0, proc_group1, QStringList(), call_method_post,
                        tknHeaders().unite(dntHeader())
                    );

                    //-X POST  -H 'Content-Type: application/x-www-form-urlencoded' -H 'Cookie: bci=-586190216227621157; _flashVersion=20; AUTHCODE=X__rkHgxkeJ9R2RmnUvCj_ZFji9QaRuM0UXT2TaSdTKcrPNVLZm-cjKbajzfRMnVEHUz_VIZ8rDjZ44YXcCEnKjVWXHoiUCsMt4lKPtjalU_2; viewport=1040; TZD=6.-478; TZ=6; TD=-478; _fo4cl=1; CDN=; BANNER_LANG=ru; JSESSIONID=b61b244091e9aef4bf8cf76fc4da49b43ca070b6745a00a6.61c1406b; LASTSRV=ok.ru; cudr=0' -H 'DNT: 1' -H 'Host: ok.ru' -H 'Referer: https://ok.ru' -H 'TKN: xNzn0croooQiNecKI0RxBMzq_Qxg_Gcm'

//                    Permissions perm = permissions(pr_media_content);

//                    switch(perm) {
//                        case perm_site:
//                        case perm_api: {
//                            return saRequest(
//                                baseUrlStr(
//                                    qst_api_def, path_groups_search,
//                                    {
//                                        { tkn_type, val_group_types },
//                                        { tkn_q, name },
//                                        { QStringLiteral("count"), 100/*0*/ }
//                                    }
//                                ),
//                                call_type_json, 0, proc_json_extract,
//                                QStringList() << tkn_response << tkn_items
//                            );
//                        }

//                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
//                    }

//                    return QJsonArray();
                }

                QJsonValue groupsByName(const QString & name) {
//                    Permissions perm = permissions(pr_media_content);

//                    switch(perm) {
//                        case perm_site:
//                        case perm_api: {
//                            return saRequest(
//                                baseUrlStr(
//                                    qst_api_def, path_groups_search,
//                                    {
//                                        { tkn_type, val_group_types },
//                                        { tkn_q, name },
//                                        { QStringLiteral("count"), 100/*0*/ }
//                                    }
//                                ),
//                                call_type_json, 0, proc_json_extract,
//                                QStringList() << tkn_response << tkn_items
//                            );
//                        }

//                        default: Logger::obj().write("VK", "GROUP INFO is not accessable", true);
//                    }

                    return QJsonArray();
                }

//                QJsonValue userInfo() {
//                    return sRequest(
//                        audioUrlStr(path_audio_init),
//                        call_type_json
//                    );
//                }

//                QJsonValue userMedia(const QString & user_id) {
//                    return sRequest(
//                        audioUrlStr(
//                            tkn_my,
//                            {
//                                {tkn_uid, user_id}
//                            }
//                        ),
//                        call_type_json
//                    );
//                }

//                QJsonValue popular(const SearchLimit & /*limits*/) {
//                    return pRequest(
//                        popAudioUrl(),
//                        call_type_json,
//                        rules(),
//                        0,
//                        proc_json_extract,
//                        QStringList() << tkn_tracks
//                    );

////                    return lQuery(popAudioUrl(), QueryRules(tkn_tracks, requestLimit(), OD_SEARCH_LIMIT));
//                }
            };
        }
    }
}

#endif // OD_GROUP
