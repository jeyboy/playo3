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

                // QJsonValue userFollowings(const QUrlQuery & /*args*/) { return QJsonObject(); }
                QJsonValue userFollowings(const QString & user_id) {
                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("library.jsx"),
                            {
                                {tkn_owner, user_id},
                                {tkn_filter, LSTR("subscriptions")}
                            }
                        ),
                        call_type_json
                    );

                    QJsonArray users = JSON_ARR(info, LSTR("subscriptions"));
                    return prepareBlock(dmt_user, prepareUsers(users));
                }

                // QJsonValue userFollowers(const QUrlQuery & /*args*/) { return QJsonObject(); }
                QJsonValue userFollowers(const QString & user_id) {
                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("library.jsx"),
                            {
                                {tkn_owner, user_id},
                                {tkn_filter, LSTR("subscribers")}
                            }
                        ),
                        call_type_json
                    );

                    QJsonArray users = JSON_ARR(info, LSTR("subscribers"));
                    return prepareBlock(dmt_user, prepareUsers(users));
                }
            };
        }
    }
}

#endif // YANDEX_USER_H
