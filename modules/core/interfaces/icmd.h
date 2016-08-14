#ifndef ICMD
#define ICMD

#include "icmd_fields.h"

#include <qjsonobject.h>
#include <qvariant.h>
#include <qhash.h>
#include <qregularexpression.h>

namespace Core {
    struct Cmd {
        static QUrlQuery paramsToQuery(const std::initializer_list<std::pair<QString, QString> > & params) {
            QUrlQuery query;

            if (params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QString> >::const_iterator it = params.begin(); it != params.end(); ++it)
                    query.addQueryItem(it -> first, it -> second);

            return query;
        }

        static QString paramsToStr(const std::initializer_list<std::pair<QString, QString> > & params) {
            return paramsToQuery(params).toString();
        }

        static QUrlQuery extractQuery(const QString & cmd) { return QUrlQuery(cmd.split('?').last()); }

        Cmd() {}

        Cmd(const QString & cmd) {
            QStringList parts = cmd.split(QRegularExpression("[\\/\\?]"), QString::SkipEmptyParts);

            source_type = parts[0].toInt();
            mtd = parts[1].toInt();
            attrs = QUrlQuery(parts[2]);
        }

        Cmd(const int source_type, const int & mtd, const QUrlQuery & attrs)
            : source_type(source_type), mtd(mtd), attrs(attrs) {}

        Cmd(const int source_type, const int & mtd, const std::initializer_list<std::pair<QString, QString> > & params)
            : source_type(source_type), mtd(mtd), attrs(paramsToQuery(params)) {}

        static Cmd build(const int source_type, const int & mtd, const std::initializer_list<std::pair<QString, QString> > & params) {
            return Cmd(source_type, mtd, paramsToQuery(params));
        }

        static Cmd build(const int source_type, const int & mtd, const QUrlQuery & attrs) {
            return Cmd(source_type, mtd, attrs);
        }

        Cmd * setAttrs(const std::initializer_list<std::pair<QString, QString> > & params) {
            attrs = paramsToQuery(params);
            return this;
        }

        QString toString() {
            return QStringLiteral("%1/%2?%3").arg(
                QString::number(source_type),
                QString::number(mtd),
                attrs.toString()
            );
        }

        int source_type;
        int mtd;

        QUrlQuery attrs;
    };

    class ICmd {
    public:
        enum ICmdMethods {
            cmd_mtd_unknown = 0,

            cmd_mtd_set_by_type,
            cmd_mtd_open_set,
            cmd_mtd_load_set_data,

            cmd_mtd_users_by_name,
            cmd_mtd_users_by_track_likes,
            cmd_mtd_users_by_track_reposting,
            cmd_mtd_user_followings,
            cmd_mtd_user_followers,

            cmd_mtd_groups_by_track,
            cmd_mtd_groups_by_user,
            cmd_mtd_groups_by_name,

            cmd_mtd_items_search, // search of items with mixed types
            cmd_mtd_items_by_collection, // get items with mixed types

            cmd_mtd_tracks_search,
            cmd_mtd_track_recommendations,
            cmd_mtd_tracks_by_artist,
            cmd_mtd_tracks_by_tag,
            cmd_mtd_tracks_by_group,
            cmd_mtd_tracks_by_user,
            cmd_mtd_tracks_by_user_likes,
            cmd_mtd_tracks_by_album,
            cmd_mtd_tracks_by_playlist,
            cmd_mtd_tracks_by_collection,
            cmd_mtd_tracks_by_tuner,

            cmd_mtd_video_search,
            cmd_mtd_video_by_user,
            cmd_mtd_video_by_playlist,

            // mixed types playlists
            cmd_mtd_playlists_by_tag,
            cmd_mtd_playlists_by_track,
            cmd_mtd_playlists_by_predicate,
            cmd_mtd_playlists_by_user,

            cmd_mtd_tracks_playlists_by_user,
            cmd_mtd_tracks_playlists_by_group,

            cmd_mtd_artists_search
        };

