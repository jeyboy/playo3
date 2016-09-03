#ifndef SOUNDCLOUD_STREAM
#define SOUNDCLOUD_STREAM

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Stream : public Base {
            public:
                // artist based station
                //curl 'https://api-v2.soundcloud.com/stations/artist_recommended?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1472654786' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'origin: https://soundcloud.com'

                // streams by ids splited by comma
                //curl 'https://api-v2.soundcloud.com/stations?urns=soundcloud%3Atrack-stations%3A194303455%2Csoundcloud%3Atrack-stations%3A216648507%2Csoundcloud%3Atrack-stations%3A241788972%2Csoundcloud%3Atrack-stations%3A268174503%2Csoundcloud%3Atrack-stations%3A274566663%2Csoundcloud%3Atrack-stations%3A274793736%2Csoundcloud%3Atrack-stations%3A277352869%2Csoundcloud%3Atrack-stations%3A277729275%2Csoundcloud%3Atrack-stations%3A277855506&client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&app_version=1472742050' -H 'Accept: application/json, text/javascript, */*; q=0.1' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'Content-Type: application/json' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Origin: https://soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0'

                QJsonValue streamsRecommendations(const QUrlQuery & args) {
                    return streamsRecommendations(
                        args.queryItemValue(CMD_OFFSET).toInt(),
                        args.queryItemValue(CMD_ITEMS_LIMIT).toInt()
                    );
                }

                QJsonValue streamsRecommendations(int offset = 0, int limit = 25) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    // curl 'https://api-v2.soundcloud.com/stations/artist_recommended?client_id=02gUJC0hH2ct1EGOcYXQIzRFU91c72Ea&limit=10&offset=0&linked_partitioning=1&app_version=1472654786' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Authorization: OAuth 1-138878-99021496-f2129f62c04ca7' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: api-v2.soundcloud.com' -H 'Referer: https://soundcloud.com/' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'origin: https://soundcloud.com'
                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site_alt1, QStringLiteral("stations/artist_recommended"), {}),
                                call_type_json, rules(offset, qMin(limit, 50)), 0,
                                proc_json_patch, COLLECTION_FIELDS, call_method_get, headers()
                            );

                            prepareStreams(response.content);
                         break;}

                        default: Logger::obj().write("Soundcloud", "STREAMS RECOMMENDATIONS is not accessable", true);
                    }

                    return prepareBlock(dmt_audio_set, cmd_mtd_streams_recommendations, response);
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_STREAM
