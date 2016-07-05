#ifndef SOUNDCLOUD_IGROUP
#define SOUNDCLOUD_IGROUP

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class IGroup {
            protected:
                virtual QJsonObject groupInfo(const QString & group_id) = 0;
                virtual QJsonValue groupsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                virtual QJsonValue groupsByUser(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                virtual QJsonValue groupsByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
            };
        }
    }
}

#endif // SOUNDCLOUD_IGROUP
