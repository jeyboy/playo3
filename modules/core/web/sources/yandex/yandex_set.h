#ifndef YANDEX_SET_H
#define YANDEX_SET_H

#include "yandex_stream.h"
#include "yandex_video.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Set : public Stream, public Video {
            protected:
//                inline QString topUrl(const QString & filter = QString(), QString genre = LSTR("all")) { return url_site_v1 + LSTR("top.jsx?genre=%1&filter=%2").arg(genre, filter); }

                enum SetType {
                    set_popular_tracks = 1, set_popular_artists, set_popular_promotions,
                    set_new_albums, set_compilations, set_genre_stream
                };

                QMap<QString, QString> audioCompilationOptions() {
                    return {
                        { LSTR("Autumn melodies"), LSTR("autumn") },
                        { LSTR("How to love classical"), LSTR("classical") },
                        { LSTR("New music"), LSTR("newmusic") },
                        { LSTR("Background music"), LSTR("activity") },
                        { LSTR("Artist choose"), LSTR("artistchoice") },
                        { LSTR("By decades"), LSTR("decades") },
                        { LSTR("For holidays"), LSTR("holiday") },
                        { LSTR("Mood"), LSTR("mood") },
                        { LSTR("Music from serials"), LSTR("TVost") },
                        { LSTR("Music from moovies"), LSTR("ost") },
                        { LSTR("For workout"), LSTR("workout") },
                        { LSTR("Mixed genres"), LSTR("genres") }
                    };
                }
                QMap<QString, QString> audioSetOptions() {
                    return {
                        { LSTR("All Genres"),                 LSTR("all") },

                        { LSTR("Pop"),                        LSTR("pop") },
                        { LSTR("Russian Pop"),                LSTR("ruspop") },
                        { LSTR("Disco"),                      LSTR("disco") },

                        { LSTR("Indie"),                      LSTR("indie") },
                        { LSTR("Local Indie"),                LSTR("local-indie") },

                        { LSTR("Rock"),                       LSTR("rock") },
                        { LSTR("Russian Rock"),               LSTR("rusrock") },
                        { LSTR("Rock & Roll"),                LSTR("rock-n-roll") },
                        { LSTR("Progressive Rock"),           LSTR("prog-rock") },
                        { LSTR("Post Rock"),                  LSTR("post-rock") },
                        { LSTR("New Wave"),                   LSTR("new-wave") },
                        { LSTR("Ukranian Rock"),              LSTR("ukrrock") },

                        { LSTR("Metal"),                      LSTR("metal") },

                        { LSTR("Alternative"),                LSTR("alternative") },

                        { LSTR("Electronic"),                 LSTR("electronic") },
                        { LSTR("Dubstep"),                    LSTR("dubstep") },
                        { LSTR("Industrial"),                 LSTR("industrial") },
                        { LSTR("Experimental"),               LSTR("experimental") },

                        { LSTR("Dance"),                      LSTR("dance") },
                        { LSTR("House"),                      LSTR("house") },
                        { LSTR("Techno"),                     LSTR("techno") },
                        { LSTR("Trance"),                     LSTR("trance") },
                        { LSTR("Drum & Bass"),                LSTR("drum-n-bass") },

                        { LSTR("Rap and Hip-Hop"),            LSTR("hip-hop") },
                        { LSTR("Russian Rap"),                LSTR("rusrap") },

                        { LSTR("RNB"),                        LSTR("r-n-b") },
                        { LSTR("Urban"),                      LSTR("urban") },
                        { LSTR("Soul"),                       LSTR("soul") },
                        { LSTR("Funk"),                       LSTR("funk") },

                        { LSTR("Jazz"),                       LSTR("jazz") },
                        { LSTR("Traditional Jazz"),           LSTR("trad-jass") },
                        { LSTR("Modern Jazz"),                LSTR("modern-jazz") },

                        { LSTR("Blues"),                      LSTR("blues") },

                        { LSTR("Reggae"),                     LSTR("reggae") },
                        { LSTR("Reggaeton"),                  LSTR("reggaeton") },
                        { LSTR("Dub"),                        LSTR("dub") },

                        { LSTR("Ska"),                        LSTR("ska") },

                        { LSTR("Punk"),                       LSTR("punk") },
                        { LSTR("Hardcore"),                   LSTR("hardcore") },

                        { LSTR("World"),                      LSTR("world") },
                        { LSTR("Russian"),                    LSTR("russian") },
                        { LSTR("Tatar"),                      LSTR("tatar") },
                        { LSTR("Celtic"),                     LSTR("celtic") },
                        { LSTR("Balkan"),                     LSTR("balkan") },
                        { LSTR("European"),                   LSTR("european") },
                        { LSTR("Eastern"),                    LSTR("eastern") },
                        { LSTR("African"),                    LSTR("african") },
                        { LSTR("Latin"),                      LSTR("latin-american") },
                        { LSTR("American"),                   LSTR("american") },

                        { LSTR("Classical"),                  LSTR("classical") },
                        { LSTR("Opera"),                      LSTR("opera") },
                        { LSTR("Modern Classical"),           LSTR("modern-classical") },

                        { LSTR("Estrada"),                    LSTR("estrada") },
                        { LSTR("Russian Estrada"),            LSTR("rusestrada") },

                        { LSTR("Shanson"),                    LSTR("shanson") },

                        { LSTR("Country"),                    LSTR("country") },

                        { LSTR("Soundtrack"),                 LSTR("soundtrack") },
                        { LSTR("Soundtrack from moovies"),    LSTR("films") },
                        { LSTR("Soundtrack from serials"),    LSTR("tv-series") },
                        { LSTR("Soundtrack from cartoons"),   LSTR("animated-films") },
                        { LSTR("Soundtrack from videogames"), LSTR("videogame-music") },
                        { LSTR("Soundtrack from musicals"),   LSTR("musical") },
                        { LSTR("Soundtrack from bollywood"),  LSTR("bollywood") },

                        { LSTR("Easy Listening"),             LSTR("easy") },
                        { LSTR("Lounge"),                     LSTR("lounge") },
                        { LSTR("New Age"),                    LSTR("new-age") },
                        { LSTR("Meditative"),                 LSTR("meditative") },

                        { LSTR("Singer & Songwriter"),        LSTR("singer-songwriter") },
                        { LSTR("Russian Singer & Songwriter"),LSTR("rusbards") },
                        { LSTR("Romances"),                   LSTR("romances") },

                        { LSTR("For Children"),               LSTR("children") },

                        { LSTR("For Sport"),                  LSTR("sport") },
                        { LSTR("For Holidays"),               LSTR("holiday") }
                    };
                }

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_tracks: return QStringLiteral("Popular Tracks: ");
                        case set_popular_artists: return QStringLiteral("Popular Artists: ");
                        case set_popular_promotions: return QStringLiteral("Popular Promotions: ");
                        case set_new_albums: return QStringLiteral("New Albums: ");
                        case set_compilations: return QStringLiteral("Compilation: ");
                        case set_genre_stream: return QStringLiteral("Stream: ");
                        default: return QStringLiteral("Unknown: ");
                    }
                }
                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;
                    DataMediaType mtype = dmt_audio;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            switch(set_type) {
                                case set_popular_tracks:
                                case set_popular_artists:
                                case set_popular_promotions:
                                case set_new_albums: {                               
                                    response = pRequest(
                                        baseUrlStr(
                                            qst_site, LSTR("genre.jsx"),
                                            {
                                                {LSTR("genre"), limits.genre},
                                                {LSTR("external-domain"), LSTR("music.yandex.ua")},
                                                {LSTR("overembed"), LSTR("false")},
                                                {tkn_filter, limits.predicate}
                                            }
                                        ),
                                        call_type_json, pageRules(tkn_page, limits.start_offset, limits.items_limit),
                                        0, proc_json_extract, QStringList() << limits.predicate
                                    );

                                    switch(set_type) {
                                        case set_popular_tracks: {
                                            prepareTracks(response.content);
                                        break;}
                                        case set_popular_artists: {
                                            prepareArtists(response.content);
                                            mtype = dmt_audio_set;
                                        break;}
                                        case set_popular_promotions: {
                                            preparePromotions(response.content);
                                            mtype = dmt_dir;
                                        break;}
                                        case set_new_albums: {
                                            prepareAlbums(response.content);
                                            mtype = dmt_audio_set;
                                        break;}
                                        default:;
                                    }
                                break;}

                                case set_compilations: {
                                    QJsonObject obj = sRequest(
                                        baseUrlStr(
                                            qst_site, LSTR("mix-promos.jsx"),
                                            {{LSTR("id"), limits.genre}}
                                        ),
                                        call_type_json
                                    );

                                    response = QueriableResponse(JSON_ARR(obj, tkn_promotions), QString(), 1, 1, true);
                                    preparePromotions(response.content);
                                    mtype = dmt_dir;
                                break;}

                                case set_genre_stream: {
                                    return streamsByGenre(limits.genre);
                                break;}
                            }
                        break;}

                        default: Logger::obj().write(name(), "SET BY TYPE is not accessable", Logger::log_error);
                    }

                    return prepareBlock(mtype, cmd_mtd_set_by_type, response, limits, {}, {{CMD_SET_TYPE, set_type}});
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

                    QString pop_tracks_title = setTypeToStr(set_popular_tracks);
                    QString pop_artists_title = setTypeToStr(set_popular_artists);
                    QString pop_promos_title = setTypeToStr(set_popular_promotions);
                    QString new_albums_title = setTypeToStr(set_new_albums);
                    QString compilations_title = setTypeToStr(set_compilations);
                    QString stream_title = setTypeToStr(set_genre_stream);

                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});
                    Cmd stream_cmd_tmpl(sourceType() | dt_stream, cmd_mtd_open_set, {});

                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
                        if (opt.value() != LSTR("all")) {
                            res.insert(
                                stream_title % opt.key(),
                                stream_cmd_tmpl.setAttrs(
                                    {
                                        { CMD_SET_TYPE, QString::number(set_genre_stream) },
                                        { CMD_GENRE, opt.value() },
                                        { CMD_STREAM_CONFIGURABLE, LSTR("t") }
                                    }
                                ) -> toString()
                            );
                        }

                        res.insert(
                            pop_tracks_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_popular_tracks) },
                                    { CMD_GENRE, opt.value() },
                                    { CMD_PREDICATE, LSTR("tracks") }
                                }
                            ) -> toString()
                        );

                        res.insert(
                            pop_artists_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_popular_artists) },
                                    { CMD_GENRE, opt.value() },
                                    { CMD_PREDICATE, LSTR("artists") }
                                }
                            ) -> toString()
                        );

                        res.insert(
                            pop_promos_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_popular_promotions) },
                                    { CMD_GENRE, opt.value() },
                                    { CMD_PREDICATE, LSTR("promotions") }
                                }
                            ) -> toString()
                        );

                        res.insert(
                            new_albums_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_new_albums) },
                                    { CMD_GENRE, opt.value() },
                                    { CMD_PREDICATE, LSTR("albums") }
                                }
                            ) -> toString()
                        );
                    }

                    QMap<QString, QString> compilations = audioCompilationOptions();
                    for(QMap<QString, QString>::Iterator opt = compilations.begin(); opt != compilations.end(); opt++) {
                        res.insert(
                            compilations_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_compilations) },
                                    { CMD_GENRE, opt.value() }
                                }
                            ) -> toString()
                        );
                    }

//                    res.insert(
//                        popular_title % PACKAGE_REPLACE_FRAGMENT_TITLE,
//                        cmd_tmpl.setAttrs(
//                            {
//                                { CMD_SET_TYPE, QString::number(set_popular) },
//                                { CMD_GENRE, PACKAGE_REPLACE_FRAGMENT }
//                            }
//                        ) -> toString()
//                    );

                    return res;
                }

                QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
            };
        }
    }
}

#endif // YANDEX_SET_H
