#ifndef GROUPABLE
#define GROUPABLE

#include "linkable_list.h"

namespace Core {
    namespace Web {
        class Groupable : public LinkableList {
            QString jsonToken() const { return QStringLiteral("groups"); }
        public:
            virtual QList<Linkable> findGroupsById(const QString & /*uid*/) { return QList<Linkable>(); }
            virtual QList<Linkable> findGroupsByName(const QString & /*name*/) { return QList<Linkable>(); }

            inline void addGroup(const Linkable & obj) { addLinkable(obj); }

            inline QHash<QString, Linkable> groupsList() const { return linkablesList(); }
            inline void clearGroups() { clearLinkables(); }
        };
    }
}


#endif // GROUPABLE
