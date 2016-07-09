#ifndef LINKABLE_LIST
#define LINKABLE_LIST

#include <qlist.h>

#include "linkable.h"

namespace Core {
    namespace Web {
        class LinkableList {
            virtual QString jsonToken() const = 0;
        protected:
            QList<Linkable> linkables;

            inline QList<Linkable> linkablesList() const { return linkables; }
            inline void clearLinkables() { linkables.clear(); }

            inline void addLinkable(const Linkable & obj) {
                if (!obj.uid().isEmpty() && !obj.humanName().isEmpty())
                    linkables.append(obj);
            }
        public:
            virtual ~LinkableList() {}

            void fromJson(const QJsonObject & hash) {
                QJsonArray ar = hash.value(jsonToken()).toArray();
                for(QJsonArray::Iterator linkable = ar.begin(); linkable != ar.end(); linkable++)
                    linkables.append(Linkable::fromJson((*linkable).toObject()));
            }
            void toJson(QJsonObject & hash) {
                QJsonArray objsJson;

                for(QList<Linkable>::Iterator linkable = linkables.begin(); linkable != linkables.end(); linkable++)
                    objsJson.append((*linkable).toJson());

                hash.insert(jsonToken(), objsJson);
            }
        };
    }
}

#endif // LINKABLE_LIST
