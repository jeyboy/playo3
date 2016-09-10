#ifndef YANDEX_SET_H
#define YANDEX_SET_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Set : public Base {
            protected:
//                inline QString topUrl(const QString & filter = QString(), QString genre = LSTR("all")) { return url_site_v1 + LSTR("top.jsx?genre=%1&filter=%2").arg(genre, filter); }

                enum SetType {
                    set_popular_tracks = 1, set_popular_artists, set_popular_promotions, set_new_albums,
                    set_compilations
                };

                QMap<QString, QString> siteOptions() {
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
                    //curl 'https://music.yandex.ua/handlers/mix-promos.jsx?id=autumn&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.7792388001525733' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; _ym_isad=1; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=N9ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_visorc_1028356=b' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/mix/autumn' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/mix/autumn'

                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            switch(set_type) {
                                case set_popular_tracks:
                                case set_popular_artists:
                                case set_popular_promotions:
                                case set_new_albums: {
                                    response = pRequest(
                                        // curl 'https://music.yandex.ua/handlers/genre.jsx?genre=ska&filter=&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.5854833866762779' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=N9ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1; _ym_visorc_1028356=b' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/genre/ska' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/genre/ska'
                                        // -H 'X-Retpath-Y: https://music.yandex.ua/genre/ska'

                                        baseUrlStr(
                                            qst_site, LSTR("genre.jsx"),
                                            {
                                                {LSTR("genre"), limits.genre},
                                                {LSTR("external-domain"), LSTR("music.yandex.ua")},
                                                {LSTR("overembed"), LSTR("false")},
                                                {LSTR("filter"), limits.predicate}
                                            }
                                        ),
                                        call_type_json, pageRules(LSTR("page"), limits.start_offset, limits.items_limit), 0, proc_json_patch
                                    );

                                    int i = 0;


                                break;}

                                case set_compilations: {
                                    // curl 'https://music.yandex.ua/handlers/mix-promos.jsx?id=classical&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.0015247747424892433' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=N9ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/mix/classical' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/mix/classical'

//                                    QJsonArray block_content = saRequest(
//                                        baseUrlStr(
//                                            qst_site_alt1, QStringLiteral("charts"),
//                                            {
//                                                { tkn_limit, SOUNDCLOUD_PER_REQUEST_LIMIT },
//                                                { QStringLiteral("genre"), QString(QStringLiteral("soundcloud:genres:") % (limits.genre.isEmpty() ? SOUNDCLOUD_ALL_GENRES_PARAM : limits.genre)) },
//                                                { QStringLiteral("kind"), set_type == set_new_hot ? QStringLiteral("trending") : QStringLiteral("top") }
//                                            }
//                                        ),
//                                        call_type_json, 0, proc_json_patch, IQUERY_DEF_FIELDS << QStringLiteral("track"), call_method_get, headers()
//                                    );

                                    return prepareBlock(dmt_audio_set, block_content);
                                break;}
                            }
                        break;}

                        default: Logger::obj().write(name(), "SET BY TYPE is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_set_by_type, response, limits, {}, {{CMD_SET_TYPE, set_type}});
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

                    QString pop_tracks_title = setTypeToStr(set_popular_tracks);
                    QString pop_artists_title = setTypeToStr(set_popular_artists);
                    QString pop_promos_title = setTypeToStr(set_popular_promotions);
                    QString new_albums_title = setTypeToStr(set_new_albums);

                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
//                        res.insert(
//                            new_hot_title % opt.key(),
//                            cmd_tmpl.setAttrs(
//                                {
//                                    { CMD_SET_TYPE, QString::number(set_new_hot) },
//                                    { CMD_GENRE, opt.value() }
//                                }
//                            ) -> toString()
//                        );
//                        res.insert(
//                            top_50_title % opt.key(),
//                            cmd_tmpl.setAttrs(
//                                {
//                                    { CMD_SET_TYPE, QString::number(set_top_50) },
//                                    { CMD_GENRE, opt.value() }
//                                }
//                            ) -> toString()
//                        );

//                        QStringList parts = opt.key().split('&', QString::SkipEmptyParts);

//                        if (parts.size() == 1)
//                            res.insert(
//                                popular_title % opt.key(),
//                                cmd_tmpl.setAttrs(
//                                    {
//                                        { CMD_SET_TYPE, QString::number(set_popular) },
//                                        { CMD_GENRE, opt.value() }
//                                    }
//                                ) -> toString()
//                            );
//                        else {
//                            res.insert(
//                                popular_title % parts.first(),
//                                cmd_tmpl.setAttrs(
//                                    {
//                                        { CMD_SET_TYPE, QString::number(set_popular) },
//                                        { CMD_GENRE, parts.first() }
//                                    }
//                                ) -> toString()
//                            );
//                            res.insert(
//                                popular_title % parts.last(),
//                                cmd_tmpl.setAttrs(
//                                    {
//                                        { CMD_SET_TYPE, QString::number(set_popular) },
//                                        { CMD_GENRE, parts.last() }
//                                    }
//                                ) -> toString()
//                            );
//                        }
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

//                    res.insert(
//                        setTypeToStr(set_recommended_artists),
//                        cmd_tmpl.setAttrs({{ CMD_SET_TYPE, QString::number(set_recommended_artists) }}) -> toString()
//                    );

                    return res;
                }

                QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
            };
        }
    }
}

#endif // YANDEX_SET_H
