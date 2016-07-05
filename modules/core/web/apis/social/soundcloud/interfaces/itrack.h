#ifndef SOUNDCLOUD_ITRACK
#define SOUNDCLOUD_ITRACK

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class ITrack {
            protected:
                QString trackUrl(const QString & track_id) = 0;
                bool likeTrack(const QString & user_id, const QString & track_id) = 0;

                QJsonObject trackInfo(const QString & track_id) = 0;
                QJsonValue tracksInfo(const QStringList & track_ids) = 0;
                QJsonValue trackRelations(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                QJsonValue tracksSearch(const SearchLimit & limitations) = 0;

                QJsonValue tracksByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                QJsonValue tracksByGroup(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                QJsonValue tracksByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                QJsonValue tracksByUserLikes(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
            };
        }
    }
}

#endif // SOUNDCLOUD_ITRACK
