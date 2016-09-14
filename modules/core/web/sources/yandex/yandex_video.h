#ifndef YANDEX_VIDEO_H
#define YANDEX_VIDEO_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Video : public Base {
            public:
                // https://yandex.ua/video/

                // curl 'https://media.clipyou.ru/index/player?player=new&record_id=22029&ad_template_id=8336&player_template_id=9663' -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: SERVERID=m2Db20p80|V9iKx|V9iKx' -H 'DNT: 1' -H 'Host: media.clipyou.ru' -H 'Referer: https://music.yandex.ua/artist/79215/tracks' -H 'Upgrade-Insecure-Requests: 1' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0'
            };
        }
    }
}

#endif // YANDEX_VIDEO_H
