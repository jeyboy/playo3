#include "ignore_list.h"

using namespace Models;

IgnoreList::IgnoreList(QJsonObject * obj) {
    if (obj)
        ignoreListFromJson(obj -> take(jsonName()).toArray());
}

void IgnoreList::ignoreListAddUid(const QVariant & uid) { list.append(uid); }

void IgnoreList::ignoreListAddUids(QVariantList & uids) { list.append(uids); }

void IgnoreList::ignoreListRemoveUid(const QVariant & uid) { list.removeAll(uid); }
bool IgnoreList::ignoreListContainUid(const QVariant & uid) { return list.contains(uid); }

QJsonObject & IgnoreList::ignoreListToJson(QJsonObject & obj) {
    obj.insert(jsonName(), QJsonArray::fromVariantList(list));
    return obj;
}
void IgnoreList::ignoreListFromJson(const QJsonArray & array) { list = array.toVariantList(); }
