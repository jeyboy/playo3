#ifndef OD_USER
#define OD_USER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class User : public virtual Base {
            public:
                QJsonValue userInfo() {
                    return sRequest(
                        audioUrlStr(path_audio_init),
                        call_type_json
                    );
                }

                QJsonValue userMedia(const QString & user_id) {
                    return sRequest(
                        audioUrlStr(
                            tkn_my,
                            {{tkn_uid, user_id}}
                        ),
                        call_type_json
                    );
                }

                QJsonValue usersByName(const QString & name) {
//                    st._aid
//                    st.army
//                    st.bthDay
//                    st.bthMonth
//                    st.bthYear
//                    st.city
//                    st.cmode
//                    st.colleage
//                    st.country
//                    st.fromAge
//                    st.gcountry
//                    st.gender f
//                    st.glocation
//                    st.grmode Groups
//                    st.holiday
//                    st.location
//                    st.mode	Users
//                    st.official
//                    st.onSite
//                    st.open
//                    st.page
//                    st.posted set
//                    st.query po
//                    st.refs
//                    st.school
//                    st.single
//                    st.tag on
//                    st.tillAge
//                    st.university
//                    st.vpl.mini	false
//                    st.workplace

                    return paRequest(
                        baseUrlStr(
                            qst_site_user, QStringLiteral("search"),
                            {
                                { QStringLiteral("cmd"), QStringLiteral("PortalSearchResults") },
                                { QStringLiteral("st.cmd"), QStringLiteral("searchResult") },
                                { QStringLiteral("st.mode"), QStringLiteral("Users") },
                                { QStringLiteral("st.vpl.mini"), QStringLiteral("false") },
                                { QStringLiteral("st.grmode"), QStringLiteral("Groups") },
                                { QStringLiteral("st.posted"), QStringLiteral("set") },
                                { QStringLiteral("st.query"), name },
                                { QStringLiteral("fetch"), QStringLiteral("false") },
                            }
                        ),
                        call_type_html, pageRules(QStringLiteral("st.page")), 0, proc_user1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );
                }

                QJsonValue usersById(const QString & user_id) {
                    return saRequest(
                        baseUrlStr(qst_site, QStringLiteral("profile/") % user_id, {}),
                        call_type_html, 0, proc_user2
                    );
                }
            };
        }
    }
}

#endif // OD_USER
