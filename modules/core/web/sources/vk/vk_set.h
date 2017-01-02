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
                        { LSTR("All Genres"),                 LSTR("0") },
                        { LSTR("Rock"),                       LSTR("1") },
                        { LSTR("Pop"),                        LSTR("2") },
                        { LSTR("Rap & Hip-Hop"),              LSTR("3") },
                        { LSTR("House & Dance"),              LSTR("5") },
                        { LSTR("Alternative"),                LSTR("21") },
                        { LSTR("Instrumental"),               LSTR("6") },
                        { LSTR("Easy Listening"),             LSTR("4") },
                        { LSTR("Metal"),                      LSTR("7") },
                        { LSTR("Dubstep & Trap"),             LSTR("8") },
                        { LSTR("Indie Pop"),                  LSTR("17") },
                        { LSTR("Jazz & Blues"),               LSTR("1001") },
                        { LSTR("Drum & Bass"),                LSTR("10") },
                        { LSTR("Trance"),                     LSTR("11") },
                        { LSTR("Ethnic"),                     LSTR("13") },
                        { LSTR("Acoustic & Vocal"),           LSTR("14") },
                        { LSTR("Reggae"),                     LSTR("15") },
                        { LSTR("Classical"),                  LSTR("16") },
                        { LSTR("Electropop & Disco"),         LSTR("22") }
                    };
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) { // rewrite on using of offset
                    SourceFlags perm = permissions(sf_compilation);
                    QJsonArray block_content;

                    switch(set_type) {
                        case set_popular_tracks: {
                            switch(perm) {
                                case sf_site: {
                                    QString genre_id = (limits.in_foreign() ? LSTR("foreign") : QString())
                                            % limits.genre; // 0 is eql to all genres

                                    Response * req_response = Manager::prepare() -> postFollowed(
                                        IQueriable::baseUrlStr(
                                            qst_site, LSTR("al_audio.php"),
                                            {
                                                { LSTR("act"),          LSTR("a_load_section") },
                                                { LSTR("al"),           LSTR("1") },
                                                { LSTR("album_id"),     genre_id },
                                                { LSTR("offset"),       limits.start_offset },
                                                { LSTR("owner_id"),     userID() },
                                                { LSTR("type"),         LSTR("popular") }
                                            }
                                        ),
                                        dntHeader()
                                    );

                                    QJsonArray items = JSON_ARR(RESPONSE_TO_JSON_OBJECT(req_response), LSTR("list"));
                                    prepareTracks(items, block_content);
                                    QueriableResponse response = QueriableResponse(block_content, QString::number(limits.start_offset.toInt() + block_content.size()), 0, 1, block_content.isEmpty());
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
                        case set_popular_tracks:        return LSTR("Popular Tracks: ");
                        case set_popular_video:         return LSTR("Popular Video");
                        case set_feed_video:            return LSTR("Video from user feeds");
                        case set_top_video:             return LSTR("Redaction choose Video");
                        case set_series_video:          return LSTR("Serial Video");
                        case set_other_video:           return LSTR("Some Video");

                        default:                        return LSTR("Unknown: ");
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

                    if (isConnected()) {
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

                        APPEND_SET_OPTION(res, cmd_tmpl, set_popular_video);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_feed_video);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_top_video);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_series_video);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_other_video);

//                        res.insert(setTypeToStr(set_popular_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_popular_video }}));
//                        res.insert(setTypeToStr(set_feed_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_feed_video }}));
//                        res.insert(setTypeToStr(set_top_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_top_video }}));
//                        res.insert(setTypeToStr(set_series_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_series_video }}));
//                        res.insert(setTypeToStr(set_other_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_other_video }}));
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
