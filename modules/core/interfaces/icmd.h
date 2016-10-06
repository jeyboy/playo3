#ifndef ICMD
#define ICMD

#include "icmd_fields.h"

#include <qdebug.h>
#include <qjsonobject.h>
#include <qvariant.h>
#include <qhash.h>
#include <qregularexpression.h>
#include <qdatetime.h>
#include <qstringbuilder.h>

namespace Core {
    struct Cmd {
        static QString variantToStr(const QVariant & data) {
            QString res;

            switch(data.type()) {
                case QVariant::Int:
                    { res = QString::number(data.toInt()); break;}
                case QVariant::UInt:
                    { res = QString::number(data.toUInt()); break;}
                case QVariant::LongLong:
                    { res = QString::number(data.toLongLong()); break;}
                case QVariant::ULongLong:
                    { res = QString::number(data.toULongLong()); break;}
                case QVariant::Double:
                    { res = QString::number(data.toDouble()); break;}


                case QVariant::Map:
                    {
                        QVariantMap map = data.toMap();
                        for(QVariantMap::Iterator entry = map.begin(); entry != map.end(); entry++)
                            res += QStringLiteral(", ") % entry.key() % ':' % variantToStr(entry.value());

                        res = res.mid(2);

                    break;}
                case QVariant::List:
                    {
                        QVariantList map = data.toList();
                        for(QVariantList::Iterator entry = map.begin(); entry != map.end(); entry++)
                            res += QStringLiteral(", ") % variantToStr(*entry);

                        res = res.mid(2);

                    break;}
                case QVariant::StringList:
                    {
                        QStringList map = data.toStringList();
                        for(QStringList::Iterator entry = map.begin(); entry != map.end(); entry++)
                            res += QStringLiteral(", ") % (*entry);

                        res = res.mid(2);

                    break;}
                case QVariant::Hash:
                    {
                        QVariantHash map = data.toHash();
                        for(QVariantHash::Iterator entry = map.begin(); entry != map.end(); entry++)
                            res += QStringLiteral(", ") % entry.key() % ':' % variantToStr(entry.value());

                        res = res.mid(2);

                    break;}


                case QVariant::Url:
                    { res = data.toUrl().toString(); break;}
                case QVariant::Date:
                    { res = data.toDate().toString(); break;}
                case QVariant::Time:
                    { res = data.toTime().toString(); break;}
                case QVariant::DateTime:
                    { res = data.toDateTime().toString(); break;}


                case QVariant::Bool:
                    { res = data.toBool() ? QStringLiteral("true") : QStringLiteral("false"); break;}
                case QVariant::Char:
                    { res = data.toChar(); break;}
                default: res = data.toString();
            }

            return res;
        }

        static QUrlQuery paramsToQuery(const std::initializer_list<std::pair<QString, QVariant> > & params) {
            QUrlQuery query;
            paramsToQuery(query, params);
            return query;
        }
        static void paramsToQuery(QUrlQuery & query, const std::initializer_list<std::pair<QString, QVariant> > & params) {
            if (params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QVariant> >::const_iterator it = params.begin(); it != params.end(); ++it) {
                    switch (it -> second.type()) {
                        case QVariant::Int:             { query.addQueryItem(it -> first, QString::number(it -> second.toInt())); break;}
                        case QVariant::Double:          { query.addQueryItem(it -> first, QString::number(it -> second.toDouble())); break;}
                        case QVariant::String:          { query.addQueryItem(it -> first, it -> second.toString()); break;}
                        default: qDebug() << "paramsToQuery: unsupported type" << it -> second.type();
                    }
                }
        }

        static QString paramsToStr(const std::initializer_list<std::pair<QString, QVariant> > & params) {
            return paramsToQuery(params).toString();
        }

        static QUrlQuery extractQuery(const QString & cmd) { return QUrlQuery(cmd.split('?').last()); }

        Cmd() {}

        Cmd(const QString & cmd) {
            QStringList parts = cmd.split(QRegularExpression("[\\/\\?]"), QString::SkipEmptyParts);

            source_type = parts[0].toInt();
            mtd = parts[1].toInt();
            attrs = QUrlQuery(parts.length() > 2 ? parts[2] : QString());
        }

        Cmd(const int source_type, const int & mtd, const QUrlQuery & attrs)
            : source_type(source_type), mtd(mtd), attrs(attrs) {}

        Cmd(const int source_type, const int & mtd, const std::initializer_list<std::pair<QString, QVariant> > & params)
            : source_type(source_type), mtd(mtd), attrs(paramsToQuery(params)) {}

