#ifndef OD_GROUP
#define OD_GROUP

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Group : public virtual Base {
            public:
                QJsonValue groupsByName(const QUrlQuery & args) {
                    return groupsByUser(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt()
                    );
                }

                QJsonValue groupsByUser(const QString & user_id, int offset = 1, int pages_limit = 5) {
                    QueriableResponse response = pRequest(
                                baseUrlStr(
                                    qst_site_group, QStringLiteral("profile/%1/groups/mine").arg(user_id),
                                    {
                                        { QStringLiteral("st.cmd"), QStringLiteral("userGroups") },
                                        { QStringLiteral("st._aid"), QStringLiteral("GroupsSubMenu_User_MyGroupsNav_All") },
                                        { QStringLiteral("st.vpl.mini"), QStringLiteral("false") },
                                    }
                                ),
                        call_type_html, pageRules(QStringLiteral("st.page"), offset, pages_limit), 0, proc_group1, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    return prepareBlock(dmt_groups, block_content, response, {{CMD_ID, user_id}});
                }

                QJsonValue groupsByName(const QUrlQuery & args) {
                    return groupsByName(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_REQUESTS_LIMIT).toInt()
                    );
                }

                QJsonValue groupsByName(const QString & name, int offset = 1, int pages_limit = 5) {
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
                            qst_site_group, QStringLiteral("search"),
                            {
                                { QStringLiteral("cmd"), QStringLiteral("PortalSearchResults") },
                                { QStringLiteral("st.cmd"), QStringLiteral("searchResult") },
                                { QStringLiteral("st.mode"), QStringLiteral("Groups") },
                                { QStringLiteral("st.vpl.mini"), QStringLiteral("false") },
                                { QStringLiteral("st.grmode"), QStringLiteral("Groups") },
                                { QStringLiteral("st.posted"), QStringLiteral("set") },
                                { QStringLiteral("st.query"),  name },
                                { QStringLiteral("fetch"), QStringLiteral("false") }
                            }
                        ),
                        call_type_html, pageRules(QStringLiteral("st.page"), offset, pages_limit), 0, proc_group2, QStringList(),
                        call_method_post, tknHeaders().unite(dntHeader())

                    );

                    return prepareBlock(dmt_groups, cmd_mtd_groups_by_name, response, {{CMD_PREDICATE, name}});
                }

                QJsonValue groupsByIdOrPerma(const QString & group_id) {
                    QString url = Info::isNumber(group_id) ?
                        baseUrlStr(qst_site, QStringLiteral("group/") % group_id, {}) :
                        baseUrlStr(qst_site, group_id, {});

                    return saRequest(url, call_type_html, 0, proc_group3);
                }
            };
        }
    }
}

#endif // OD_GROUP
