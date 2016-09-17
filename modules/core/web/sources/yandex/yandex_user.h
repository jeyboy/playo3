#ifndef YANDEX_USER_H
#define YANDEX_USER_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class User : public virtual Base {
            public:
                // https://music.yandex.ua/users/music.partners/playlists
                // curl 'https://music.yandex.ua/handlers/library.jsx?owner=music.partners&filter=tracks&sort=&dir=&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.3435709301969304' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=OhTbN201Z8L819ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1; _ym_visorc_1028356=b' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/users/music.partners/tracks' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/users/music.partners/tracks'
                // curl 'https://music.yandex.ua/handlers/library.jsx?owner=music.partners&filter=albums&sort=&dir=&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.3435709301969304' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=OhTbN201Z8L819ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1; _ym_visorc_1028356=b' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/users/music.partners/tracks' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/users/music.partners/tracks'
                // curl 'https://music.yandex.ua/handlers/library.jsx?owner=music.partners&filter=artists&sort=&dir=&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.3435709301969304' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=OhTbN201Z8L819ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1; _ym_visorc_1028356=b' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/users/music.partners/tracks' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/users/music.partners/tracks'
                // curl 'https://music.yandex.ua/handlers/library.jsx?owner=music.partners&filter=playlists&sort=&dir=&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.3435709301969304' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=OhTbN201Z8L819ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1; _ym_visorc_1028356=b' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/users/music.partners/tracks' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/users/music.partners/tracks'

                // curl 'https://music.yandex.ua/handlers/library.jsx?bare=1&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.1379279585175972' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://music.yandex.ua/' -H 'X-Retpath-Y: https://music.yandex.ua/' -H 'X-Requested-With: XMLHttpRequest' -H 'Cookie: Session_id=3:1474023377.5.0.1474023377854:F3CJsg:16.0|363617853.0.2|4:118655.726606.c8Y2XC5XsZJwkOLjRCP6VQofygY; yandexuid=2190563221452621883; L=RFwFBQVmBgVyWVRkYU16VwFUVUNRenN4IyQzOyoWU19QRA==.1474023377.12651.37347.0faa78387be102abce08ff7197b92a62; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1#1789383377.udn.cDpqZXlib3kxOTg1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=OhTbN201Z8L819ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; ys=udn.cDpqZXlib3kxOTg1; _ym_isad=1; _ym_visorc_1028356=b; sessionid2=3:1474023377.5.0.1474023377854:F3CJsg:16.1|363617853.0.2|4:118655.305085.0npC_lcOUN-MsuMxUPyo_YPp5QQ; yandex_login=jeyboy1985' -H 'Connection: keep-alive'
                // curl 'https://music.yandex.ua/handlers/library.jsx?owner=jeyboy1985&filter=subscriptions&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.7583768040403811' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: yandexuid=2190563221452621883; L=YXF9YlkGa0VTVA9FfnsBfGJZTg9afkVbATwJI1gKQ2BABg==.1474087951.12653.367559.6fb904c98bf3de6c3c2859cd6d1178c2; yp=1788794542.multib.1#1789447951.udn.cDpqZXlib3kxOTg1#1474693065.szm.1:1920x1080:1920x968; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fplaylists%22%7D; yabs-vdrf=SpDnNIW57Yqy0pDnNEmOrC0y1pDnNWG5dJ5C1pDnNDm3LUrK1WDnN5GIw4Am0VjnNDm1Aa5G1hTbN201Z8L819ifNtWF8wa009ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; _ym_isad=1; Session_id=3:1474087951.5.0.1474087951638:F3CJsg:1e.0|363617853.0.2|4:118691.298409.D5oz1ab1k9vNPAJ40JDorve65D4; device_id="aff97816c34c903dcae394ba20c1b499028c14c94"; _ym_visorc_722884=b; _ym_visorc_1028356=b; sessionid2=3:1474087951.5.0.1474087951638:F3CJsg:1e.1|363617853.0.2|4:118691.670301.ocAZrgeDN7-qf2ocuArDP400Yk0; yandex_login=jeyboy1985; ys=udn.cDpqZXlib3kxOTg1' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/users/jeyboy1985/subscriptions' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Current-UID: 363617853' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/users/jeyboy1985/subscriptions'
            };
        }
    }
}

#endif // YANDEX_USER_H
