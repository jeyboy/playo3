#ifndef YANDEX_TRACK_H
#define YANDEX_TRACK_H

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Track : public Base {
            public:
                QString trackUrl(const QString & track_id) {
                    // codec, bitrate, src, gain
                    QJsonObject obj = Manager::prepare() -> jsonGet(
                        baseUrlStr(qst_site_alt1, LSTR("track/%1/download").arg(track_id), {})
                    );

                    obj = Manager::prepare() -> jsonGet(
                        QString(JSON_STR(obj, LSTR("src")) % LSTR("&format=json"))
                    );

                    QString path = JSON_STR(obj, LSTR("path"));

                    return QStringLiteral("https://%1/get-mp3/%2/%3%4?track-id=%5&play=false&").arg(
                        JSON_STR(obj, LSTR("host")), calcKey(path, JSON_CSTR(obj, LSTR("s"))),
                        JSON_CSTR(obj, LSTR("ts")), path, track_id.split(':').first()
                    );
                }

                QJsonValue trackInfo(const QString & track_id) {
                    QJsonObject info = tracksInfo(QStringList() << track_id).toObject();
                    QJsonArray content = info.value(tkn_content).toArray();
                    return content.isEmpty() ? QJsonObject() : content[0].toObject();
                }

                QJsonValue tracksInfo(const QStringList & track_ids) { // TODO: finish me
                    Permissions perm = permissions(pr_media_content);
                    QJsonArray block_content;

                    // array of { id, realId, title, fileSize, storageDir, durationMs, embedPlayback, artists: {id, name, decomposed: ["feat or vs", {id, name}, ..]}}

                    switch(perm) {
                        case perm_site:
                        case perm_api: {
                            QUrlQuery query;

                            for(QStringList::ConstIterator track_id = track_ids.constBegin(); track_id != track_ids.constEnd(); track_id++)
                                query.addQueryItem(LSTR("tracks[]"), *track_id);

                            Response * response = Manager::prepare() -> postFollowed(
                                baseUrlStr(
                                    qst_site_alt1, LSTR("tracks"), query
                                ), dntHeader()
                            );


                            int i = 0;
//                            prepareTracks(items, block_content);
                        break;}

                        default: Logger::obj().write("VK", "track info is not accessable", true);
                    }

                    return prepareBlock(dmt_audio, block_content);
                }


                QJsonValue tracksByAlbum(const QUrlQuery & args) {
                    return tracksByAlbum(
                        args.queryItemValue(CMD_ID)
                    );
                }
                QJsonValue tracksByAlbum(const QString & album_id) {
                    // curl 'https://music.yandex.ua/handlers/album.jsx?album=2211661&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.36949077823991205' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'Cookie: Session_id=noauth:1473434912; yandexuid=2190563221452621883; L=BVhXc19GQ2NeT2oAWUZyQF5TXUphWUsMPiwgLSgqXkgPeQ==.1473434711.12637.322461.43fa5d46fffc2065a3098115c2a5d7f2; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Fusers%2Fjeyboy1985%2Fartists%22%7D; yabs-vdrf=N9ifNtWF8wa009ifN0WEw4Am09ifNJ0GrC0y19ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; _ym_isad=1; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/genre/country/albums?page=109' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/genre/country/albums?page=109'

                    QJsonArray volumes = saRequest(
                        baseUrlStr(
                            qst_site, LSTR("album.jsx"),
                            {
                                {LSTR("album"), album_id}/*,
                                {LSTR("external-domain"), LSTR("music.yandex.ua")},
                                {LSTR("overembed"), LSTR("false")},
                                {LSTR("lang"), LSTR("uk")}*/
                            }
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << LSTR("volumes")
                    );

                    QJsonArray tracks;
                    for(QJsonArray::Iterator volume = volumes.begin(); volume != volumes.end(); volume++)
                        QueriableArg::arrAppend(tracks, (*volume).toArray());

                    return prepareBlock(dmt_audio, tracks);
                }






                //artist // similar // albums // alsoAlbums // tracks // radio // alsoAlbumIds // albumIds // concerts
                inline QString artistTracksUrl(const QString & artistId) { return url_site_v1 + QStringLiteral("artist.jsx?artist=%1&what=tracks&sort=&dir=").arg(artistId); }

                inline QString searchUrl(const QString & predicate, const QString & sources = QStringLiteral("all")) { return url_site_v1 + QStringLiteral("music-search.jsx?text=%1&type=%2").arg(predicate, sources); }
            };
        }
    }
}

#endif // YANDEX_TRACK_H