        QJsonValue run(const ICmdMethods & cmd, const QUrlQuery & params) {
            switch(cmd) {
                case cmd_mtd_set_by_type: return setByType(params);
                case cmd_mtd_open_set: return openSet(params);
                case cmd_mtd_load_set_data: return loadSetData(params);

                case cmd_mtd_users_by_name: return usersByName(params);
                case cmd_mtd_users_by_track_likes: return usersByTrackLikes(params);
                case cmd_mtd_users_by_track_reposting: return usersByTrackReposting(params);
                case cmd_mtd_user_followings: return userFollowings(params);
                case cmd_mtd_user_followers: return userFollowers(params);

                case cmd_mtd_groups_by_track: return groupsByTrack(params);
                case cmd_mtd_groups_by_user: return groupsByUser(params);
                case cmd_mtd_groups_by_name: return groupsByName(params);

                case cmd_mtd_items_search: return itemsSearch(params);
                case cmd_mtd_items_by_collection: return itemsByCollection(params);

                case cmd_mtd_tracks_search:  return tracksSearch(params);
                case cmd_mtd_track_recommendations: return trackRecommendations(params);
                case cmd_mtd_tracks_by_artist: return tracksByArtist(params);
                case cmd_mtd_tracks_by_tag: return tracksByTag(params);
                case cmd_mtd_tracks_by_group: return tracksByGroup(params);
                case cmd_mtd_tracks_by_user: return tracksByUser(params);
                case cmd_mtd_tracks_by_user_likes: return tracksByUserLikes(params);
                case cmd_mtd_tracks_by_album: return tracksByAlbum(params);
                case cmd_mtd_tracks_by_playlist: return tracksByPlaylist(params);
                case cmd_mtd_tracks_by_collection: return tracksByCollection(params);
                case cmd_mtd_tracks_by_tuner: return tracksByTuner(params);

                case cmd_mtd_video_search: return videoSearch(params);
                case cmd_mtd_video_by_user: return videoByUser(params);
                case cmd_mtd_video_by_playlist: return videoByPlaylist(params);

                case cmd_mtd_playlists_by_tag: return playlistsByTag(params);
                case cmd_mtd_playlists_by_track: return playlistsByTrack(params);
                case cmd_mtd_playlists_by_predicate: return playlistsByPredicate(params);
                case cmd_mtd_playlists_by_user: return playlistsByUser(params);

                case cmd_mtd_tracks_playlists_by_user: return tracksPlaylistsByUser(params);
                case cmd_mtd_tracks_playlists_by_group: return tracksPlaylistsByGroup(params);

                default: return QJsonObject();
            }
        }

        virtual QJsonValue setByType(const QUrlQuery & /*args*/) { return QJsonObject(); }

//        virtual QJsonValue takeSet(const QString & /*args*/) { return QJsonObject(); }
//        virtual QJsonValue loadPlaylist(const QVariantMap & /*attrs*/) { return QJsonObject(); } // TODO: rebuild on QString attrs

        virtual QJsonValue openSet(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue loadSetData(const QUrlQuery & /*args*/) { return QJsonObject(); } // TODO: rebuild on QString attrs

        virtual QJsonValue usersByName(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue usersByTrackLikes(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue usersByTrackReposting(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userFollowings(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userFollowers(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue groupsByTrack(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groupsByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groupsByName(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue itemsSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue itemsByCollection(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue tracksSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue trackRecommendations(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByArtist(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByTag(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByGroup(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByUserLikes(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByAlbum(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByPlaylist(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByCollection(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByTuner(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue videoSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoByPlaylist(const QUrlQuery & /*args*/) { return QJsonObject(); }

        // mixed types playlists
        virtual QJsonValue playlistsByTag(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByTrack(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByPredicate(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue tracksPlaylistsByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksPlaylistsByGroup(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue artistsSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
    };
}

#endif // ICMD
