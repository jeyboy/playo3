#ifndef ICMD
#define ICMD

#include "icmd_fields.h"

#include <qjsonobject.h>
#include <qvariant.h>
#include <qhash.h>

namespace Core {
    class ICmd {
        enum ICmdMethods {
            cmd_mtd_unknown = 0,

            cmd_mtd_set_by_type,

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
            cmd_mtd_tracks_playlists_by_group
        };

        QUrl cmdToParams(const QString & cmd) { return QUrl(cmd); }

        QUrlQuery paramsToQuery(const std::initializer_list<std::pair<QString, QString> > & params) {
            QUrlQuery query;

            if (params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QString> >::const_iterator it = params.begin(); it != params.end(); ++it)
                    query.addQueryItem(it -> first, it -> second);

            return query;
        }

        QString paramsToStr(const std::initializer_list<std::pair<QString, QString> > & params) {
            return paramsToQuery(params).toString();
        }
    public:
        QJsonValue run(const QString & cmd) {
            QUrl url = cmdToParams(cmd);
            switch((ICmdMethods)url.path().toInt()) {
//                case :;
                default: return QJsonObject();
            }
        }

        virtual QJsonValue setByType(const QString & /*args*/) { return QJsonObject(); }

        virtual QJsonValue takeSet(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue loadPlaylist(const QVariantMap & /*attrs*/) { return QJsonObject(); } // TODO: rebuild on QString attrs

        virtual QJsonValue usersByName(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue usersByTrackLikes(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue usersByTrackReposting(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userFollowings(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userFollowers(const QString & /*args*/) { return QJsonObject(); }

        virtual QJsonValue groupsByTrack(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groupsByUser(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groupsByName(const QString & /*args*/) { return QJsonObject(); }

        virtual QJsonValue itemsSearch(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue itemsByCollection(const QString & /*args*/) { return QJsonObject(); }

        virtual QJsonValue tracksSearch(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue trackRecommendations(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByArtist(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByTag(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByGroup(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByUser(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByUserLikes(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByAlbum(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByPlaylist(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByCollection(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue tracksByTuner(const QString & /*args*/) { return QJsonObject(); }

        virtual QJsonValue videoSearch(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoByUser(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoByPlaylist(const QString & /*args*/) { return QJsonObject(); }

        // mixed types playlists
        virtual QJsonValue playlistsByTag(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByTrack(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByPredicate(const QString & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByUser(const QString & /*args*/) { return QJsonObject(); }

        virtual QJsonValue tracksPlaylistsByUser(const QString & /*args*/) { return QJsonObject(); }
//        virtual QJsonValue tracksPlaylistsByGroup(const QString & /*args*/) { return QJsonObject(); }
    };
}

#endif // ICMD
