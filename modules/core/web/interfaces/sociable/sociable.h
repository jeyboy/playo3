#ifndef SOCIABLE
#define SOCIABLE

#include <qstringbuilder.h>

#include "modules/core/web/interfaces/sociable/friendable.h"
#include "modules/core/web/interfaces/sociable/groupable.h"

namespace Core {
    namespace Web {
        class Sociable : public Friendable, public Groupable {
        public:
            virtual ~Sociable() {}

            void setSociableLimitations(const bool & friend_by_id, const bool & friend_by_name,
                                      const bool & group_by_id, const bool & group_by_name)
            {
                setFriendSearchLimitations(friend_by_id, friend_by_name);
                setGroupSearchLimitations(group_by_id, group_by_name);
            }

            void fromJson(const QJsonObject & hash) {
                Friendable::fromJson(hash);
                Groupable::fromJson(hash);
            }
            void toJson(QJsonObject & hash) {
                Friendable::toJson(hash);
                Groupable::toJson(hash);
            }
        };
    }
}

#endif // SOCIABLE
