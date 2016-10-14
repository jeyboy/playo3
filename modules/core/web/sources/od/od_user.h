#ifndef OD_USER
#define OD_USER

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class User : public virtual Base {
            public:
                QJsonValue userInfo() {
                    QJsonObject res = sRequest(
                        audioUrlStr(path_audio_init),
                        call_type_json
                    );

                    QJsonArray tracks = res.value(tkn_tracks).toArray();
                    if (tracks.isEmpty()) {
                        int totalTracks = res.value(LSTR("totalTracks")).toInt();
                        if (totalTracks > 0) {
                            QJsonObject media = userMedia(JSON_CSTR(res, tkn_me)).toObject();
                            tracks = JSON_ARR(media, tkn_tracks);
                            res.insert(tkn_tracks, tracks);
                        }
                    }

                    return res;
                }

                QJsonValue userMedia(const QString & user_id) {
                    return sRequest(
                        audioUrlStr(tkn_my, {{tkn_uid, user_id}}),
                        call_type_json
                    );
                }

                QJsonValue usersByName(const QString & name, int offset = 1, int pages_limit = 5, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
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

                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_user, LSTR("search"),
                            {
                                { LSTR("cmd"),          LSTR("PortalSearchResults") },
                                { LSTR("st.cmd"),       LSTR("searchResult") },
                                { LSTR("st.mode"),      LSTR("Users") },
                                { LSTR("st.vpl.mini"),  LSTR("false") },
                                { LSTR("st.grmode"),    LSTR("Groups") },
                                { LSTR("st.posted"),    LSTR("set") },
                                { LSTR("st.query"),     name },
                                { LSTR("fetch"),        LSTR("false") },
                            }
                        ),
                        call_type_html, pageRules(LSTR("st.page"), offset, pages_limit), 0, proc_user1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    return prepareBlock(dmt_user, cmd_mtd_users_by_name, response, block_params, {{CMD_PREDICATE, name}});
                }

                QJsonValue usersById(const QString & user_id, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QJsonArray block_content = saRequest(
                        baseUrlStr(qst_site, LSTR("profile/") % user_id, {}),
                        call_type_html, 0, proc_user2
                    );

                    return prepareBlock(dmt_user, block_content, block_params);
                }
            };
        }
    }
}

#endif // OD_USER
