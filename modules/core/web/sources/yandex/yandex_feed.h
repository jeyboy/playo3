#ifndef YANDEX_FEED_H
#define YANDEX_FEED_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Feed : public virtual Base {
            public:
                // ncrnd=0.9069681936002026
                // days: [{day, tracksToPlay: [], events: [{type = 'recomended-genre-radio', title: {}, genres: [id, artists:[{}], albums: []]}, {type='genre-top, tracks: [], genre, title: {type, text}'}]}]
//                inline QString feedsUrl() { return url_site_v1 + QStringLiteral("feed.jsx?nocheck="); }

                // curl 'https://music.yandex.ua/handlers/feed.jsx?nocheck=&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.328420764991853' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://music.yandex.ua/feed' -H 'X-Current-UID: 363617853' -H 'X-Retpath-Y: https://music.yandex.ua/feed' -H 'X-Requested-With: XMLHttpRequest' -H 'Cookie: Session_id=3:1474023377.5.0.1474023377854:F3CJsg:16.0|363617853.0.2|4:118655.726606.c8Y2XC5XsZJwkOLjRCP6VQofygY; yandexuid=2190563221452621883; L=RFwFBQVmBgVyWVRkYU16VwFUVUNRenN4IyQzOyoWU19QRA==.1474023377.12651.37347.0faa78387be102abce08ff7197b92a62; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1#1789383377.udn.cDpqZXlib3kxOTg1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=OhTbN201Z8L819ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; device_id="aff97816c34c903dcae394ba20c1b499028c14c94"; ys=udn.cDpqZXlib3kxOTg1; _ym_isad=1; _ym_visorc_1028356=b; sessionid2=3:1474023377.5.0.1474023377854:F3CJsg:16.1|363617853.0.2|4:118655.305085.0npC_lcOUN-MsuMxUPyo_YPp5QQ; yandex_login=jeyboy1985' -H 'Connection: keep-alive'

//                QJsonValue feedsBySource(const QStringList & sources = QStringList()) { // not finished
//                    Permissions perm = permissions(pr_media_content);
//                    QJsonArray block_content;

//                    switch(perm) {
//                        case perm_site:
//                        case perm_api: {
//                            block_content = saRequest(
//                                baseUrlStr(
//                                    qst_api, QStringLiteral("newsfeed.get"),
//                                    {
//                                        { QStringLiteral("filters"), QStringLiteral("audio,video") },
//                                        { QStringLiteral("count"), 100 },
//                                        { QStringLiteral("source_ids"), sources }
//                                    }
//                                ),
//                                call_type_json, 0, proc_json_extract, QStringList() << tkn_response << tkn_items
//                            );
//                        break;}

//                        default: Logger::obj().write(name(), "feedsBySource is not accessable", true);
//                    }

//                    return prepareBlock(dmt_feed, block_content);
//                }
            };
        }
    }
}

#endif // YANDEX_FEED_H
