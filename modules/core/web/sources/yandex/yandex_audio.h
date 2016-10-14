#ifndef YANDEX_AUDIO
#define YANDEX_AUDIO

#include "yandex_defines.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Audio : public virtual Base {
            public:
                QString audioUrl(const QString & track_id) {
                    // codec, bitrate, src, gain
                    QJsonObject obj = Manager::prepare() -> jsonGet(
                        baseUrlStr(qst_site_alt1, LSTR("track/%1/download").arg(track_id), {})
                    );

                    obj = Manager::prepare() -> jsonGet(
                        QString(JSON_STR(obj, LSTR("src")) % LSTR("&format=json"))
                    );

                    QString path = JSON_STR(obj, LSTR("path"));

                    return LSTR("https://%1/get-mp3/%2/%3%4?track-id=%5&play=false&").arg(
                        JSON_STR(obj, LSTR("host")), calcKey(path, JSON_CSTR(obj, LSTR("s"))),
                        JSON_CSTR(obj, LSTR("ts")), path, track_id.split(':').first()
                    );
                }

                // track id should be eq to track_id:album_id
                QString audioLyric(const QString & track_id) { //TODO: write me
                    QJsonObject json = sRequest(
                        baseUrlStr(qst_site, LSTR("track.jsx"), {{LSTR("track"), track_id}}),
                        call_type_json, 0, proc_none
                    );

                    return QString();
                }

                QJsonValue audioInfo(const QString & track_id) {
                    QJsonObject info = audioInfo(QStringList() << track_id).toObject();
                    QJsonArray content = info.value(tkn_content).toArray();
                    return content.isEmpty() ? QJsonObject() : content[0].toObject();
                }

                QJsonValue audioInfo(const QUrlQuery & args) {
                    return audioInfo(args.queryItemValue(CMD_ID).split(','));
                }
                QJsonValue audioInfo(const QStringList & track_ids) {
                    SourceFlags perm = permissions(sf_audio_by_id);
                    QJsonArray block_content;

                    switch(perm) {
                        case sf_site:
                        case sf_api: {
                            int limiter = track_ids.size() / YANDEX_IDS_PER_REQUEST + (int)(track_ids.size() % YANDEX_IDS_PER_REQUEST != 0);
                            for(int step = 0; step < limiter; step++) {
                                std::initializer_list<std::pair<QString, QVariant> > params = {
                                    { LSTR("entries"), (QStringList)track_ids.mid(step * YANDEX_IDS_PER_REQUEST, YANDEX_IDS_PER_REQUEST) },
                                    { LSTR("strict"), LSTR("true") }
                                };

                                saRequest(
                                    baseUrlStr(qst_site, LSTR("track-entries.jsx"), params),
                                    call_type_json, &block_content, proc_json_patch, IQUERY_DEF_FIELDS, call_method_post
                                );
                            }
                        break;}

                        default: Logger::obj().write(name(), "track info is not accessable", Logger::log_error);
                    }

                    prepareTracks(block_content);
                    return prepareBlock(dmt_audio, block_content);
                }

                QJsonValue audioSearch(const QUrlQuery & args) { return audioSearch(SearchLimit::fromICmdParams(args)); }
                QJsonValue audioSearch(const SearchLimit & limits) {
                    SourceFlags perm = permissions(sf_audio_by_title);
                    QueriableResponse response;

                    switch(perm) {
                        case sf_api:
                        case sf_site: {
                            response = pRequest(
                                baseUrlStr(qst_site, LSTR("music-search.jsx"),
                                     {
                                         {LSTR("text"), limits.predicate},
                                         {LSTR("type"), tkn_tracks}
                                     }
                                ),
                                call_type_json, pageRules(tkn_page, limits.start_offset, limits.requests_limit, limits.items_limit),
                                0, proc_json_extract, QStringList() << tkn_tracks << tkn_items
                            );

                            prepareTracks(response.content);
                        break;}

                        default: Logger::obj().write(name(), "TRACK SEARCH is not accessable", Logger::log_error);
                    }

                    return prepareBlock(dmt_audio, cmd_mtd_audio_search, response, limits);
                }

                QJsonValue audioByArtist(const QUrlQuery & args) { return audioByArtist(args.queryItemValue(CMD_ID)); }
                QJsonValue audioByArtist(const QString & artist_id, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("artist.jsx"),
                            {
                                {LSTR("artist"), artist_id},
                                {LSTR("what"), LSTR("tracks")}
                            }
                        ),
                        call_type_json
                    );

                    return prepareTracksBlock(info, block_params);
                }

                QJsonValue audioByAlbum(const QUrlQuery & args) {
                    return audioByAlbum(
                        args.queryItemValue(CMD_ID)
                    );
                }
                QJsonValue audioByAlbum(const QString & album_id) {
                    // curl 'https://music.yandex.ua/handlers/album.jsx?album=2211661&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.36949077823991205' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Encoding: gzip, deflate, br' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/genre/country/albums?page=109' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https://music.yandex.ua/genre/country/albums?page=109'

                    QJsonArray volumes = saRequest(
                        baseUrlStr(qst_site, LSTR("album.jsx"), {{LSTR("album"), album_id}}),
                        call_type_json, 0, proc_json_extract, QStringList() << LSTR("volumes")
                    );

                    QJsonArray tracks;
                    for(QJsonArray::Iterator volume = volumes.begin(); volume != volumes.end(); volume++)
                        QueriableArg::arrAppend(tracks, (*volume).toArray());

                    return prepareBlock(dmt_audio, tracks);
                }

                QJsonValue audioByPlaylist(const QUrlQuery & args) {
                    return audioByPlaylist(
                        args.queryItemValue(CMD_OWNER),
                        args.queryItemValue(CMD_ID)
                    );
                }
                QJsonValue audioByPlaylist(const QString & owner_uid, const QString & kinds) {
                    // curl 'https://music.yandex.ua/handlers/playlist.jsx?owner=music.partners&kinds=1767&light=true&lang=uk&external-domain=music.yandex.ua&overembed=false&ncrnd=0.6619828515219549' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json, text/javascript, */*; q=0.01' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Referer: https://music.yandex.ua/genre/metal' -H 'X-Retpath-Y: https://music.yandex.ua/genre/metal' -H 'X-Requested-With: XMLHttpRequest'

                    //INFO: yandex server is crashed if we split params to payload!!!! wtf?
                    QJsonObject obj = sRequest(
                        baseUrlStr(qst_site, LSTR("playlist.jsx"),
                            {
                                {tkn_owner, owner_uid},
                                {LSTR("kinds"), kinds},
                                {LSTR("light"), LSTR("true")}
                            }
                        ),
                        call_type_json, 0, proc_json_extract,
                        QStringList() << tkn_playlist,
                        call_method_post, Headers(), 0, false
                    );

                    return prepareTracksBlock(obj);
                }

                QJsonValue audioByUser(const QUrlQuery & args) { return audioByUser(args.queryItemValue(CMD_ID)); }
                QJsonValue audioByUser(const QString & user_id) {
                    QJsonObject info = sRequest(
                        baseUrlStr(
                            qst_site, LSTR("library.jsx"),
                            {
                                {tkn_owner, user_id},
                                {tkn_filter, tkn_tracks}
                            }
                        ),
                        call_type_json
                    );

                    return prepareTracksBlock(info);
                }
            };
        }
    }
}

#endif // YANDEX_AUDIO
