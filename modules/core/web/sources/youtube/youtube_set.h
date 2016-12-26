#ifndef YOUTUBE_SET_H
#define YOUTUBE_SET_H

#include "youtube_video.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Set : public Video {
            protected:
                enum SetType { set_popular_video = 1, set_feed_video, set_top_video, set_video_history, set_video_liked, set_video_disliked, set_video_see_later };

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) { // rewrite on using of offset
//                    SourceFlags perm = permissions(sf_compilation);
//                    QJsonArray block_content;

                    switch(set_type) {
                        case set_popular_video:     {
                            QueriableResponse response;

                            response = pRequest(
                                videosUrl(),
                                call_type_json,
                                rules()
                            );

                            return prepareBlock(dmt_set, cmd_mtd_set_by_type, response, limits, {}/*, {{CMD_SET_TYPE, set_type}}*/);
                        break;}

                        case set_feed_video:        {

                        break;}

                        case set_top_video:         {

                        break;}

                        case set_video_history:     {

                        break;}

                        case set_video_liked:       { return videoByUserRating(true); break;}

                        case set_video_disliked:    { return videoByUserRating(false); break;}

                        case set_video_see_later:   {

                        break;}
                    }

                    return QJsonObject();
                }

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_video:         return LSTR("Popular video");
                        case set_feed_video:            return LSTR("Video from user feeds");
                        case set_top_video:             return LSTR("Redaction choose video");
                        case set_video_history:         return LSTR("View history");
                        case set_video_liked:           return LSTR("Liked video");
                        case set_video_disliked:        return LSTR("Disliked video");
                        case set_video_see_later:       return LSTR("See later video");

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

                    APPEND_SET_OPTION(res, cmd_tmpl, set_popular_video);
                    APPEND_SET_OPTION(res, cmd_tmpl, set_top_video);

                    if (isConnected()) {
                        APPEND_SET_OPTION(res, cmd_tmpl, set_feed_video);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_history);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_liked);
                        APPEND_SET_OPTION(res, cmd_tmpl, set_video_disliked);
                    }

//                    res.insert(setTypeToStr(set_popular_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_popular_video }}));
//                    res.insert(setTypeToStr(set_top_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_top_video }}));

//                    if (isConnected()) {
//                        res.insert(setTypeToStr(set_feed_video), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_feed_video }}));
//                        res.insert(setTypeToStr(set_history), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_history }}));
//                        res.insert(setTypeToStr(set_liked), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_liked }}));
//                        res.insert(setTypeToStr(set_disliked), cmd_tmpl.setAttrsAsStr({{ CMD_SET_TYPE, set_disliked }}));
//                    }

                    return res;
                }

                inline QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
//                inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
            };
        }
    }
}

#endif // YOUTUBE_SET_H
