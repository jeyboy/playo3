#ifndef YANDEX_SET_H
#define YANDEX_SET_H

#include "yandex_stream.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Set : public Stream {
            protected:
//                inline QString topUrl(const QString & filter = QString(), QString genre = LSTR("all")) { return url_site_v1 + LSTR("top.jsx?genre=%1&filter=%2").arg(genre, filter); }

                enum SetType {
                    set_popular_tracks = 1, set_popular_artists, set_popular_promotions, set_new_albums,
                    set_compilations
                };

                QMap<QString, QString> audioSetOptions() {
                    return {
                        { QStringLiteral("All Genres"),                 QStringLiteral("all") },

                        { QStringLiteral("Pop"),                        QStringLiteral("pop") },
                        { QStringLiteral("Russian Pop"),                QStringLiteral("ruspop") },
                        { QStringLiteral("Disco"),                      QStringLiteral("disco") },

                        { QStringLiteral("Indie"),                      QStringLiteral("indie") },
                        { QStringLiteral("Local Indie"),                QStringLiteral("local-indie") },

                        { QStringLiteral("Rock"),                       QStringLiteral("rock") },
                        { QStringLiteral("Russian Rock"),               QStringLiteral("rusrock") },
                        { QStringLiteral("Rock & Roll"),                QStringLiteral("rock-n-roll") },
                        { QStringLiteral("Progressive Rock"),           QStringLiteral("prog-rock") },
                        { QStringLiteral("Post Rock"),                  QStringLiteral("post-rock") },
                        { QStringLiteral("New Wave"),                   QStringLiteral("new-wave") },
                        { QStringLiteral("Ukranian Rock"),              QStringLiteral("ukrrock") },

                        { QStringLiteral("Metal"),                      QStringLiteral("metal") },

                        { QStringLiteral("Alternative"),                QStringLiteral("alternative") },

                        { QStringLiteral("Electronic"),                 QStringLiteral("electronic") },
                        { QStringLiteral("Dubstep"),                    QStringLiteral("dubstep") },
                        { QStringLiteral("Industrial"),                 QStringLiteral("industrial") },
                        { QStringLiteral("Experimental"),               QStringLiteral("experimental") },

                        { QStringLiteral("Dance"),                      QStringLiteral("dance") },
                        { QStringLiteral("House"),                      QStringLiteral("house") },
                        { QStringLiteral("Techno"),                     QStringLiteral("techno") },
                        { QStringLiteral("Trance"),                     QStringLiteral("trance") },
                        { QStringLiteral("Drum & Bass"),                QStringLiteral("drum-n-bass") },

                        { QStringLiteral("Rap and Hip-Hop"),            QStringLiteral("hip-hop") },
                        { QStringLiteral("Russian Rap"),                QStringLiteral("rusrap") },

                        { QStringLiteral("RNB"),                        QStringLiteral("r-n-b") },
                        { QStringLiteral("Urban"),                      QStringLiteral("urban") },
                        { QStringLiteral("Soul"),                       QStringLiteral("soul") },
                        { QStringLiteral("Funk"),                       QStringLiteral("funk") },

                        { QStringLiteral("Jazz"),                       QStringLiteral("jazz") },
                        { QStringLiteral("Traditional Jazz"),           QStringLiteral("trad-jass") },
                        { QStringLiteral("Modern Jazz"),                QStringLiteral("modern-jazz") },

                        { QStringLiteral("Blues"),                      QStringLiteral("blues") },

                        { QStringLiteral("Reggae"),                     QStringLiteral("reggae") },
                        { QStringLiteral("Reggaeton"),                  QStringLiteral("reggaeton") },
                        { QStringLiteral("Dub"),                        QStringLiteral("dub") },

                        { QStringLiteral("Ska"),                        QStringLiteral("ska") },

                        { QStringLiteral("Punk"),                       QStringLiteral("punk") },
                        { QStringLiteral("Hardcore"),                   QStringLiteral("hardcore") },

                        { QStringLiteral("World"),                      QStringLiteral("world") },
                        { QStringLiteral("Russian"),                    QStringLiteral("russian") },
                        { QStringLiteral("Tatar"),                      QStringLiteral("tatar") },
                        { QStringLiteral("Celtic"),                     QStringLiteral("celtic") },
                        { QStringLiteral("Balkan"),                     QStringLiteral("balkan") },
                        { QStringLiteral("European"),                   QStringLiteral("european") },
                        { QStringLiteral("Eastern"),                    QStringLiteral("eastern") },
                        { QStringLiteral("African"),                    QStringLiteral("african") },
                        { QStringLiteral("Latin"),                      QStringLiteral("latin-american") },
                        { QStringLiteral("American"),                   QStringLiteral("american") },

                        { QStringLiteral("Classical"),                  QStringLiteral("classical") },
                        { QStringLiteral("Opera"),                      QStringLiteral("opera") },
                        { QStringLiteral("Modern Classical"),           QStringLiteral("modern-classical") },

                        { QStringLiteral("Estrada"),                    QStringLiteral("estrada") },
                        { QStringLiteral("Russian Estrada"),            QStringLiteral("rusestrada") },

                        { QStringLiteral("Shanson"),                    QStringLiteral("shanson") },

                        { QStringLiteral("Country"),                    QStringLiteral("country") },

                        { QStringLiteral("Soundtrack"),                 QStringLiteral("soundtrack") },
                        { QStringLiteral("Soundtrack from moovies"),    QStringLiteral("films") },
                        { QStringLiteral("Soundtrack from serials"),    QStringLiteral("tv-series") },
                        { QStringLiteral("Soundtrack from cartoons"),   QStringLiteral("animated-films") },
                        { QStringLiteral("Soundtrack from videogames"), QStringLiteral("videogame-music") },
                        { QStringLiteral("Soundtrack from musicals"),   QStringLiteral("musical") },
                        { QStringLiteral("Soundtrack from bollywood"),  QStringLiteral("bollywood") },

                        { QStringLiteral("Easy Listening"),             QStringLiteral("easy") },
                        { QStringLiteral("Lounge"),                     QStringLiteral("lounge") },
                        { QStringLiteral("New Age"),                    QStringLiteral("new-age") },
                        { QStringLiteral("Meditative"),                 QStringLiteral("meditative") },

                        { QStringLiteral("Singer & Songwriter"),        QStringLiteral("singer-songwriter") },
                        { QStringLiteral("Russian Singer & Songwriter"),QStringLiteral("rusbards") },
                        { QStringLiteral("Romances"),                   QStringLiteral("romances") },

                        { QStringLiteral("For Children"),               QStringLiteral("children") },

                        { QStringLiteral("For Sport"),                  QStringLiteral("sport") },
                        { QStringLiteral("For Holidays"),               QStringLiteral("holiday") }
                    };
                }

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_tracks: return QStringLiteral("Popular Tracks: ");
                        case set_popular_artists: return QStringLiteral("Popular Artists: ");
                        case set_popular_promotions: return QStringLiteral("Popular Promotions: ");
                        case set_new_albums: return QStringLiteral("New Albums: ");
                        case set_compilations: return QStringLiteral("Compilation: ");
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
                                                {LSTR("filter"), limits.predicate}
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
                            }
                        break;}

                        default: Logger::obj().write(name(), "SET BY TYPE is not accessable", true);
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

                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
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

                    QMap<QString, QString> compilations = {
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
