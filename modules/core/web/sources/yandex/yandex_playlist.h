#ifndef YANDEX_PLAYLIST_H
#define YANDEX_PLAYLIST_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Playlist : public virtual Base { // has tags array in response
            public:
                // curl 'https://music.yandex.ua/handlers/playlists-list.jsx' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' -H 'Referer: https://music.yandex.ua/users/music.partners/playlists' -H 'X-Current-UID: 363617853' -H 'X-Retpath-Y: https://music.yandex.ua/users/music.partners/playlists' -H 'X-Requested-With: XMLHttpRequest' -H 'Cookie: yandexuid=2190563221452621883; L=YXF9YlkGa0VTVA9FfnsBfGJZTg9afkVbATwJI1gKQ2BABg==.1474087951.12653.367559.6fb904c98bf3de6c3c2859cd6d1178c2; yp=1788794542.multib.1#1789447951.udn.cDpqZXlib3kxOTg1#1474693065.szm.1:1920x1080:1920x968#1476680947.los.1#1476680947.losc.0; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fmusic.partners%2Fplaylists%22%7D; yabs-vdrf=SpDnNIW57Yqy0pDnNEmOrC0y1pDnNWG5dJ5C1pDnNDm3LUrK1WDnN5GIw4Am0VjnNDm1Aa5G1hTbN201Z8L819ifNtWF8wa009ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; Session_id=3:1474087951.5.0.1474087951638:F3CJsg:1e.0|363617853.0.2|4:118691.298409.D5oz1ab1k9vNPAJ40JDorve65D4; sessionid2=3:1474087951.5.0.1474087951638:F3CJsg:1e.1|363617853.0.2|4:118691.670301.ocAZrgeDN7-qf2ocuArDP400Yk0; yandex_login=jeyboy1985; device_id="aff97816c34c903dcae394ba20c1b499028c14c94"; ys=udn.cDpqZXlib3kxOTg1#wprid.1474088961765503-17439903023986964095193917-sas1-5468; _ym_isad=1' -H 'Connection: keep-alive' --data 'ids=1687&owner=139954184&sign=u98cd41597697b6707717f67edc5d24f0&lang=ru&external-domain=music.yandex.ua&overembed=false'
                QJsonValue playlistsInfo(const QUrlQuery & args) {
                    return playlistsInfo(
                        args.queryItemValue(CMD_OWNER),
                        args.queryItemValue(CMD_ID)
                    );
                }
                QJsonValue playlistsInfo(const QString & owner_uid, const QString & playlist_ids) {
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            int limiter = playlist_ids.size() / YANDEX_IDS_PER_REQUEST + (int)(playlist_ids.size() % YANDEX_IDS_PER_REQUEST != 0);
                            for(int step = 0; step < limiter; step++) {
                                std::initializer_list<std::pair<QString, QVariant> > params = {
                                    { LSTR("ids"), (QStringList)playlist_ids.mid(step * YANDEX_IDS_PER_REQUEST, YANDEX_IDS_PER_REQUEST) },
                                    { tkn_owner, owner_uid }
                                };

                                saRequest(
                                    baseUrlStr(qst_site, LSTR("playlists-list.jsx"), params),
                                    call_type_json, &block_content, proc_json_patch, IQUERY_DEF_FIELDS, call_method_post
                                );
                            }
                        break;}

                        default: Logger::obj().write(name(), "playlists info is not accessable", Logger::log_error);
                    }

                    preparePlaylists(block_content);
                    return prepareBlock(dmt_audio_set, block_content);
                }

                QJsonValue playlistsSearch(const QUrlQuery & args) { return playlistsSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue playlistsSearch(const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, LSTR("music-search.jsx"),
                                     {
                                         {LSTR("text"), limits.predicate},
                                         {LSTR("type"), tkn_playlists}
                                     }
                                ),
                                call_type_json, pageRules(tkn_page, limits.start_offset, limits.requests_limit, limits.items_limit),
                                0, proc_json_extract, QStringList() << tkn_playlists << tkn_items
                            );

                            preparePlaylists(response.content);
                        break;}

                        default: Logger::obj().write(name(), "PLAYLISTS SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio_set, cmd_mtd_playlists_search, response, limits);
                }

                QJsonValue playlistsByUser(const QUrlQuery & args) { return playlistsByUser(args.queryItemValue(CMD_ID)); }
                QJsonValue playlistsByUser(const QString & user_id) {
                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("library.jsx"),
                            {
                                {tkn_owner, user_id},
                                {tkn_filter, tkn_playlists}
                            }
                        ),
                        call_type_json
                    );

                    return preparePlaylistsBlock(info);
                }
            };
        }
    }
}

#endif // YANDEX_PLAYLIST_H
