#ifndef YANDEX_SET_H
#define YANDEX_SET_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Set : public Base {
            protected:
                inline QString topUrl(const QString & filter = QString(), QString genre = QStringLiteral("all")) { return url_site_v1 + QStringLiteral("top.jsx?genre=%1&filter=%2").arg(genre, filter); }

                enum SetType { /*set_new_hot = 1, set_top_50, set_popular, set_recommended_artists*/ };

                QMap<QString, QString> siteOptions() {
                    return {
//                        { QStringLiteral("All Genres"),                 QStringLiteral("all-music") },
                    };
                }

                QString setTypeToStr(const SetType & stype) {
//                    switch(stype) {
//                        case set_new_hot: return QStringLiteral("New & Hot: ");
//                        case set_top_50: return QStringLiteral("Top 50: ");
//                        case set_popular: return QStringLiteral("Popular: ");
//                        case set_recommended_artists: return QStringLiteral("Recommended Artists");
//                        default: return QStringLiteral("Unknown: ");
//                    }
                }
                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    //curl 'https://music.yandex.ua/handlers/mix-promos.jsx?id=autumn&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.7792388001525733' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; _ym_isad=1; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=N9ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_visorc_1028356=b' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/mix/autumn' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/mix/autumn'



//                    Permissions perm = permissions(pr_media_content);
//                    QueriableResponse response;

//                    switch(perm) {
//                        case perm_api:
//                        case perm_site: {
//                            switch(set_type) {
//                                case set_popular: {
//                                    response = pRequest(
//                                        baseUrlStr(
//                                            qst_api, path_tracks,
//                                            trackSearchQuery(QString(), limits.genre == SOUNDCLOUD_ALL_GENRES_PARAM ? QString() : limits.genre, true)
//                                        ),
//                                        call_type_json, rules(limits.start_offset, qMax(50, limits.items_limit)), 0, proc_json_patch
//                                    );
//                                break;}

//                                case set_new_hot:
//                                case set_top_50: {
//                                    QJsonArray block_content = saRequest(
//                                        baseUrlStr(
//                                            qst_site_alt1, QStringLiteral("charts"),
//                                            {
//                                                { tkn_limit, SOUNDCLOUD_PER_REQUEST_LIMIT },
//                                                { QStringLiteral("genre"), QString(QStringLiteral("soundcloud:genres:") % (limits.genre.isEmpty() ? SOUNDCLOUD_ALL_GENRES_PARAM : limits.genre)) },
//                                                { QStringLiteral("kind"), set_type == set_new_hot ? QStringLiteral("trending") : QStringLiteral("top") }
//                                            }
//                                        ),
//                                        call_type_json, 0, proc_json_patch, COLLECTION_FIELDS << QStringLiteral("track"), call_method_get, headers()
//                                    );

//                                    return prepareBlock(dmt_audio, block_content);
//                                break;}
//                            }
//                        break;}

//                        default: Logger::obj().write("Soundcloud", "SET BY TYPE is not accessable", true);
//                    }

//                    return prepareBlock(dmt_audio, cmd_mtd_set_by_type, response, limits, {}, {{CMD_SET_TYPE, set_type}});
                }

            public:
                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QMap<QString, QString> setsList() {
//                    QMap<QString, QString> res;
//                    QMap<QString, QString> opts = siteOptions();

//                    QString new_hot_title = setTypeToStr(set_new_hot);
//                    QString top_50_title = setTypeToStr(set_top_50);
//                    QString popular_title = setTypeToStr(set_popular);

//                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

//                    for(QMap<QString, QString>::Iterator opt = opts.begin(); opt != opts.end(); opt++) {
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
//                    }

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

//                    return res;
                }

                QJsonValue openSet(const QUrlQuery & attrs) { return QJsonArray() << setByType(attrs); }
            };
        }
    }
}

#endif // YANDEX_SET_H
