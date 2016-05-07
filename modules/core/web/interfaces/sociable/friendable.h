#ifndef FRIENDABLE
#define FRIENDABLE

#include "linkable_list.h"

namespace Core {
    namespace Web {
        class Friendable : public LinkableList {
            QString jsonToken() const { return QStringLiteral("friends"); }
        public:
//            virtual QList<Linkable> findFriendById(const QString & uid) = 0;
//            virtual QList<Linkable> findFriendByName(const QString & name) = 0;

            inline void addFriend(const Linkable & obj) { addLinkable(obj); }

            inline QHash<QString, Linkable> friendsList() const { return linkablesList(); }
            inline void clearFriends() { clearLinkables(); }
        };
    }
}

#endif // FRIENDABLE
