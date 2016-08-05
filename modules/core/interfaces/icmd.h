#ifndef ICMD
#define ICMD

#include "icmd_fields.h"

#include <qjsonvalue.h>
#include <qvariant.h>
#include <qhash.h>

namespace Core {
    class ICmd {
        enum ICmdMethods {
            cmd_mtd_unknown = 0,

        };

        QHash<Qstring, ICmdMethods> _methods;

        QUrl cmdToParams(const QString & cmd) { return QUrl(cmd); }

        QUrlQuery paramsToQuery(const std::initializer_list<std::pair<QString, QVariant> > & params) {
            QUrlQuery query;

            if (params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QVariant> >::const_iterator it = params.begin(); it != params.end(); ++it)
                    setParam(query, it -> first, it -> second);

            return query;
        }

        QString paramsToStr(const std::initializer_list<std::pair<QString, QVariant> > & params) {
            return paramsToQuery(params).toString();
        }
    public:
        ICmd() {
//            _methods.insert();
        }

        QJsonValue run(const QString & cmd) {
            QUrl url = cmdToParams(cmd);
            switch(_methods.value(url.path(), cmd_mtd_unknown)) {
                case :;
                default: return QJsonArray();
            }
        }

        QJsonValue setByType(const QString & args) = 0;

        QJsonValue usersByName(const QString & args) = 0;
        QJsonValue usersByTrackLikes(const QString & args) = 0;
        QJsonValue usersByTrackReposting(const QString & args) = 0;
        QJsonValue userFollowings(const QString & args) = 0;
        QJsonValue userFollowers(const QString & args) = 0;

        QJsonValue groupsByTrack(const QString & args) = 0;
        QJsonValue groupsByUser(const QString & args) = 0;
        QJsonValue groupsByName(const QString & args) = 0;

        QJsonValue itemsSearch(const QString & args) = 0; // search of items with mixed types
        QJsonValue itemsByCollection(const QString & args) = 0; // get items with mixed types

        QJsonValue tracksSearch(const QString & args) = 0;
        QJsonValue trackRecommendations(const QString & args) = 0;
        QJsonValue tracksByArtist(const QString & args) = 0;
        QJsonValue tracksByTag(const QString & args) = 0;
        QJsonValue tracksByGroup(const QString & args) = 0;
        QJsonValue tracksByUser(const QString & args) = 0;
        QJsonValue tracksByUserLikes(const QString & args) = 0;
        QJsonValue tracksByAlbum(const QString & args) = 0;
        QJsonValue tracksByPlaylist(const QString & args) = 0;
        QJsonValue tracksByCollection(const QString & args) = 0;
        QJsonValue tracksByTuner(const QString & args) = 0;

        QJsonArray videoSearch(const QString & args) = 0;
        QJsonValue videoByUser(const QString & args) = 0;
        QJsonValue videoByPlaylist(const QString & args) = 0;

        // mixed types playlists
        QJsonValue playlistsByTag(const QString & args) = 0;
        QJsonValue playlistsByTrack(const QString & args) = 0;
        QJsonValue playlistsByPredicate(const QString & args) = 0;
        QJsonValue playlistsByUser(const QString & args) = 0;

        QJsonValue tracksPlaylistsByUser(const QString & args) = 0;
//        QJsonValue tracksPlaylistsByGroup(const QString & args) = 0;
    };
}

#endif // ICMD
