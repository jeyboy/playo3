#ifndef SOUNDCLOUD_ITRACK
#define SOUNDCLOUD_ITRACK

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class ITrack {
            protected:
                virtual QString trackUrl(const QString & track_id) = 0;
                virtual bool likeTrack(const QString & user_id, const QString & track_id) = 0;

                virtual QJsonObject trackInfo(const QString & track_id) = 0;
                virtual QJsonValue tracksInfo(const QStringList & track_ids) = 0;
                virtual QJsonValue trackRelations(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                virtual QJsonValue tracksSearch(const SearchLimit & limitations) = 0;

                virtual QJsonValue tracksByTag(const QString & tag, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                virtual QJsonValue tracksByGroup(const QString & group_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                virtual QJsonValue tracksByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                virtual QJsonValue tracksByUserLikes(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
            };
        }
    }
}

#endif // SOUNDCLOUD_ITRACK
