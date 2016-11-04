#ifndef YOUTUBE_SET_H
#define YOUTUBE_SET_H

#include "youtube_defines.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Set : public virtual Base {
            protected:
                enum SetType { set_popular_video = 1, set_feed_video, set_top_video, set_history, set_liked, set_see_later };

                QJsonValue setByType(const SetType & /*set_type*/, const SearchLimit & /*limits*/) { // rewrite on using of offset
//                    SourceFlags perm = permissions(sf_compilation);
//                    QJsonArray block_content;

//                    switch(set_type) {
//                        case set_popular_tracks: {
//                            switch(perm) {
//                                case sf_site: {
//                                    QString genre_id = (limits.in_foreign() ? LSTR("foreign") : QString())
//                                            % limits.genre; // 0 is eql to all genres

//                                    Response * req_response = Manager::prepare() -> postFollowed(
//                                        IQueriable::baseUrlStr(
//                                            qst_site, LSTR("al_audio.php"),
//                                            {
//                                                { LSTR("act"),          LSTR("a_load_section") },
//                                                { LSTR("al"),           LSTR("1") },
//                                                { LSTR("album_id"),     genre_id },
//                                                { LSTR("offset"),       limits.start_offset },
//                                                { LSTR("owner_id"),     userID() },
//                                                { LSTR("type"),         LSTR("popular") }
//                                            }
//                                        ),
//                                        dntHeader()
//                                    );

//                                    QJsonArray items = JSON_ARR(RESPONSE_TO_JSON_OBJECT(req_response), LSTR("list"));
//                                    prepareTracks(items, block_content);
//                                    QueriableResponse response = QueriableResponse(block_content, QString::number(limits.start_offset + block_content.size()), 0, 1, block_content.isEmpty());
//                                    return prepareBlock(dmt_audio, cmd_mtd_set_by_type, response, limits, {}, {{CMD_SET_TYPE, set_type}});
//                                break;}

//                                case sf_api: {
//                                    QString genre_id = limits.genre;

//                                    block_content = saRequest(
//                                        baseUrlStr(
//                                            qst_api, tkn_execute,
//                                            {
//                                                {
//                                                    tkn_code, QString(
//                                                       "var popular = API.audio.getPopular({"
//                                                            "only_eng: " + boolToStr(limits.in_foreign())/*boolToStr(onlyEng)*/ + ", "
//                                                            "count: 1000 " % (!genre_id.isEmpty() ? (", genre_id: " + genre_id) : "") + ""
//                                                         "});"
//                                                       "return {\"" % block_items_audio % "\": popular};"
//                                                    )
//                                                }
//                                            }
//                                        ),
//                                        call_type_json, 0, proc_json_extract,
//                                        IQUERY_DEF_FIELDS << block_items_audio
//                                    );

//                                    prepareBlock(dmt_audio, block_content/*,cmd_mtd_set_by_type, response, {}, {{CMD_SET_TYPE, set_type}}*/);
//                                break;}

//                                default: Logger::obj().write(name(), "SET BY TYPE is not accessable", Logger::log_error);
//                            }
//                        break;}

//                        case set_popular_video: { return videoByCategory(LSTR("ugc")); break;}

//                        case set_top_video: { return videoByCategory(LSTR("top")); break;}

//                        case set_feed_video: { return videoByCategory(LSTR("feed")); break;}

//                        case set_series_video: { return videoByCategory(LSTR("series")); break;}

//                        case set_other_video: { return videoByCategory(LSTR("other")); break;}
//                    }

                    return QJsonObject();
                }

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_video:         return LSTR("Popular video");
                        case set_feed_video:            return LSTR("Video from user feeds");
                        case set_top_video:             return LSTR("Redaction choose video");
                        case set_history:               return LSTR("View history");
                        case set_liked:                 return LSTR("Liked video");
                        case set_see_later:             return LSTR("See later video");

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
//                    QMap<QString, QString> opts = audioSetOptions();

//                    QString popular_title = setTypeToStr(set_popular_tracks);
                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

//                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
//                        res.insert(
//                            popular_title % opt.key(),
//                            cmd_tmpl.setAttrsAsStr(
//                                {
//                                    { CMD_SET_TYPE, set_popular_tracks },
//                                    { CMD_GENRE, opt.value() }
//                                }
//                            )
//                        );
//                    }

                    res.insert(setTypeToStr(set_popular_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_popular_video }}));
                    res.insert(setTypeToStr(set_feed_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_feed_video }}));
                    res.insert(setTypeToStr(set_top_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_top_video }}));
                    res.insert(setTypeToStr(set_history), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_history }}));
                    res.insert(setTypeToStr(set_liked), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_liked }}));

                    return res;
                }

                inline QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
//                inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
            };
        }
    }
}

#endif // YOUTUBE_SET_H
