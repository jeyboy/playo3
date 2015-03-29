#include "deleted_list.h"

using namespace Playo3;

DeletedList::DeletedList() {}

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

QJsonArray DeletedList::deletedToJson() {
    return QJsonArray::fromVariantList(list);
}
void DeletedList::deletedFromJson(QJsonArray array) {
    list = array.toVariantList();
}
