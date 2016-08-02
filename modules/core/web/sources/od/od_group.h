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
                }

                QJsonValue groupsByName(const QString & name) {
                    return saRequest(
                        baseUrlStr(
                            qst_site, QStringLiteral("groups/search"),
                            {
                                { QStringLiteral("st.cmd"), QStringLiteral("userGroupsSearch") },
                                { QStringLiteral("cmd"), QStringLiteral("UserGroupsMainBlock") },
                                { QStringLiteral("st.posted"), QStringLiteral("set") },
                                { QStringLiteral("st.query"), name }
                            }
                        ),
                        call_type_html, 0, proc_group2, QStringList(), call_method_post,
                        tknHeaders().unite(dntHeader())
                    );
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
