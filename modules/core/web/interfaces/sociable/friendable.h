#ifndef FRIENDABLE
#define FRIENDABLE

#include "linkable_list.h"

namespace Core {
    namespace Web {
        class Friendable : public LinkableList {
            QString jsonToken() const { return QStringLiteral("friends"); }
        protected:
            bool search_by_friend_id, search_by_friend_name;
        public:
            Friendable() : search_by_friend_id(false), search_by_friend_name(false) {}
            virtual ~Friendable() {}

            void setFriendSearchLimitations(const bool & by_id, const bool & by_name) {
                search_by_friend_id = by_id;
                search_by_friend_name = by_name;
            }

            bool hasSearchByFriendId() { return search_by_friend_id; }
            bool hasSearchByFriendName() { return search_by_friend_name; }

            virtual QList<Linkable> findFriendsById(const QString & /*uid*/) { return QList<Linkable>(); }
            virtual QList<Linkable> findFriendsByName(const QString & /*name*/) { return QList<Linkable>(); }

            inline void addFriend(const Linkable & obj) { addLinkable(obj); }

            inline QList<Linkable> friendsList() const { return linkablesList(); }
            inline void clearFriends() { clearLinkables(); }
        };
    }
}

#endif // FRIENDABLE
