#ifndef SOUNDCLOUD_IUSER
#define SOUNDCLOUD_IUSER

#include "defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class IUser {
            protected:
                QJsonValue usersById(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                QJsonValue usersByName(const QString & name, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;

                QJsonValue userFollowings(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
                QJsonValue userFollowers(const QString & user_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) = 0;
            };
        }
    }
}

#endif // SOUNDCLOUD_IUSER
