#ifndef GROUPABLE
#define GROUPABLE

#include "linkable_list.h"

namespace Core {
    namespace Web {
        class Groupable : public LinkableList {
            QString jsonToken() const { return QLatin1String("groups"); }
        protected:
            bool search_by_group_id, search_by_group_name;
        public:
            Groupable() : search_by_group_id(false), search_by_group_name(false) {}
            virtual ~Groupable() {}

            void setGroupSearchLimitations(const bool & by_id, const bool & by_name) {
                search_by_group_id = by_id;
                search_by_group_name = by_name;
            }
            bool hasSearchByGroupId() { return search_by_group_id; }
            bool hasSearchByGroupName() { return search_by_group_name; }

            virtual QList<Linkable> findGroupsById(const QString & /*uid*/) { return QList<Linkable>(); }
            virtual QList<Linkable> findGroupsByName(const QString & /*name*/) { return QList<Linkable>(); }

            inline void addGroup(const Linkable & obj) { addLinkable(obj); }

            inline QList<Linkable> groupsList() const { return linkablesList(); }
            inline void clearGroups() { clearLinkables(); }
        };
    }
}


#endif // GROUPABLE
