#ifndef DELETED_LIST_H
#define DELETED_LIST_H

#include <QVariantList>
#include <QJsonArray>

class DeletedList {
public:
    DeletedList();
    ~DeletedList();

    void addRemovedUID(QVariant uid);
    void removeUID(QVariant uid);
    bool containsUID(QVariant uid);

    QJsonArray deletedToJson();
    void deletedFromJson(QJsonArray);
private:
    QVariantList list;
};

#endif // DELETED_LIST_H
