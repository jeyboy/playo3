#ifndef OD_GROUP
#define OD_GROUP

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Group : public Base {
            public:
                QJsonValue groupsByUser(const QString & user_id) {



                    //curl 'https://ok.ru/profile/511343312018/groups/mine?st.cmd=userGroups&st._aid=GroupsSubMenu_User_MyGroupsNav_All&st.vpl.mini=false&gwt.requested=aa292914' -X POST  -H 'Content-Type: application/x-www-form-urlencoded' -H 'Cookie: bci=-586190216227621157; _flashVersion=20; AUTHCODE=X__rkHgxkeJ9R2RmnUvCj_ZFji9QaRuM0UXT2TaSdTKcrPNVLZm-cjKbajzfRMnVEHUz_VIZ8rDjZ44YXcCEnKjVWXHoiUCsMt4lKPtjalU_2; viewport=1040; TZD=6.-478; TZ=6; TD=-478; _fo4cl=1; CDN=; BANNER_LANG=ru; JSESSIONID=b61b244091e9aef4bf8cf76fc4da49b43ca070b6745a00a6.61c1406b; LASTSRV=ok.ru; cudr=0' -H 'DNT: 1' -H 'Host: ok.ru' -H 'Referer: https://ok.ru' -H 'TKN: xNzn0croooQiNecKI0RxBMzq_Qxg_Gcm' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0'

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

//                    return QJsonArray();
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
