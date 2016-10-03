#ifndef VK_GROUP
#define VK_GROUP

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Group : public Base {
            public:

                QJsonValue groupsById(const QUrlQuery & args) {
                    return groupsByIdOrPerma(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue groupsByIdOrPermas(const QStringList & ids) {
                    return groupsByIdOrPerma(ids.join(QStringLiteral(",")));
                }
                QJsonValue groupsByIdOrPerma(const QString & id, int offset = 0, int count = 100) {
                    SourceFlags perm = permissions(sf_group_by_id);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
//                            block_content = saRequest(
//                                baseUrlStr(
//                                    qst_api, path_groups_by_id,
//                                    { { tkn_group_ids, id } }
//                                ),
//                                call_type_json, 0, proc_json_extract
//                            );

                            response = pRequest(
                                baseUrlStr(
                                    qst_api, path_groups_by_id,
                                    { { tkn_group_ids, id }, { tkn_offset, OFFSET_TEMPLATE } }
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_extract
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_id, response, {}, {{CMD_ID, id}});
                }

                QJsonValue groupsByName(const QUrlQuery & args) {
                    return groupsByName(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue groupsByName(const QString & gname, int offset = 0, int count = 100) {
                    SourceFlags perm = permissions(sf_group_by_title);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
//                            return saRequest(
//                                baseUrlStr(
//                                    qst_api, path_groups_search,
//                                    {
//                                        { tkn_type, val_group_types },
//                                        { tkn_q, name },
//                                        { QStringLiteral("count"), count }
//                                    }
//                                ),
//                                call_type_json, 0, proc_json_extract,
//                                QStringList() << tkn_response << tkn_items
//                            );

                            response = pRequest(
                                baseUrlStr(
                                    qst_api, path_groups_search,
                                    {
                                        { tkn_type, val_group_types },
                                        { tkn_q, gname },
                                        { tkn_limit, 100 },
                                        { tkn_offset, OFFSET_TEMPLATE }
                                    }
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_extract,
                                QStringList() << tkn_response << tkn_items
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_group, cmd_mtd_groups_by_name, response, {}, {{CMD_PREDICATE, name}});
                }
            };
        }
    }
}

#endif // VK_GROUP
