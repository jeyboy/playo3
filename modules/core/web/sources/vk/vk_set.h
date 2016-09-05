#ifndef VK_SET
#define VK_SET

#include "vk_defines.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Set : public Base {
            protected:
                enum SetType { set_popular_tracks = 1, set_popular_video };

                QMap<QString, QString> siteOptions() {
                    return {
                        { QStringLiteral("All Genres"),                 QStringLiteral("0") },
                        { QStringLiteral("Rock"),                       QStringLiteral("1") },
                        { QStringLiteral("Pop"),                        QStringLiteral("2") },
                        { QStringLiteral("Rap & Hip-Hop"),              QStringLiteral("3") },
                        { QStringLiteral("House & Dance"),              QStringLiteral("5") },
                        { QStringLiteral("Alternative"),                QStringLiteral("21") },
                        { QStringLiteral("Instrumental"),               QStringLiteral("6") },
                        { QStringLiteral("Easy Listening"),             QStringLiteral("4") },
                        { QStringLiteral("Metal"),                      QStringLiteral("7") },
                        { QStringLiteral("Dubstep & Trap"),             QStringLiteral("8") },
                        { QStringLiteral("Indie Pop"),                  QStringLiteral("17") },
                        { QStringLiteral("Jazz & Blues"),               QStringLiteral("1001") },
                        { QStringLiteral("Drum & Bass"),                QStringLiteral("10") },
                        { QStringLiteral("Trance"),                     QStringLiteral("11") },
                        { QStringLiteral("Ethnic"),                     QStringLiteral("13") },
                        { QStringLiteral("Acoustic & Vocal"),           QStringLiteral("14") },
                        { QStringLiteral("Reggae"),                     QStringLiteral("15") },
                        { QStringLiteral("Classical"),                  QStringLiteral("16") },
                        { QStringLiteral("Electropop & Disco"),         QStringLiteral("22") }
                    };
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) { // rewrite on using of offset
                    Permissions perm = permissions(pr_pack);
                    QJsonArray block_content;
                    QueriableResponse response;
                    DataMediaType dmt_val = dmt_unknow;

                    switch(perm) {
                        case perm_site: {
                            switch(set_type) {
                                case set_popular_tracks: {
                                    QString genre_id = (limits.in_foreign() ? QStringLiteral("foreign") : QString())
                                            % QString::number(0); // 0 is eql to all genres // limits.genre; // genres.toInt(attrs);

                                    Response * req_response = Manager::prepare() -> postFollowed(
                                        IQueriable::baseUrlStr(
                                            qst_site, QStringLiteral("al_audio.php"),
                                            {
                                                { QStringLiteral("act"), QStringLiteral("a_load_section") },
                                                { QStringLiteral("al"), QStringLiteral("1") },
                                                { QStringLiteral("album_id"), genre_id },
                                                { QStringLiteral("offset"), limits.start_offset },
                                                { QStringLiteral("owner_id"), userID(pr_recommendations) },
                                                { QStringLiteral("type"), QStringLiteral("popular") }
                                            }
                                        ),
                                        dntHeader()
                                    );

                                    QJsonArray items = RESPONSE_TO_JSON_OBJECT(req_response).value(QStringLiteral("list")).toArray();
                                    prepareTracks(items, block_content);
                                    response = QueriableResponse(block_content, QString::number(limits.start_offset + block_content.size()), 0, 1, block_content.isEmpty());
                                    dmt_val = dmt_audio;
                                break;}

                                case set_popular_video: {
                                    dmt_val = dmt_video;
                                break;}
                            }

                            return prepareBlock(dmt_val, cmd_mtd_set_by_type, response, limits, {{CMD_SET_TYPE, set_type}});
                        break;}

                        case perm_api: {
                            switch(set_type) {
                                case set_popular_tracks: {
                                    int genre_id = -1; //limits.genre; //genres.toInt(attrs);

                                    block_content = saRequest(
                                        baseUrlStr(
                                            qst_api, tkn_execute,
                                            {
                                                {
                                                    tkn_code, QString(
                                                       "var popular = API.audio.getPopular({"
                                                            "only_eng: " + boolToStr(limits.in_foreign())/*boolToStr(onlyEng)*/ + ", "
                                                            "count: 1000 " + (genre_id != -1 ? (", genre_id: " + QString::number(genre_id)) : "") + ""
                                                         "});"
                                                       "return {\"" % block_items_audio % "\": popular};"
                                                    )
                                                }
                                            }
                                        ),
                                        call_type_json, 0, proc_json_extract,
                                        IQUERY_DEF_FIELDS << block_items_audio
                                    );
                                    dmt_val = dmt_audio;
                                break;}

                                case set_popular_video: {
                                    dmt_val = dmt_video;
                                break;}
                            }
                        break;}

                        default: Logger::obj().write("VK", "SET BY TYPE is not accessable", true);
                    }

                    return prepareBlock(dmt_val, block_content/*cmd_mtd_set_by_type, response, {{CMD_SET_TYPE, set_type}}*/);
                }

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_tracks: return QStringLiteral("Popular Tracks: ");
                        case set_popular_video: return QStringLiteral("Popular Video");
                        default: return QStringLiteral("Unknown: ");
                    }
                }
            public:
                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QMap<QString, QString> setsList() {
                    QMap<QString, QString> res;
                    QMap<QString, QString> opts = siteOptions();

                    QString popular_title = setTypeToStr(set_popular_tracks);
                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
                        res.insert(
                            popular_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_popular_tracks) },
                                    { CMD_GENRE, opt.value() }
                                }
                            ) -> toString()
                        );
                    }

                    return res;
                }

                inline QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
//                inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
            };
        }
    }
}

#endif // VK_SET
