#ifndef YANDEX_VIDEO_H
#define YANDEX_VIDEO_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Video : public virtual Base {
            public:
                // https://yandex.ua/video/

                // curl 'https://media.clipyou.ru/index/player?player=new&record_id=22029&ad_template_id=8336&player_template_id=9663' -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: SERVERID=m2Db20p80|V9iKx|V9iKx' -H 'DNT: 1' -H 'Host: media.clipyou.ru' -H 'Referer: https://music.yandex.ua/artist/79215/tracks' -H 'Upgrade-Insecure-Requests: 1' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0'

                QString videoUrl(const QString & video_id) {//TODO: write me
//                    // codec, bitrate, src, gain
//                    QJsonObject obj = Manager::prepare() -> jsonGet(
//                        baseUrlStr(qst_site_alt1, LSTR("track/%1/download").arg(track_id), {})
//                    );

//                    obj = Manager::prepare() -> jsonGet(
//                        QString(JSON_STR(obj, LSTR("src")) % LSTR("&format=json"))
//                    );

//                    QString path = JSON_STR(obj, LSTR("path"));

//                    return QStringLiteral("https://%1/get-mp3/%2/%3%4?track-id=%5&play=false&").arg(
//                        JSON_STR(obj, LSTR("host")), calcKey(path, JSON_CSTR(obj, LSTR("s"))),
//                        JSON_CSTR(obj, LSTR("ts")), path, track_id.split(':').first()
//                    );

                    return QString();
                }

                QJsonValue videoSearch(const QUrlQuery & args) { return videoSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue videoSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
//                            response = pRequest(
//                                baseUrlStr(qst_site, LSTR("music-search.jsx"),
//                                     {
//                                         {LSTR("text"), limits.predicate},
//                                         {LSTR("type"), tkn_videos}
//                                     }
//                                ),
//                                call_type_json, pageRules(tkn_page, limits.start_offset, limits.requests_limit, limits.items_limit),
//                                0, proc_json_extract, QStringList() << tkn_videos << tkn_items
//                            );

                            prepareVideos(response.content);
                        break;}

                        default: Logger::obj().write(name(), "VIDEO SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_tracks_search, response, limits);
                }

            };
        }
    }
}

#endif // YANDEX_VIDEO_H
