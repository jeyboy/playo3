#ifndef SOUNDCLOUD_REQUEST_SITE
#define SOUNDCLOUD_REQUEST_SITE

#include "defines.h"

#include "auth.h"
#include "group.h"
#include "playlist.h"
#include "set.h"
#include "track.h"
#include "user.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class RequestSite : public SiteAuth, public SiteGroup, public SitePlaylist,
                    public SiteSet, public SiteTrack, public SiteUser {
            protected:
                // standart step for all search requests is 10

                // mixed search with autocomplete - users / groups/ tracks
                //'https://api-v2.soundcloud.com/search/autocomplete?q=s&queries_limit=0&results_limit=10&limit=10&offset=0&linked_partitioning=1'

                // mixed search - users / groups/ tracks
                // curl 'https://api-v2.soundcloud.com/search?q=dubstep&sc_a_id=184de48a-5c8d-4ef7-908a-b4602d2a667d&facet=model&user_id=301086-912545-767108-258358&client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1466770454' -H 'Host: api-v2.soundcloud.com' -H 'DNT: 1' -H 'Referer: https://soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive' -H 'Pragma: no-cache' -H 'Cache-Control: no-cache'



//                likers of track

//                curl 'https://api-v2.soundcloud.com/tracks/248496843/likers?offset=1467491261224612&limit=6&client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1467192015' -H 'Host: api-v2.soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive'

//                reposters of track

//                curl 'https://api.soundcloud.com/e1/tracks/248496843/reposters?app_version=1467192015&client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=6&linked_partitioning=1&offset=18' -H 'Host: api.soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive'

//                user

//                curl 'https://api-v2.soundcloud.com/users/53128020/tracks?representation=&client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=20&offset=0&linked_partitioning=1&app_version=1467192015' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0'

//                curl 'https://api-v2.soundcloud.com/users/53128020/playlists?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1467192015' -H 'Host: api-v2.soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive'


//                curl 'https://api-v2.soundcloud.com/stream/users/53128020?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=20&offset=0&linked_partitioning=1&app_version=1467192015' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0'

//                curl 'https://api-v2.soundcloud.com/users/53128020/likes?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1467192015' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0'

//                curl 'https://api.soundcloud.com/users/53128020/followings?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=3&offset=0&linked_partitioning=1&app_version=1467192015' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0'

//                curl 'https://api.soundcloud.com/users/53128020/followers?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=12&offset=0&linked_partitioning=1&app_version=1467192015' -H 'Host: api.soundcloud.com' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:47.0) Gecko/20100101 Firefox/47.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Connection: keep-alive'
            };
        }
    }
}

#endif // SOUNDCLOUD_REQUEST_SITE
