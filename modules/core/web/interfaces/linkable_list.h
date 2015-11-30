#ifndef LINKABLE_LIST
#define LINKABLE_LIST

#include "linkable.h"

namespace Core {
    namespace Web {
        class LinkableList {
            virtual QString jsonToken() const = 0;
            QHash<QString, Linkable> linkables;
        protected:
            inline QHash<QString, Linkable> linkablesList() const { return linkables; }
            inline void clearLinkables() { linkables.clear(); }

            inline void addLinkable(const Linkable & obj) {
                if (!obj.uid().isEmpty() && !obj.human_name().isEmpty())
                    linkables.insert(obj.uid(), obj);
            }
        public:
            void fromJson(const QJsonObject & hash) {
                QJsonArray ar = hash.value(jsonToken()).toArray();
                for(QJsonArray::Iterator linkable = ar.begin(); linkable != ar.end(); linkable++)
                    addLinkable(Linkable::fromJson((*linkable).toObject()));
            }
            void toJson(QJsonObject & hash) {
                QJsonArray objsJson;

                for(QHash<QString, Linkable>::Iterator linkable = linkables.begin(); linkable != linkables.end(); linkable++)
                    objsJson.append(linkable.value().toJson());

                hash.insert(jsonToken(), objsJson);
            }
        };
    }
}

#endif // LINKABLE_LIST