        static Cmd build(const int source_type, const int & mtd, const std::initializer_list<std::pair<QString, QVariant> > & params) {
            return Cmd(source_type, mtd, paramsToQuery(params));
        }

        static Cmd build(const int source_type, const int & mtd, const QUrlQuery & attrs) {
            return Cmd(source_type, mtd, attrs);
        }

        Cmd * setAttrs(const std::initializer_list<std::pair<QString, QVariant> > & params) {
            attrs = paramsToQuery(params);
            return this;
        }

        QString setAttrsAsStr(const std::initializer_list<std::pair<QString, QVariant> > & params) {
            attrs = paramsToQuery(params);
            return toString();
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
            cmd_mtd_open_set, //FIXME remove and replace calls with cmd_mtd_open_set
            cmd_mtd_load_set_data,

            cmd_mtd_users_by_id,
            cmd_mtd_users_by_name,
            cmd_mtd_users_by_audio_likes,
            cmd_mtd_users_by_audio_reposting,
            cmd_mtd_user_followings,
            cmd_mtd_user_followers,
            cmd_mtd_user_audio_recommendations,
            cmd_mtd_user_video_recommendations,

            cmd_mtd_groups_by_id,
            cmd_mtd_groups_by_audio,
            cmd_mtd_groups_by_user,
            cmd_mtd_groups_by_name,

            cmd_mtd_items_search, // search of items with mixed types
            cmd_mtd_items_by_collection, // get items with mixed types

            cmd_mtd_audio_info,
            cmd_mtd_audio_search,
            cmd_mtd_audio_recommendations,
            cmd_mtd_audio_by_artist,
            cmd_mtd_audio_by_tag,
            cmd_mtd_audio_by_group,
            cmd_mtd_audio_by_user,
            cmd_mtd_audio_by_user_likes,
            cmd_mtd_audio_by_album,
            cmd_mtd_audio_by_playlist,
            cmd_mtd_audio_by_collection,
            cmd_mtd_audio_by_tuner,

            cmd_mtd_video_search,
            cmd_mtd_video_recommendations,
            cmd_mtd_video_by_user,
            cmd_mtd_video_by_playlist,
            cmd_mtd_video_by_category,

            // mixed types playlists
            cmd_mtd_playlists_info,
            cmd_mtd_playlists_search,
            cmd_mtd_playlists_by_tag,
            cmd_mtd_playlists_by_audio,
            cmd_mtd_playlists_by_predicate,
            cmd_mtd_playlists_by_user,

            cmd_mtd_audio_playlists_by_user,
            cmd_mtd_audio_playlists_by_group,

            cmd_mtd_video_playlists_by_user,
            cmd_mtd_video_categories,

            cmd_mtd_streams_recommendations,
            cmd_mtd_streams_by_genre,
            cmd_mtd_stream_by_artist,
            cmd_mtd_stream_configure,
//            cmd_mtd_stream_config,

            cmd_mtd_artist_info,
            cmd_mtd_artists_search,
            cmd_mtd_artist_recommendations,

            cmd_mtd_albums_info,
            cmd_mtd_albums_search,
            cmd_mtd_albums_by_artist,

            cmd_mtd_feeds_info
        };

