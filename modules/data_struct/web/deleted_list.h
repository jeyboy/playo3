#ifndef DELETED_LIST_H
#define DELETED_LIST_H

#include <QVariantList>
#include <QJsonArray>

namespace Playo3 {
    class DeletedList {
    public:
        DeletedList(QJsonObject * obj = 0);
        ~DeletedList();

        void addRemovedUID(QVariant uid);
        void removeUID(QVariant uid);
        bool containsUID(QVariant uid);

        QJsonObject & deletedToJson(QJsonObject & obj);
        void deletedFromJson(QJsonArray);
    private:
        inline QString jsonName() const { return "deleted"; }

        QVariantList list;
    };
}

#endif // DELETED_LIST_H
