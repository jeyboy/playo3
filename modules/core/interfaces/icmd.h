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

    enum CmdRelType : quint64 {
        crel_none                   = 0,

        crel_id                     = 1,
        crel_perma                  = (quint64(1)) << 1,
        crel_name                   = (quint64(1)) << 2,
        crel_genre                  = (quint64(1)) << 3,
        crel_category               = (quint64(1)) << 4,
        crel_tag                    = (quint64(1)) << 5,

        crel_like                   = (quint64(1)) << 6,
        crel_dislike                = (quint64(1)) << 7,
        crel_history                = (quint64(1)) << 8,
        crel_reposting              = (quint64(1)) << 9,


        crel_audio                  = (quint64(1)) << 10,
        crel_video                  = (quint64(1)) << 11,
        crel_artist                 = (quint64(1)) << 12,

        crel_album                  = (quint64(1)) << 13,
        crel_playlist               = (quint64(1)) << 14,
        crel_recommendation         = (quint64(1)) << 15,

        crel_collection             = (quint64(1)) << 16,
        crel_channel                = (quint64(1)) << 17,
        crel_stream                 = (quint64(1)) << 18,
        crel_tuner                  = (quint64(1)) << 19,

        crel_user                   = (quint64(1)) << 20,
        crel_group                  = (quint64(1)) << 21,

        crel_feed                   = (quint64(1)) << 22,
        crel_item                   = (quint64(1)) << 23,

        // 1 level

        crel_audio_album            = crel_audio | crel_album,
        crel_audio_playlist         = crel_audio | crel_playlist,
        crel_audio_stream           = crel_audio | crel_stream,
        crel_audio_channel          = crel_audio | crel_channel,
        crel_audio_recommendation   = crel_audio | crel_recommendation,

        crel_audio_like             = crel_audio | crel_like,
        crel_audio_reposting        = crel_audio | crel_reposting,

        crel_video_album            = crel_video | crel_album,
        crel_video_playlist         = crel_video | crel_playlist,
        crel_video_stream           = crel_video | crel_stream,
        crel_video_channel          = crel_video | crel_channel,
        crel_video_recommendation   = crel_video | crel_recommendation,

        crel_stream_recommendation  = crel_stream | crel_recommendation,

        crel_user_like              = crel_user | crel_like,
        crel_user_history           = crel_user | crel_history,

        // 2 level
    };

    class ICmd {
    public:
        enum ICmdMethods { // 1: result type // 2: relation type
            cmd_mtd_unknown = 0,

            cmd_mtd_open_set,
            cmd_mtd_set_by_type,

            cmd_mtd_load_container_data,

            cmd_mtd_info, //2: audio, video, artist, album, feed, playlist

            cmd_mtd_users, //2: id, name, audio_likes, audio_reposting
            cmd_mtd_groups, //2: id, name, user, audio
            cmd_mtd_followings, //2: user
            cmd_mtd_followers, //2: user
            cmd_mtd_audio, //2: artist, tag, group, user, user_likes, album, playlist, collection, tuner
            cmd_mtd_video, //2: user, playlist, category, user_rating, user_history, channel
            cmd_mtd_items, //2: collection

            cmd_mtd_search, //1: items, audio, video, playlist, artist, album

            cmd_mtd_channels, //1: audio, video //2: id, name, user, category
            cmd_mtd_streams, //1: audio, video //2: genre, artist
            cmd_mtd_recommendations, //1: audio, video, stream //2: user, artist, audio, video
            cmd_mtd_albums, //1: audio, video //2: artist,
            cmd_mtd_playlists, //1: audio, video //2: tag, audio, user, group ? predicate

            cmd_mtd_categories, //1: video, audio
            cmd_mtd_configure, //1: stream
        };

        QJsonValue run(const ICmdMethods & cmd, const QUrlQuery & params) {
            switch(cmd) {
                case cmd_mtd_open_set: return openSet(params);
                case cmd_mtd_set_by_type: return setByType(params);

                case cmd_mtd_load_container_data: return loadContainerData(params);

                case cmd_mtd_info: return info(params);
                case cmd_mtd_search: return search(params);


                case cmd_mtd_users: return users(params);
                case cmd_mtd_groups: return groups(params);
                case cmd_mtd_followings: return followings(params);
                case cmd_mtd_followers: return followers(params);
                case cmd_mtd_albums: return albums(params);
                case cmd_mtd_audio: return audio(params);
                case cmd_mtd_video: return video(params);
                case cmd_mtd_items: return items(params);

                case cmd_mtd_channels: return channels(params);
                case cmd_mtd_streams: return streams(params);
                case cmd_mtd_playlists: return playlists(params);
                case cmd_mtd_recommendations: return recommendations(params);

                case cmd_mtd_categories: return categories(params);
                case cmd_mtd_configure: return configure(params);










//                case cmd_mtd_users_by_id: return usersById(params);
//                case cmd_mtd_users_by_name: return usersByName(params);
//                case cmd_mtd_users_by_audio_likes: return usersByAudioLikes(params);
//                case cmd_mtd_users_by_audio_reposting: return usersByAudioReposting(params);
//                case cmd_mtd_user_followings: return userFollowings(params);
//                case cmd_mtd_user_followers: return userFollowers(params);
//                case cmd_mtd_user_audio_recommendations: return userAudioRecommendations(params);
//                case cmd_mtd_user_video_recommendations: return userVideoRecommendations(params);

//                case cmd_mtd_groups_by_id: return groupsById(params);
//                case cmd_mtd_groups_by_audio: return groupsByAudio(params);
//                case cmd_mtd_groups_by_user: return groupsByUser(params);
//                case cmd_mtd_groups_by_name: return groupsByName(params);

//                case cmd_mtd_items_search: return itemsSearch(params);
//                case cmd_mtd_items_by_collection: return itemsByCollection(params);

//                case cmd_mtd_audio_info: return audioInfo(params);
//                case cmd_mtd_audio_search: return audioSearch(params);
//                case cmd_mtd_audio_recommendations: return audioRecommendations(params);
//                case cmd_mtd_audio_by_artist: return audioByArtist(params);
//                case cmd_mtd_audio_by_tag: return audioByTag(params);
//                case cmd_mtd_audio_by_group: return audioByGroup(params);
//                case cmd_mtd_audio_by_user: return audioByUser(params);
//                case cmd_mtd_audio_by_user_likes: return audioByUserLikes(params);
//                case cmd_mtd_audio_by_album: return audioByAlbum(params);
//                case cmd_mtd_audio_by_playlist: return audioByPlaylist(params);
//                case cmd_mtd_audio_by_collection: return audioByCollection(params);
//                case cmd_mtd_audio_by_tuner: return audioByTuner(params);

//                case cmd_mtd_video_info: return videoInfo(params);
//                case cmd_mtd_video_search: return videoSearch(params);
//                case cmd_mtd_video_by_user: return videoByUser(params);
//                case cmd_mtd_video_by_playlist: return videoByPlaylist(params);
//                case cmd_mtd_video_by_category: return videoByCategory(params);
//                case cmd_mtd_video_recommendations: return videoRecommendations(params);
//                case cmd_mtd_video_by_user_rating: return videoByUserRating(params);
//                case cmd_mtd_video_by_user_history: return videoByUserHistory(params);
//                case cmd_mtd_video_by_video_channel: return videoByVideoChannel(params);


//                case cmd_mtd_video_channels_by_id: return videoChannelsById(params);
//                case cmd_mtd_video_channels_by_name: return videoChannelsByName(params);
//                case cmd_mtd_video_channels_by_user: return videoChannelsByUser(params);
//                case cmd_mtd_video_channels_by_category: return videoChannelsByCategory(params);

//                case cmd_mtd_playlists_info: return playlistsInfo(params);
//                case cmd_mtd_playlists_search: return playlistsSearch(params);
//                case cmd_mtd_playlists_by_tag: return playlistsByTag(params);
//                case cmd_mtd_playlists_by_audio: return playlistsByAudio(params);
//                case cmd_mtd_playlists_by_predicate: return playlistsByPredicate(params);
//                case cmd_mtd_playlists_by_user: return playlistsByUser(params);

//                case cmd_mtd_audio_playlists_by_user: return audioPlaylistsByUser(params);
//                case cmd_mtd_audio_playlists_by_group: return audioPlaylistsByGroup(params);

//                case cmd_mtd_video_playlists_by_user: return videoPlaylistsByUser(params);
//                case cmd_mtd_video_categories: return videoCategories(params);

//                case cmd_mtd_streams_recommendations: return streamsRecommendations(params);
//                case cmd_mtd_streams_by_genre: return streamsByGenre(params);
//                case cmd_mtd_streams_by_artist: return streamsByArtist(params);
//                case cmd_mtd_stream_configure: return streamConfigure(params);
////                case cmd_mtd_stream_configuration: return streamConfiguration(params);

//                case cmd_mtd_artist_info: return artistInfo(params);
//                case cmd_mtd_artists_search: return artistsSearch(params);
//                case cmd_mtd_artist_recommendations: return artistRecommendations(params);

//                case cmd_mtd_albums_info: return albumsInfo(params);
//                case cmd_mtd_albums_search: return albumsSearch(params);
//                case cmd_mtd_albums_by_artist: return albumsByArtist(params);

//                case cmd_mtd_feeds_info: return feedsInfo(params);

                default: return QJsonObject();
            }
        }

        virtual QJsonValue openSet(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue setByType(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue loadContainerData(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue info(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue search(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue users(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue groups(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue followings(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue followers(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue albums(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue audio(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue video(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue items(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue channels(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue streams(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue playlists(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue recommendations(const QUrlQuery & /*args*/) { return QJsonObject(); }

        virtual QJsonValue categories(const QUrlQuery & /*args*/) { return QJsonObject(); }
        virtual QJsonValue configure(const QUrlQuery & /*args*/) { return QJsonObject(); }

//        virtual QVariant feedsConfiguration(const QUrlQuery & /*args*/) { return QVariant(); }
    };
}

#endif // ICMD
