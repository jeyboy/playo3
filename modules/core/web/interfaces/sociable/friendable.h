#ifndef FRIENDABLE
#define FRIENDABLE

#include "linkable_list.h"

namespace Core {
    namespace Web {
        class Friendable : public LinkableList {
            QString jsonToken() const { return QStringLiteral("friends"); }
        public:
            virtual QList<Linkable> findFriendsById(const QString & /*uid*/) { return QList<Linkable>(); }
            virtual QList<Linkable> findFriendsByName(const QString & /*name*/) { return QList<Linkable>(); }

            inline void addFriend(const Linkable & obj) { addLinkable(obj); }

            inline QHash<QString, Linkable> friendsList() const { return linkablesList(); }
            inline void clearFriends() { clearLinkables(); }
        };
    }
}

#endif // FRIENDABLE
