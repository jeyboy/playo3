#ifndef VK_SET
#define VK_SET

#include "vk_defines.h"
#include "vk_video.h"

namespace Core {
    namespace Web {
        namespace Vk {
            class Set : public Video {
            protected:
                enum SetType { set_popular_tracks = 1, set_popular_video, set_feed_video, set_top_video, set_series_video, set_other_video };
                QMap<QString, QString> audioSetOptions() {
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
                    SourceFlags perm = permissions(sf_compilation);
                    QJsonArray block_content;

                    switch(set_type) {
                        case set_popular_tracks: {
                            switch(perm) {
                                case sf_site: {
                                    QString genre_id = (limits.in_foreign() ? QStringLiteral("foreign") : QString())
                                            % limits.genre; // 0 is eql to all genres

                                    Response * req_response = Manager::prepare() -> postFollowed(
                                        IQueriable::baseUrlStr(
                                            qst_site, QStringLiteral("al_audio.php"),
                                            {
                                                { QStringLiteral("act"), QStringLiteral("a_load_section") },
                                                { QStringLiteral("al"), QStringLiteral("1") },
                                                { QStringLiteral("album_id"), genre_id },
                                                { QStringLiteral("offset"), limits.start_offset },
                                                { QStringLiteral("owner_id"), userID() },
                                                { QStringLiteral("type"), QStringLiteral("popular") }
                                            }
                                        ),
                                        dntHeader()
                                    );

                                    QJsonArray items = RESPONSE_TO_JSON_OBJECT(req_response).value(QStringLiteral("list")).toArray();
                                    prepareTracks(items, block_content);
                                    QueriableResponse response = QueriableResponse(block_content, QString::number(limits.start_offset + block_content.size()), 0, 1, block_content.isEmpty());
                                    return prepareBlock(dmt_audio, cmd_mtd_set_by_type, response, limits, {}, {{CMD_SET_TYPE, set_type}});
                                break;}

                                case sf_api: {
                                    QString genre_id = limits.genre;

                                    block_content = saRequest(
                                        baseUrlStr(
                                            qst_api, tkn_execute,
                                            {
                                                {
                                                    tkn_code, QString(
                                                       "var popular = API.audio.getPopular({"
                                                            "only_eng: " + boolToStr(limits.in_foreign())/*boolToStr(onlyEng)*/ + ", "
                                                            "count: 1000 " % (!genre_id.isEmpty() ? (", genre_id: " + genre_id) : "") + ""
                                                         "});"
                                                       "return {\"" % block_items_audio % "\": popular};"
                                                    )
                                                }
                                            }
                                        ),
                                        call_type_json, 0, proc_json_extract,
                                        IQUERY_DEF_FIELDS << block_items_audio
                                    );

                                    prepareBlock(dmt_audio, block_content/*,cmd_mtd_set_by_type, response, {}, {{CMD_SET_TYPE, set_type}}*/);
                                break;}

                                default: Logger::obj().write(name(), "SET BY TYPE is not accessable", Logger::log_error);
                            }
                        break;}

                        case set_popular_video: { return videoByCategory(LSTR("ugc")); break;}

                        case set_top_video: { return videoByCategory(LSTR("top")); break;}

                        case set_feed_video: { return videoByCategory(LSTR("feed")); break;}

                        case set_series_video: { return videoByCategory(LSTR("series")); break;}

                        case set_other_video: { return videoByCategory(LSTR("other")); break;}
                    }

                    return QJsonObject();
                }

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_tracks: return QStringLiteral("Popular Tracks: ");
                        case set_popular_video: return QStringLiteral("Popular Video");
                        case set_feed_video: return QStringLiteral("Video from user feeds");
                        case set_top_video: return QStringLiteral("Redaction choose Video");
                        case set_series_video: return QStringLiteral("Serial Video");
                        case set_other_video: return QStringLiteral("Some Video");

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
                    QMap<QString, QString> opts = audioSetOptions();

                    QString popular_title = setTypeToStr(set_popular_tracks);
                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
                        res.insert(
                            popular_title % opt.key(),
                            cmd_tmpl.setAttrsAsStr(
                                {
                                    { CMD_SET_TYPE, set_popular_tracks },
                                    { CMD_GENRE, opt.value() }
                                }
                            )
                        );
                    }

                    res.insert(setTypeToStr(set_popular_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_popular_video }}));
                    res.insert(setTypeToStr(set_feed_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_feed_video }}));
                    res.insert(setTypeToStr(set_top_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_top_video }}));
                    res.insert(setTypeToStr(set_series_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_series_video }}));
                    res.insert(setTypeToStr(set_other_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_other_video }}));

                    return res;
                }

                inline QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
//                inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
            };
        }
    }
}

#endif // VK_SET
