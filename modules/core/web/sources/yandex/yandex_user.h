#ifndef YANDEX_USER_H
#define YANDEX_USER_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class User : public virtual Base {
            public:
                // curl 'https://music.yandex.ua/handlers/user-shuffle.jsx?owner=RollingStoneRussia&lang=ru&external-domain=music.yandex.ua&overembed=false&ncrnd=0.7786232729448537' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/users/RollingStoneRussia/subscriptions' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Current-UID: 363617853' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/users/RollingStoneRussia/subscriptions'
                // curl 'https://music.yandex.ua/handlers/library.jsx?bare=1&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.1379279585175972' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://music.yandex.ua/' -H 'X-Retpath-Y: https://music.yandex.ua/' -H 'X-Requested-With: XMLHttpRequest'

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
