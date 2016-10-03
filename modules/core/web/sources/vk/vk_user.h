#ifndef VK_USER
#define VK_USER

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class User : public Base {
            public:
                QJsonObject userStatus(const QString & user_id) { // deactivated: 'deleted', id, first_name, last_name, counters
                    SourceFlags perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            QUrlQuery query;
                            setParam(query, tkn_fields, tkn_counters);
                            setParam(query, tkn_user_ids, user_id);

                            QJsonObject ret = sRequest(baseUrlStr(qst_api, path_user_info, query), call_type_json);
                            return ret.value(tkn_response).toArray().first().toObject();
                        }

                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", Logger::log_error);
                    }

                    return QJsonObject();
                }

                QJsonValue usersByIdsOrPermas(const QStringList & ids) {
                    return usersByIdOrPerma(ids.join(QStringLiteral(",")));
                }
                QJsonValue usersById(const QUrlQuery & args) {
                    return usersByIdOrPerma(
                        args.queryItemValue(CMD_ID)/*,
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()*/
                    );
                }
                QJsonValue usersByIdOrPerma(const QString & id) {
                    SourceFlags perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            block_content = saRequest(
                                baseUrlStr(
                                    qst_api, path_user_info,
                                    {
                                        { tkn_user_ids, id },
                                        { tkn_fields, val_user_fields }
                                    }
                                ),
                                call_type_json, 0, proc_json_extract
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_user, block_content);
                }

                QJsonValue usersByName(const QUrlQuery & args) {
                    return usersByName(
                        args.queryItemValue(CMD_PREDICATE),
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }
                QJsonValue usersByName(const QString & uname, int offset = 0, int count = 100) {
                    SourceFlags perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            response = pRequest(
                                baseUrlStr(
                                    qst_api, path_users_search,
                                    {
                                        { tkn_q, uname },
                                        { tkn_fields, val_user_fields },
                                        { tkn_limit, count },
                                        { tkn_offset, OFFSET_TEMPLATE }
                                    }
                                ),
                                call_type_json, rules(offset, count), 0, proc_json_extract, QStringList() << tkn_response << tkn_items
                            );
                        break;}

                        default: Logger::obj().write(name(), "GROUP INFO is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_users_by_name, response, {}, {{CMD_PREDICATE, name}});
                }
            };
        }
    }
}

#endif // VK_USER
