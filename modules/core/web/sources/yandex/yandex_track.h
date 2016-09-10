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

                //artist // similar // albums // alsoAlbums // tracks // radio // alsoAlbumIds // albumIds // concerts
                inline QString artistTracksUrl(const QString & artistId) { return url_site_v1 + QStringLiteral("artist.jsx?artist=%1&what=tracks&sort=&dir=").arg(artistId); }

                inline QString searchUrl(const QString & predicate, const QString & sources = QStringLiteral("all")) { return url_site_v1 + QStringLiteral("music-search.jsx?text=%1&type=%2").arg(predicate, sources); }
            };
        }
    }
}

#endif // YANDEX_TRACK_H
