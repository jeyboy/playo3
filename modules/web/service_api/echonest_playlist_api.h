#ifndef ECHONEST_PLAYLIST_API
#define ECHONEST_PLAYLIST_API

#include "iapi.h"

class EchonestPlaylistApi : public IApi {
    public:
        //type 	no 	no 	artist-radio, song-radio , genre-radio 	the type of the playlist to be generated. See below for details on each of the types - the type of playlist to be generated

        //    Playlist Types:

        //            artist-radio - plays songs for the given artists and similar artists
        //            song-radio - plays songs similar to the song specified.
        //            genre-radio - plays songs from artists matching the given genre


        inline QUrl playlistBasicUrl(QString & type, QStringList & artists, QStringList & genres, QStringList & songs_ids, int limit = 100) {
            QUrl url(baseUrl("playlist/basic"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, qMin(limit, requestLimit()), 0);

            setParam(query, "artist", artists);
            setParam(query, "genre", genres);
            setParam(query, "song_id", songs_ids);

            if (!type.isEmpty())
                setParam(query, "type", type);
            url.setQuery(query);
            return url;
        }

        QJsonArray playlistBasic(QString type = QString(), QStringList artists = QStringList(),
                                 QStringList genres = QStringList(), QStringList songs_ids = QStringList(), int limit = 100) {
            QJsonObject response;

            if (proceedQuery(playlistBasicUrl(type, artists, genres, songs_ids, limit), response))
                return response.value("response").toObject().value("songs").toArray();

            return QJsonArray();
        }

        //{
        //    "response": {
        //        "songs": [
        //            {
        //                "artist_foreign_ids": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:artist:7516"
        //                    }
        //                ],
        //                "artist_id": "AR633SY1187B9AC3B9",
        //                "artist_name": "Weezer",
        //                "id": "SOBSLVH12A8C131F38",
        //                "title": "Island In The Sun",
        //                "tracks": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:track:20637990",
        //                        "foreign_release_id": "7digital-US:release:1914387",
        //                        "id": "TRTXLYU13A79B0B112",
        //                        "preview_url": "http://previews.7digital.com/clips/34/20637990.clip.mp3",
        //                        "release_image": "http://cdn.7static.com/static/img/sleeveart/00/019/143/0001914387_200.jpg"
        //                    }
        //                ]
        //            },
        //            {
        //                "artist_foreign_ids": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:artist:94744"
        //                    }
        //                ],
        //                "artist_id": "ARQRQRA12131B4B5A8",
        //                "artist_name": "Cake",
        //                "id": "SOZXNWX131343927EA",
        //                "title": "Love You Madly",
        //                "tracks": [
        //                    {
        //                        "catalog": "7digital-US",
        //                        "foreign_id": "7digital-US:track:3156513",
        //                        "foreign_release_id": "7digital-US:release:282089",
        //                        "id": "TRDFWLB12E5ADFBA5F",
        //                        "preview_url": "http://previews.7digital.com/clips/34/3156513.clip.mp3",
        //                        "release_image": "http://cdn.7static.com/static/img/sleeveart/00/002/820/0000282089_200.jpg"
        //                    }
        //                ]
        //            }
        //        ],
        //        "status": {
        //            "code": 0,
        //            "message": "Success",
        //            "version": "4.2"
        //        }
        //    }
        //}
};

#endif // ECHONEST_PLAYLIST_API