        QJsonValue run(const ICmdMethods & cmd, const QUrlQuery & params) {
            switch(cmd) {
                case cmd_mtd_set_by_type: return setByType(params);
                case cmd_mtd_open_set: return openSet(params);
                case cmd_mtd_load_set_data: return loadSetData(params);

                case cmd_mtd_users_by_id: return usersById(params);
                case cmd_mtd_users_by_name: return usersByName(params);
                case cmd_mtd_users_by_audio_likes: return usersByAudioLikes(params);
                case cmd_mtd_users_by_audio_reposting: return usersByAudioReposting(params);
                case cmd_mtd_user_followings: return userFollowings(params);
                case cmd_mtd_user_followers: return userFollowers(params);
                case cmd_mtd_user_audio_recommendations: return userAudioRecommendations(params);
                case cmd_mtd_user_video_recommendations: return userVideoRecommendations(params);

                case cmd_mtd_groups_by_id: return groupsById(params);
                case cmd_mtd_groups_by_audio: return groupsByAudio(params);
                case cmd_mtd_groups_by_user: return groupsByUser(params);
                case cmd_mtd_groups_by_name: return groupsByName(params);

                case cmd_mtd_items_search: return itemsSearch(params);
                case cmd_mtd_items_by_collection: return itemsByCollection(params);

                case cmd_mtd_audio_info: return audioInfo(params);
                case cmd_mtd_audio_search: return audioSearch(params);
                case cmd_mtd_audio_recommendations: return audioRecommendations(params);
                case cmd_mtd_audio_by_artist: return audioByArtist(params);
                case cmd_mtd_audio_by_tag: return audioByTag(params);
                case cmd_mtd_audio_by_group: return audioByGroup(params);
                case cmd_mtd_audio_by_user: return audioByUser(params);
                case cmd_mtd_audio_by_user_likes: return audioByUserLikes(params);
                case cmd_mtd_audio_by_album: return audioByAlbum(params);
                case cmd_mtd_audio_by_playlist: return audioByPlaylist(params);
                case cmd_mtd_audio_by_collection: return audioByCollection(params);
                case cmd_mtd_audio_by_tuner: return audioByTuner(params);

                case cmd_mtd_video_search: return videoSearch(params);
                case cmd_mtd_video_by_user: return videoByUser(params);
                case cmd_mtd_video_by_playlist: return videoByPlaylist(params);
                case cmd_mtd_video_by_category: return videoByCategory(params);

                case cmd_mtd_playlists_info: return playlistsInfo(params);
                case cmd_mtd_playlists_search: return playlistsSearch(params);
                case cmd_mtd_playlists_by_tag: return playlistsByTag(params);
                case cmd_mtd_playlists_by_audio: return playlistsByAudio(params);
                case cmd_mtd_playlists_by_predicate: return playlistsByPredicate(params);
                case cmd_mtd_playlists_by_user: return playlistsByUser(params);

                case cmd_mtd_audio_playlists_by_user: return audioPlaylistsByUser(params);
                case cmd_mtd_audio_playlists_by_group: return audioPlaylistsByGroup(params);

                case cmd_mtd_video_playlists_by_user: return videoPlaylistsByUser(params);
                case cmd_mtd_video_categories: return videoCategories(params);

                case cmd_mtd_streams_recommendations: return streamsRecommendations(params);
                case cmd_mtd_streams_by_genre: return streamsByGenre(params);
                case cmd_mtd_stream_by_artist: return streamByArtist(params);
                case cmd_mtd_stream_configure: return streamConfigure(params);
//                case cmd_mtd_stream_configuration: return streamConfiguration(params);

                case cmd_mtd_artist_info: return artistInfo(params);
                case cmd_mtd_artists_search: return artistsSearch(params);
                case cmd_mtd_artist_recommendations: return artistRecommendations(params);

                case cmd_mtd_albums_info: return albumsInfo(params);
                case cmd_mtd_albums_search: return albumsSearch(params);
                case cmd_mtd_albums_by_artist: return albumsByArtist(params);

                case cmd_mtd_feeds_info: return feedsInfo(params);

                default: return QJsonObject();
            }
        }

        virtual QJsonValue setByType(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue openSet(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue loadSetData(const QUrlQuery & /*args*/) { return QJsonObject(); } // TODO: rebuild on QString attrs

        virtual QJsonValue usersById(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue usersByName(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue usersByAudioLikes(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue usersByAudioReposting(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userFollowings(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userFollowers(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userAudioRecommendations(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue userVideoRecommendations(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue groupsById(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groupsByAudio(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groupsByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groupsByName(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue itemsSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue itemsByCollection(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue audioInfo(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioRecommendations(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByArtist(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByTag(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByGroup(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByUserLikes(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByAlbum(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByPlaylist(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByCollection(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioByTuner(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue videoSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoByPlaylist(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoByCategory(const QUrlQuery & /*args*/) { return QJsonObject(); }

        // mixed types playlists
        virtual QJsonValue playlistsInfo(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByTag(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByAudio(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByPredicate(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlistsByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue audioPlaylistsByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audioPlaylistsByGroup(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue videoPlaylistsByUser(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue videoCategories(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue streamsRecommendations(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue streamsByGenre(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue streamByArtist(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue streamConfigure(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QVariant streamConfiguration(const QUrlQuery & /*args*/) { return QVariant(); }

        virtual QJsonValue artistInfo(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue artistsSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue artistRecommendations(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue albumsInfo(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue albumsSearch(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue albumsByArtist(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QVariant feedsConfiguration(const QUrlQuery & /*args*/) { return QVariant(); }
        virtual QJsonValue feedsInfo(const QUrlQuery & /*args*/) { return QJsonObject(); }
    };
}

#endif // ICMD
