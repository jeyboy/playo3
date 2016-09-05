#ifndef SOUNDCLOUD_SET
#define SOUNDCLOUD_SET

#include "soundcloud_defines.h"
#include "soundcloud_stream.h"

#define SOUNDCLOUD_ALL_GENRES_PARAM QStringLiteral("all-music")

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Set : public Stream {
            protected:
                enum SetType { set_new_hot = 1, set_top_50, set_popular, set_recommended_artists };

                // liked and owned playlists
                //curl 'https://api-v2.soundcloud.com/users/99021496/playlists/liked_and_owned?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=12&offset=0&linked_partitioning=1&app_version=1472742050' -H 'Host: api-v2.soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Referer: https://soundcloud.com/' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive'

                // liked stations
                //curl 'https://api-v2.soundcloud.com/stations/liked?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1472742050' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0'

                // played tracks
                //curl 'https://api-v2.soundcloud.com/me/play-history/tracks?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1472742050' -H 'Host: api-v2.soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Referer: https://soundcloud.com/' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive'

                // played stations
                // curl 'https://api-v2.soundcloud.com/me/play-history/contexts?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1472742050' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0'

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_new_hot: return QStringLiteral("New & Hot: ");
                        case set_top_50: return QStringLiteral("Top 50: ");
                        case set_popular: return QStringLiteral("Popular: ");
                        case set_recommended_artists: return QStringLiteral("Recommended Artists");
                        default: return QStringLiteral("Unknown: ");
                    }
                }

                QMap<QString, QString> siteOptions() {
                    return {
                        { QStringLiteral("All Genres"),                 QStringLiteral("all-music") },
                        { QStringLiteral("Alternative Rock"),           QStringLiteral("alternativerock") },
                        { QStringLiteral("Ambient"),                    QStringLiteral("ambient") },
                        { QStringLiteral("Classical"),                  QStringLiteral("classical") },
                        { QStringLiteral("Country"),                    QStringLiteral("country") },
                        { QStringLiteral("Dance & EDM"),                QStringLiteral("danceedm") },
                        { QStringLiteral("Dancehall"),                  QStringLiteral("dancehall") },
                        { QStringLiteral("Deep House"),                 QStringLiteral("deephouse") },
                        { QStringLiteral("Disco"),                      QStringLiteral("disco") },
                        { QStringLiteral("Drum & Bass"),                QStringLiteral("drumbass") },
                        { QStringLiteral("Dubstep"),                    QStringLiteral("dubstep") },
                        { QStringLiteral("Electronic"),                 QStringLiteral("electronic") },
                        { QStringLiteral("Folk & Singer-Songwriter"),   QStringLiteral("folksingersongwriter") },
                        { QStringLiteral("Hip-hop & Rap"),              QStringLiteral("hiphoprap") },
                        { QStringLiteral("House"),                      QStringLiteral("house") },
                        { QStringLiteral("Indie"),                      QStringLiteral("indie") },
                        { QStringLiteral("Jazz & Blues"),               QStringLiteral("jazzblues") },
                        { QStringLiteral("Latin"),                      QStringLiteral("latin") },
                        { QStringLiteral("Metal"),                      QStringLiteral("metal") },
                        { QStringLiteral("Piano"),                      QStringLiteral("piano") },
                        { QStringLiteral("Pop"),                        QStringLiteral("pop") },
                        { QStringLiteral("R&B & Soul"),                 QStringLiteral("rbsoul") },
                        { QStringLiteral("Reggae"),                     QStringLiteral("reggae") },
                        { QStringLiteral("Reggaeton"),                  QStringLiteral("reggaeton") },
                        { QStringLiteral("Rock"),                       QStringLiteral("rock") },
                        { QStringLiteral("Soundtrack"),                 QStringLiteral("soundtrack") },
                        { QStringLiteral("Techno"),                     QStringLiteral("techno") },
                        { QStringLiteral("Trance"),                     QStringLiteral("trance") },
                        { QStringLiteral("Trap"),                       QStringLiteral("trap") },
                        { QStringLiteral("Triphop"),                    QStringLiteral("triphop") },
                        { QStringLiteral("World"),                      QStringLiteral("world") }
                    };
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            switch(set_type) {
                                case set_popular: {
                                    response = pRequest(
                                        baseUrlStr(
                                            qst_api, path_tracks,
                                            trackSearchQuery(QString(), limits.genre == SOUNDCLOUD_ALL_GENRES_PARAM ? QString() : limits.genre, true)
                                        ),
                                        call_type_json, rules(limits.start_offset, qMax(50, limits.items_limit)), 0, proc_json_patch
                                    );
                                break;}

                                case set_new_hot:
                                case set_top_50: {
                                    QJsonArray block_content = saRequest(
                                        baseUrlStr(
                                            qst_site_alt1, QStringLiteral("charts"),
                                            {
                                                { tkn_limit, SOUNDCLOUD_PER_REQUEST_LIMIT },
                                                { QStringLiteral("genre"), QString(QStringLiteral("soundcloud:genres:") % (limits.genre.isEmpty() ? SOUNDCLOUD_ALL_GENRES_PARAM : limits.genre)) },
                                                { QStringLiteral("kind"), set_type == set_new_hot ? QStringLiteral("trending") : QStringLiteral("top") }
                                            }
                                        ),
                                        call_type_json, 0, proc_json_patch, COLLECTION_FIELDS << QStringLiteral("track"), call_method_get, headers()
                                    );

                                    return prepareBlock(dmt_audio, block_content);
                                break;}

                                case set_recommended_artists: {
                                    return streamsRecommendations(limits.start_offset, qMax(50, limits.items_limit));
                                break;}
                            }
                        break;}

                        default: Logger::obj().write("Soundcloud", "SET BY TYPE is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_set_by_type, response, limits, {{CMD_SET_TYPE, set_type}});
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

                    QString new_hot_title = setTypeToStr(set_new_hot);
                    QString top_50_title = setTypeToStr(set_top_50);
                    QString popular_title = setTypeToStr(set_popular);

                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
                        res.insert(
                            new_hot_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_new_hot) },
                                    { CMD_GENRE, opt.value() }
                                }
                            ) -> toString()
                        );
                        res.insert(
                            top_50_title % opt.key(),
                            cmd_tmpl.setAttrs(
                                {
                                    { CMD_SET_TYPE, QString::number(set_top_50) },
                                    { CMD_GENRE, opt.value() }
                                }
                            ) -> toString()
                        );

                        QStringList parts = opt.key().split('&', QString::SkipEmptyParts);

                        if (parts.size() == 1)
                            res.insert(
                                popular_title % opt.key(),
                                cmd_tmpl.setAttrs(
                                    {
                                        { CMD_SET_TYPE, QString::number(set_popular) },
                                        { CMD_GENRE, opt.value() }
                                    }
                                ) -> toString()
                            );
                        else {
                            res.insert(
                                popular_title % parts.first(),
                                cmd_tmpl.setAttrs(
                                    {
                                        { CMD_SET_TYPE, QString::number(set_popular) },
                                        { CMD_GENRE, parts.first() }
                                    }
                                ) -> toString()
                            );
                            res.insert(
                                popular_title % parts.last(),
                                cmd_tmpl.setAttrs(
                                    {
                                        { CMD_SET_TYPE, QString::number(set_popular) },
                                        { CMD_GENRE, parts.last() }
                                    }
                                ) -> toString()
                            );
                        }
                    }

                    res.insert(
                        popular_title % PACKAGE_REPLACE_FRAGMENT_TITLE,
                        cmd_tmpl.setAttrs(
                            {
                                { CMD_SET_TYPE, QString::number(set_popular) },
                                { CMD_GENRE, PACKAGE_REPLACE_FRAGMENT }
                            }
                        ) -> toString()
                    );

                    res.insert(
                        setTypeToStr(set_recommended_artists),
                        cmd_tmpl.setAttrs({{ CMD_SET_TYPE, QString::number(set_recommended_artists) }}) -> toString()
                    );

                    return res;
                }

                //inline QJsonValue openSet(const QString & attrs) { return openSet(Cmd::extractQuery(attrs)); }
                QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
            };
        }
    }
}

#endif // SOUNDCLOUD_SET
