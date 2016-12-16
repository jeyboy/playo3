#ifndef OD_GROUP
#define OD_GROUP

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Group : public virtual Base {
            public:
                QJsonValue groupsByUser(const QString & user_id, int offset = 1, int pages_limit = 5, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QueriableResponse response = pRequest(
                                baseUrlStr(
                                    qst_site_group, LSTR("profile/%1/groups/mine").arg(user_id),
                                    {
                                        { LSTR("st.cmd"), LSTR("userGroups") },
                                        { LSTR("st.vpl.mini"), const_false },
                                    }
                                ),
                        call_type_html, pageRules(LSTR("st.page"), offset, pages_limit), 0, proc_group1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_user, response, block_params, {{CMD_ID, user_id}});
                }

                QJsonValue groupsByName(const QString & name, int offset = 1, int pages_limit = 5, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
//                    dirty	1
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
//                    st.gender
//                    st.glocation
//                    st.grmode	Groups
//                    st.holiday
//                    st.location
//                    st.mode Groups
//                    st.official
//                    st.onSite
//                    st.open
//                    st.page
//                    st.posted	set
//                    st.query sos
//                    st.refs
//                    st.school
//                    st.single
//                    st.tillAge
//                    st.university
//                    st.vpl.mini false
//                    st.workplace
                    //st.tag on

                    QueriableResponse response = pRequest(
                        baseUrlStr(
                            qst_site_group, LSTR("search"),
                            {
                                { LSTR("cmd"),          LSTR("PortalSearchResults") },
                                { LSTR("st.cmd"),       LSTR("searchResult") },
                                { LSTR("st.mode"),      LSTR("Groups") },
                                { LSTR("st.vpl.mini"),  const_false },
                                { LSTR("st.grmode"),    LSTR("Groups") },
                                { LSTR("st.posted"),    LSTR("set") },
                                { LSTR("st.query"),     name },
                                { LSTR("fetch"),        const_false }
                            }
                        ),
                        call_type_html, pageRules(LSTR("st.page"), offset, pages_limit), 0, proc_group2, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_name, response, block_params, {{CMD_PREDICATE, name}});
                }

                QJsonValue groupsByIdOrPerma(const QString & group_id, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QString url = Info::isNumber(group_id) ?
                        baseUrlStr(qst_site, LSTR("group/") % group_id, {}) :
                        baseUrlStr(qst_site, group_id, {});

                    QJsonArray block_content = saRequest(url, call_type_html, 0, proc_group3);
                    return prepareBlock(dmt_group, block_content, block_params);
                }
            };
        }
    }
}

#endif // OD_GROUP
