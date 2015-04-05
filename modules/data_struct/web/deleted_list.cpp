#include "deleted_list.h"

using namespace Playo3;

DeletedList::DeletedList(QJsonObject * obj) {
    if (obj)
        deletedFromJson(obj -> take(jsonName()).toArray());
}

DeletedList::~DeletedList() {}

void DeletedList::addRemovedUID(QVariant uid) {
    list.append(uid);
}
void DeletedList::removeUID(QVariant uid) {
    list.removeAll(uid);
}
bool DeletedList::containsUID(QVariant uid) {
    return list.contains(uid);
}

QJsonObject & DeletedList::deletedToJson(QJsonObject & obj) {
    obj.insert(jsonName(), QJsonArray::fromVariantList(list));
    return obj;
}
void DeletedList::deletedFromJson(QJsonArray array) {
    list = array.toVariantList();
}
