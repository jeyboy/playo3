#ifndef GROUPABLE
#define GROUPABLE

#include "linkable_list.h"

namespace Core {
    namespace Web {
        class Groupable : public LinkableList {
            QString jsonToken() const { return QStringLiteral("groups"); }
        public:
//            virtual QList<Linkable> findGroupById(const QString & uid) = 0;
//            virtual QList<Linkable> findGroupByName(const QString & name) = 0;

            inline void addGroup(const Linkable & obj) { addLinkable(obj); }

            inline QHash<QString, Linkable> groupsList() const { return linkablesList(); }
            inline void clearGroups() { clearLinkables(); }
        };
    }
}


#endif // GROUPABLE
